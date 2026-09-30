#pragma once

#include "userRepository.hpp"

#include <memory>

namespace userAccRegistry {
/*
Connects to the database described by the LOGIN_APP_DB environment
variable and returns a repository backed by it.
Throws if the variable is missing or the connection fails.
*/

std::unique_ptr<userRepository> makePostgresUserRepository();
 
} // namespace userAccRegistry
