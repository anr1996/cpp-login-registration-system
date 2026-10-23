#pragma once

#include "post.hpp"
#include "userAccount.hpp"
#include <optional>
#include <string>
#include <string_view>
#include <vector>


namespace userAccRegistry {

/*
Interface for anything that can store and look up user accounts.
It says what a storage backend must do, not how it does it.
*/
class userRepository {
public:
	virtual ~userRepository() = default;
	userRepository() = default;
	userRepository(const userRepository&) = delete;
	userRepository& operator=(const userRepository&) = delete;
	userRepository(userRepository&&) = delete;
	userRepository& operator=(userRepository&&) = delete;

	
	// Creates and stores a new account (the password gets hashed).
	// Returns the new account, or std:::nullopt if the username or email is already in use.
	virtual std::optional<userSettings::userAccount> create(
								std::string name, 
								std::string email, 
								std::string password, 
								userSettings::UserProfile profile	
								) = 0;
	
	// Returns the account with this username, or std::nullopt if there is none.
	virtual std::optional<userSettings::userAccount> find_name(std::string_view name) = 0;

	// Returns the account with this email, or std::nullopt if there is none.
	virtual std::optional<userSettings::userAccount> find_email(std::string_view email) = 0;

	
	// Saves a new post for the given username. Returns false if the username doesn't exist (nothing is saved in that case).	
	virtual bool create_post(std::string_view username, std::string body) = 0;

	// Returns up to 'limit' posts by this user, newest first.
	virtual std::vector<userSettings::Post> get_posts(std::string_view username, int limit) = 0;
	
	// Returns up to 'limit' city labels starting with 'prefix' (case-insensitive).
	virtual std::vector<std::string> search_cities(std::string_view prefix, int limit) = 0;
	
	/*
	Returns the official label (such as "Keizer, OR, US") for a typed location.
	matching case-insensitively, or std::nullopt if no such city exists.
	*/	
	virtual std::optional<std::string> find_city(std::string_view label) = 0;	
};

} // namespace userAccRegistry
