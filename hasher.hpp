#pragma once
#include <argon2.h>
#include <string>
#include <stdexcept>
#include <vector>
#include <random>

std::string hashPassword(const std::string& password) {
	// Generate a 16-byte random salt
	std::vector<uint8_t> salt(16);
	std::random_device rd;

	for (auto& b : salt) b = static_cast<uint8_t>(rd());
	
	// Output buffer for the encoded hash string
	const size_t encodedLen = 128;
	std::vector<char> encoded(encodedLen);

	int result = argon2id_hash_encoded(
		2,
		1 << 16,
		1,
		password.c_str(), password.size(),
		salt.data(), salt.size(),	
		32,
		encoded.data(), encoded.size()
	);

	if (result != ARGON2_OK) {
		throw std::runtime_error("Password hashing failed.");
	};
	
	return std::string(encoded.data());
}

bool verifyPassword(const std::string& password, const std::string& storedHash) {
	int result = argon2id_verify(storedHash.c_str(), password.c_str(), password.size());
	return result == ARGON2_OK;
}
