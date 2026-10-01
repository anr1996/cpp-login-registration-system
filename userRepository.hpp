#pragma once

#include "userAccount.hpp"

#include <optional>
#include <string>
#include <string_view>

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
	virtual std::optional<userSettings::userAccount> create(std::string name, std::string email, std::string password) = 0;
	
	// Returns the account with this username, or std::nullopt if there is none.
	virtual std::optional<userSettings::userAccount> find_name(std::string_view name) = 0;

	// Returns the account with this email, or std::nullopt if there is none.
	virtual std::optional<userSettings::userAccount> find_email(std::string_view email) = 0;

};

} // namespace userAccRegistry
