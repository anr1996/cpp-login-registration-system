#include "userAccount.hpp"
#include <cstddef>
#include <functional>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>

// Transparent hasher so the map accepts string_view lookups without constructing a temporary std::string
struct TransparentStringHash {
	using is_transparent = void;
	
	std::size_t operator()(std::string_view sv) const noexcept {
		return std::hash<std::string_view>{}(sv);
	}
};

class userRegistry {
	
	private:
		std::unordered_map<std::string, std::unique_ptr<userSettings::userAccount>, TransparentStringHash, std::equal_to<>>by_name_;
	
	public:
		// Returns a non-owning pointer to the new account, or nullptr if the username is taken.
		// the caller must not delete it.
		userSettings::userAccount* create(std::string name, std::string password, std::string email);
		
		// Returns a non-owning pointer, or nullptr if not found.
		userSettings::userAccount* find(std::string_view name);
		
};
