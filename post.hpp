#pragma once

#include <string>

namespace userSettings {
// One post as the UI needs it. Plain data, no database types.
struct Post {
	std::string author;
	std::string body;
	std::string createdAt;	// "YYYY-MM-DD HH24:MI", already formatted by the query
};

} // namespace userSettings


