#pragma once

#include <string>

namespace userSettings {

/*The personal details required to create a user profile.
This reduces the total parameters for functions simplifying their implementations.
*/

struct UserProfile {
	std::string firstName;
	std::string lastName;
	std::string dateOfBirth;
	std::string phoneNumber;
	std::string gender;
	std::string location;

};

} // namespace userSettings
