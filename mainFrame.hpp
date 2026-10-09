#pragma once

#include "appConstants.hpp"
#include "homePanel.hpp"
#include "loginPanel.hpp"
#include "postgresUserRepository.hpp"
#include "registerPanel.hpp"
#include "welcomePanel.hpp"

#include <wx/artprov.h>
#include <wx/statline.h>
#include <wx/wx.h>

#include <memory>
#include <optional>



// MainFrame - owns all three panels, switches which size is visiable.
class MainFrame : public wxFrame {
public:
	MainFrame();
	
	void ShowWelcome();
	void ShowLogin();
	void ShowRegister();

	/*
	Switches to the home screen for the given account, remembering it
	as the "current user" so HomePanel can ask who is logged in.
	*/
	void ShowHome(userSettings::userAccount account);

	// Clears the current user and returns to the welcome screen.
	void Logout();

	/*
	Returns the account logged in, or nullptr if nobody is.
	Non-owning: the caller must not delete this pointer.
	*/
	const userSettings::userAccount* GetCurrentUser() const;
	
	static userAccRegistry::userRepository& getRegistry();
	static void ShowLongMessage(const wxString& title, int64_t style, const wxString& message);


private:
	wxBoxSizer *sizer_;
	WelcomePanel *welcomePanel_;
	LoginPanel *loginPanel_;
	RegisterPanel *registerPanel_;	
	HomePanel *homePanel_;
	
	// Holds the logged-in account, or std::nullopt if nobody is logged in.
	std::optional<userSettings::userAccount> currentUser_;

};
