#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include "hasher.hpp"

// enum to assign an int value to user input
enum class Action {
	loginIn,
	registerAccount,
	exitUI,
	Invalid
};

// stringToAction used to compare user input as a string to enum value
constexpr Action stringToAction(std::string_view str) {
	if (str == "login") return Action::loginIn;
	if (str == "register") return Action::registerAccount;
	if (str == "exit") return Action::exitUI;
	return Action::Invalid;
}


int main() {
	std::string choice;
	bool running = true;
	
	// Request input from user
	while (running) {
		std::cout << "\n=== Login System ===\n";
		std::cout << "\nChoose from the following options:\n";
		std::cout << "1.) login\n";
		std::cout << "2.) Register\n";
		std::cout << "3.) Exit\n";
		std::cin >> choice;
		
		// Using a switch statement to check user input.
		switch (stringToAction(choice)) {
			case Action::loginIn:
				std::cout << "Login selected (not built yet)\n";
				break;
			
			// Handle account Registration.
			case Action::registerAccount: {
				bool usernameTaken = false;
				std::string username, password;
				while(!usernameTaken){

					std::cout << "Enter a username ";
					std::cin >> username;
					
					std::ifstream checkFile("data.txt");
					std::string existingUsername, existingHash;
					
					if (checkFile.peek() == std::ifstream::traits_type::eof()){
						usernameTaken = true;
					}
					
					while (checkFile >> existingUsername >> existingHash) {
						if (existingUsername == username) {
							std::cout << "Username is already taken.\n";
							break;
						} else {
							usernameTaken = true;
						}
					};
						
						checkFile.close();
				}
				
				std::cout << "Enter a password ";
				std::cin >> password;
				std::string hashedPassword = hashPassword(password);

				
				std::ofstream outFile("data.txt", std::ios::app);
				if (!outFile) {
					std::cout << "Error: could not open data.txt for writing";					
					break;
				}
				
				outFile << username << " " << hashedPassword << "\n";
				outFile.close();
				
				std::cout << "Account creation successful\n"; 
				break;
			}
			
			case Action::exitUI:
				std::cout << "Exiting...\n";
				running = false;
				break;
			
			case Action::Invalid:
				std:: cout << "Invalid response, try again\n";
				break;
	
			default:
				std::cout << "unknown error.\n";

		}
	}
}
