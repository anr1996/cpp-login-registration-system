#pragma once

#include <string>

namespace userSettings {
	// One search result row: who they are, and whether they current users follows them.
	struct UserSummary {
		std::string username;
		bool isFollowed;
	};
	
} // namespace userSettings


