#include "loginPanel.hpp"
#include "mainFrame.hpp"
#include "appConstants.hpp"

#include <exception>
#include <iostream>
#include <optional>

// LoginPanel
LoginPanel::LoginPanel(wxWindow *parent, MainFrame *mainFrame) : wxPanel(parent), mainFrame_(mainFrame) {
	auto *sizer = new wxBoxSizer(wxVERTICAL);
	
	auto *userLabel = new wxStaticText(this, wxID_ANY, "Username:");
	userInput_ = new wxTextCtrl(this, wxID_ANY);
	
	auto *passLabel = new wxStaticText(this, wxID_ANY, "Password:");
	passInput_ = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);
	
	auto *loginBtn = new wxButton(this, wxID_ANY, "Login", wxDefaultPosition, wxSize(definedVar::BUTTON_WIDTH, definedVar::BUTTON_HEIGHT));
	auto *backBtn = new wxButton(this, wxID_ANY, "Back", wxDefaultPosition, wxSize(definedVar::BUTTON_WIDTH, definedVar::BUTTON_HEIGHT));

	sizer->Add(userLabel, 0, wxALL, definedVar::BORDER_WIDTH);	
	sizer->Add(userInput_, 0, wxALL | wxEXPAND, definedVar::BORDER_WIDTH);		
	sizer->Add(passLabel, 0, wxALL, definedVar::BORDER_WIDTH);
	sizer->Add(passInput_, 0, wxALL | wxEXPAND, definedVar::BORDER_WIDTH);	
	sizer->Add(loginBtn, 0, wxALIGN_CENTER | wxALL, definedVar::BORDER_WIDTH);		
	sizer->Add(backBtn, 0, wxALIGN_CENTER | wxALL, definedVar::BORDER_WIDTH);

	SetSizer(sizer);

	loginBtn->Bind(wxEVT_BUTTON, &LoginPanel::OnLoginClick, this);
	backBtn->Bind(wxEVT_BUTTON, &LoginPanel::OnBackClick, this);
	
}

void LoginPanel::OnLoginClick(wxCommandEvent & /*event*/) {
	const wxString user = userInput_->GetValue();
	const wxString pass = passInput_->GetValue();
	
	try {
		const std::optional<userSettings::userAccount> accountFound = MainFrame::getRegistry().find_name(user.ToStdString());
		if (accountFound && accountFound->check_password(pass.ToStdString())) {
			MainFrame::ShowLongMessage("Login successful.", wxOK | wxICON_INFORMATION, "success");
		} else {
			MainFrame::ShowLongMessage("Login failed.", wxOK | wxICON_ERROR, "Username or password is incorrect.");
		} 

		
	} catch (const std::exception &e) {
		std::cerr << "Database error: " << e.what() << "\n";
		MainFrame::ShowLongMessage("Login failed.", wxOK | wxICON_ERROR, "Could not reach the database. Please try again.");
	}

}

void LoginPanel::OnBackClick(wxCommandEvent & /*event*/){
	mainFrame_->ShowWelcome();
}
