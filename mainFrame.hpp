#pragma once

#include "appConstants.hpp"
#include "loginPanel.hpp"
#include "postgresUserRepository.hpp"
#include "registerPanel.hpp"
#include "welcomePanel.hpp"

#include <wx/artprov.h>
#include <wx/statline.h>
#include <wx/wx.h>

#include <memory>

// MainFrame - owns all three panels, switches which size is visiable.
class MainFrame : public wxFrame {
public:
	MainFrame();
	
	void ShowWelcome();
	void ShowLogin();
	void ShowRegister();

	static userAccRegistry::userRepository& getRegistry();
	static void ShowLongMessage(const wxString& title, int64_t style, const wxString& message);


private:
	wxBoxSizer *sizer_;
	WelcomePanel *welcomePanel_;
	LoginPanel *loginPanel_;
	RegisterPanel *registerPanel_;	
};
