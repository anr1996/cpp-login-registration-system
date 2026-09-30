#include "userRegistry.hpp"

namespace userAccRegistry {
userSettings::userAccount* userRegistry::create(std::string name, std::string email, std::string password) {
	if (by_name_.contains(name)) {
		return nullptr;
	}
	
	auto acct = std::make_unique<userSettings::userAccount>(std::move(name), std::move(email), std::move(password));
	
	userSettings::userAccount* raw = acct.get();
	by_name_.emplace(std::string(raw->get_username()), std::move(acct));
	return raw;
}

userSettings::userAccount* userRegistry::find(std::string_view name) {
	auto it = by_name_.find(name);

	return it == by_name_.end() ? nullptr : it->second.get();
}

}
