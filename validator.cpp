#include "validator.hpp"
#include "accountRules.hpp"
#include <chrono>
#include <charconv>
#include <regex>
#include <string_view>

namespace {

/*
Parses "YYYY-MM-DD" into a calendar date. Return false if the text is
the wrong shape or the data is non existent (Feb 30, month 13 etc...)
*/
bool parseDate(const std::string& text, std::chrono::year_month_day& out) {
	// date shape check first: exactly 10 characters, dashes in the right places
	if (text.size() != 10 || text[4] != '-' || text[7] != '-') {
		return false;
	}

	// Converts a slice of the text to an int, failing on non-digits.
	auto toInt = [&text](std::size_t start, std::size_t length, int& value) {
		const char* first = text.data() + start;
		const char* last = first + length;
		const auto result = std::from_chars(first, last, value);
		return result.ec == std::errc{} && result.ptr == last;
	};
		
	int y = 0;
	int m = 0;
	int d = 0;
	if (!toInt(0, 4, y) || !toInt(5, 2, m) || !toInt(8, 2, d)) {return false;}

	using namespace std::chrono;
	const year_month_day date{year{y}, month{static_cast<unsigned>(m)}, day{static_cast<unsigned>(d)}};
	if (!date.ok()) {return false;}
	
	out = date;
	return true;
}

} // namespace



namespace validator {

	bool isValidUsername(const std::string& username){
		static const std::regex pattern(
			"[a-zA-Z0-9][a-zA-Z0-9_.-]{1,28}[a-zA-Z0-9]$"
		);
		return std::regex_match(username, pattern);
	}
	
	bool isValidPassword(const std::string& password){
		static const std::regex pattern(
			"^.{12,128}$"
		);
		return std::regex_match(password, pattern);
	}
	
	bool isValidEmail(const std::string& email){
		static const std::regex pattern(
			"^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$"
		);
		return std::regex_match(email, pattern);
	}

	bool isValidName(const std::string& name){
		static const std::regex pattern(
			"^[a-zA-Z]+([ '-][a-zA-Z]+)*$"
		);
		return name.size() <= 50 && std::regex_match(name, pattern);
	}


	bool isValidPhone(const std::string& phoneNumber){
	static const std::regex pattern(
		"^[0-9]{10}$"
	);
	return std::regex_match(phoneNumber, pattern);
	}

	bool isValidGender(const std::string& gender) {
		return gender == "male" ||
		       gender == "female" ||
		       gender == "non_binary" ||
		       gender == "other";
	}


	bool isValidDateOfBirth(const std::string& dob) {
		std::chrono::year_month_day date;
		return parseDate(dob, date);	
	}

	bool isOldEnough(const std::string& dob, int minAge) {
		using namespace std::chrono;
		year_month_day birth;
		if (!parseDate(dob, birth)) {return false;}
		
		// floor<days> trim "now" down to a whole day, then converts it to a calendar date.
		const year_month_day today{floor<days>(system_clock::now())};
		
		// Birthday not reached yet this year means one year younger.
		int age = int(today.year()) - int(birth.year());
		if (today.month() < birth.month() || (today.month() == birth.month() && today.day() < birth.day())) {
			--age;
		}
		return age >= minAge;

	}

	bool isValidProfile(const userSettings::UserProfile& profile) {
	return isValidName(profile.firstName) &&
	       isValidName(profile.lastName) &&
	       isValidPhone(profile.phoneNumber) &&
	       isValidGender(profile.gender) &&
	       isValidDateOfBirth(profile.dateOfBirth) &&
	       !profile.location.empty() &&
	       isOldEnough(profile.dateOfBirth, accountRules::MIN_AGE);		

	}
}
