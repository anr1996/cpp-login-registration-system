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

/*
One location that lists the columns and turns the row back into an account.
date_of_birth::text makes Postgres return "YYYY-MM-DD" as a string.
*/
constexpr const char* USER_COLUMNS = "username, email, password_hash, first_name, last_name, "
				     "date_of_birth::text, phone_number, gender, location";

userSettings::userAccount rowToAccount(const pqxx::row_ref& row) {
	return userSettings::userAccount::fromStoredHash(
	row[0].as<std::string>(),
	row[1].as<std::string>(),
	row[2].as<std::string>(),
	userSettings::UserProfile{
	.firstName = row[3].as<std::string>(),
	.lastName = row[4].as<std::string>(),
	.dateOfBirth = row[5].as<std::string>(),
	.phoneNumber = row[6].as<std::string>(),
	.gender = row[7].as<std::string>(),
	.location = row[8].as<std::string>()});
}

class PostgresUserRepository : public userRepository {
private:
	pqxx::connection conn_;

public:
	explicit PostgresUserRepository(const std::string& connectionInfo) : conn_(connectionInfo) {}
	

	std::optional<userSettings::userAccount> create(
							std::string name, 
							std::string email, 
							std::string password, 
							userSettings::UserProfile profile						
							) override {

		// Building the account hashes the password (see userAccount's constructor).
		userSettings::userAccount account(
							std::move(name), 
							std::move(email), 
							std::move(password),
							std::move(profile)	
							);

		const userSettings::UserProfile& databaseProfile = account.get_profile();
		
		pqxx::work txn(conn_);
		const pqxx::result inserted = txn.exec(
			"INSERT INTO users (username, email, password_hash, first_name, last_name, date_of_birth, phone_number, gender, location) "
			"VALUES ($1, $2, $3, $4, $5, $6::date, $7, $8, $9) "
			"ON CONFLICT DO NOTHING "
			"RETURNING id",
			pqxx::params{
				     std::string(account.get_username()),
				     std::string(account.get_email()),
				     std::string(account.get_pass()),
				     databaseProfile.firstName,				     
				     databaseProfile.lastName,
				     databaseProfile.dateOfBirth,				     
				     databaseProfile.phoneNumber,
    				     databaseProfile.gender,				     
				     databaseProfile.location});
	
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
			std::string("SELECT ") + USER_COLUMNS + " FROM users WHERE username = $1::citext",	
			pqxx::params{std::string(name)});
		
		if (rows.empty()) {
			return std::nullopt;
		}
		return rowToAccount(rows[0]);		
	
	}

	
	std::optional<userSettings::userAccount> find_email(std::string_view email) override {
		pqxx::work txn(conn_);
		const pqxx::result rows = txn.exec(
			std::string("SELECT ") + USER_COLUMNS + " FROM users WHERE email = $1::citext",	
			pqxx::params{std::string(email)});
	
		if (rows.empty()) {
			return std::nullopt;
		}
		return rowToAccount(rows[0]);	
	}

	bool create_post(std::string_view username, std::string body) override {
		pqxx::work txn{conn_};
		
		/*
		INSERT .. SELECT: instead of VALUES, the rows come from a SELECT.
		This will look up the user's id by username inside the same statement,
		so the userAccount doesn't need to carry an id around. If no user matches,
		the SELECT returns zero rows and nothing is inserted.
		*/
		pqxx::result result = txn.exec(
		"INSERT INTO posts (user_id, body) "
		"SELECT id, $2::text FROM users WHERE username = $1::citext "
		"RETURNING id",
		pqxx::params{std::string(username), body});
		
		if (result.empty()) {
			return false;
		}

		txn.commit();
		return true;
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


} // userAccRegistory
