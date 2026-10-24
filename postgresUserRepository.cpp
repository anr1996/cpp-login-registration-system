#include "postgresUserRepository.hpp"
#include <pqxx/pqxx>
#include <cstdlib>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>


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
	
	std::vector<userSettings::Post> get_posts(std::string_view username, int limit) override {
		pqxx::work txn(conn_);
		
		// Posts by this user, plus posts by anyone this user follows.
		const pqxx::result rows = txn.exec(
			"SELECT u.username::text, p.body, to_char(p.created_at, 'YYYY-MM-DD HH24:MI') "
			"FROM posts p JOIN users u ON u.id = p.user_id "
			"WHERE p.user_id = (SELECT id FROM users WHERE username = $1::citext) "
			"	OR p.user_id IN (SELECT f.followee_id FROM follows f "
			"		JOIN users me ON me.id = f.follower_id "
			"		WHERE me.username = $1::citext) "
			"ORDER BY p.created_at DESC, p.id DESC "
			"LIMIT $2",
			pqxx::params{std::string(username), limit});
		
		std::vector<userSettings::Post> posts;
		posts.reserve(rows.size());
		for (const auto& row : rows) {
			posts.push_back(userSettings::Post{
				.author = row[0].as<std::string>(),
				.body = row[1].as<std::string>(), 
				.createdAt = row[2].as<std::string>()});
		}
		return posts;

	}

	std::vector<userSettings::UserSummary> search_users(std::string_view viewer, std::string_view prefix, int limit) override {
		pqxx::work txn(conn_);
		const pqxx::result rows = txn.exec(
			"SELECT u.username::text, "
			"	EXISTS (SELECT 1 FROM follows f "
			"		WHERE f.followee_id = u.id "
			"			AND f.follower_id = (SELECT id FROM users WHERE username = $1::citext)) "
			"FROM users u "
			"WHERE u.username::text ILIKE $2 || '%' "
			"	AND u.username <> $1::citext "
			"ORDER BY u.username::text "
			"LIMIT $3",
			pqxx::params{std::string(viewer), std::string(prefix), limit});	
	
		std::vector<userSettings::UserSummary> users;
		users.reserve(rows.size());
		for (const auto& row : rows) {
			users.push_back(userSettings::UserSummary{
			.username = row[0].as<std::string>(),
			.isFollowed = row[1].as<bool>()});
		}
		return users;
	}

	
	bool follow(std::string_view follower, std::string_view followee) override {
		pqxx::work txn(conn_);
		/*
		INSERT .. SELECT will look up both ids by username in one statement.
		// ON CONFLICT DO NOTHING makes following twice harmless.
		*/
		txn.exec(
			"INSERT INTO follows (follower_id, followee_id) "
			"SELECT a.id, b.id FROM users a, users b "
			"WHERE a.username = $1::citext AND b.username = $2::citext AND a.id <> b.id "
			"ON CONFLICT DO NOTHING",

			pqxx::params{std::string(follower), std::string(followee)});
		txn.commit();
		return true;
	}

	bool unfollow(std::string_view follower, std::string_view followee) override {
		pqxx::work txn(conn_);
		const pqxx::result removed = txn.exec(
			"DELETE FROM follows "
			"WHERE follower_id = (SELECT id FROM users WHERE username = $1::citext) "
			" AND followee_id = (SELECT id FROM users WHERE username = $2::citext) "
			"RETURNING follower_id",
			pqxx::params{std::string(follower), std::string(followee)});
		txn.commit();
		return !removed.empty();
		
	}


	std::vector<std::string> search_cities(std::string_view prefix, int limit) override {
		pqxx::work txn(conn_);
		const pqxx::result rows = txn.exec(
			"SELECT label::text FROM cities "
			"WHERE label::text ILIKE $1 || '%' "
			"ORDER BY label::text LIMIT $2",
			pqxx::params{std::string(prefix), limit});

		std::vector<std::string> labels;
		labels.reserve(rows.size());
		for (const auto& row : rows) {
			labels.push_back(row[0].as<std::string>());
		}
		return labels;
	}
	
	std::optional<std::string> find_city(std::string_view label) override {
		pqxx::work txn(conn_);
		const pqxx::result rows = txn.exec(
			"SELECT label::text FROM cities WHERE label = $1::citext",
			pqxx::params{std::string(label)});
		if (rows.empty()) {
			return std::nullopt;
		}
		return rows[0][0].as<std::string>();

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
