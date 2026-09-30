#include "postgresUserRepository.hpp"

#include <pqxx/pqxx>

#include <cstdlib>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

namespace userAccRegistry {

namespace {
class PostgresUserRepository : public userRepository {
private:
	pqxx::connection conn_;

public:
	explicit PostgresUserRepository(const std::string& connectionInfo) : conn_(connectionInfo) {}
	std::optional<userSettings::userAccount> create(std::string name, std::string email, std::string password) override {
		// Building the account hashes the password (see userAccount's constructor).
		userSettings::userAccount account(std::move(name), std::move(email), std::move(password));
		
		pqxx::work txn(conn_);
		const pqxx::result inserted = txn.exec(
			"INSERT INTO users (username, email, password_hash) "
			"VALUES ($1, $2, $3) "
			"ON CONFLICT DO NOTHING "
			"RETURNING id",
			pqxx::params{std::string(account.get_username()),
				     std::string(account.get_email()),
				     std::string(account.get_pass())});
	
		// No row back means the database skipped the insert because the username or email already exists.
		if (inserted.empty()) {
			return std::nullopt;
		}
		
		txn.commit();
		return account;
	}

	std::optional<userSettings::userAccount> find_name(std::string_view name) override {
		pqxx::work txn(conn_);
		const pqxx::result rows = txn.exec(
			"SELECT username, email, password_hash FROM users WHERE username = $1::citext",
			pqxx::params{std::string(name)});
		
		if (rows.empty()) {
			return std::nullopt;
		}
		
		// A SELECT changes nothing, so there is nothing to commit.
		const auto row = rows[0];
		return userSettings::userAccount::fromStoredHash(
			row[0].as<std::string>(),
			row[1].as<std::string>(),
			row[2].as<std::string>());
	}

	
	std::optional<userSettings::userAccount> find_email(std::string_view email) override {
	pqxx::work txn(conn_);
	const pqxx::result rows = txn.exec(
		"SELECT username, email, password_hash FROM users WHERE email = $2::citext",
		pqxx::params{std::string(email)});
	
	if (rows.empty()) {
		return std::nullopt;
	}
	
	// A SELECT changes nothing, so there is nothing to commit.
	const auto row = rows[0];
	return userSettings::userAccount::fromStoredHash(
		row[0].as<std::string>(),
		row[1].as<std::string>(),
		row[2].as<std::string>());
	}


};

} // namespace

std::unique_ptr<userRepository> makePostgresUserRepository() {
	const char* connectionInfo = std::getenv("LOGIN_APP_DB");
	if (connectionInfo == nullptr) {
		throw std::runtime_error("The LOGIN_APP_DB environment variable is not set.");
	}
	return std::make_unique<PostgresUserRepository>(connectionInfo);
	
}

} // namespace userAccRegistory
