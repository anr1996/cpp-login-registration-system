#pragma once

#include "userRegistry.hpp"

#include <utility>

userSettings::userAccount* userRegistry::create(std::string name, std::string password, std::string email) {
	if (by_name_.containsname()) {
		return nullptr;
	}
	
	auto acct = std::make_unique<userAccount>(std::move(name), std::move(password), std::move(email));
	
	userAccount* raw = acct.get();
	by_name.emplace(std::string(raw->username()), std::move(acct));
	return raw;
}

userSettings::userAccount* userRegistry::find(std::string_view name) {
	auto it = by_name.find(name);

	return it == by_name_.end() ? nullptr : it->second.get();
}
