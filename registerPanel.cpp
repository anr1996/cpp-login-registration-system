#include "registerPanel.hpp"
#include "mainFrame.hpp"
#include "appConstants.hpp"
#include "validator.hpp"

#include <exception>
#include <iostream>
#include <optional>


RegisterPanel::RegisterPanel(wxWindow *parent, MainFrame *mainFrame) : wxPanel(parent), mainFrame_(mainFrame) {
	auto *sizer = new wxBoxSizer(wxVERTICAL);

	// Username row
	auto *userLabel = new wxStaticText(this, wxID_ANY, "Username:");
	userInput_ = new wxTextCtrl(this, wxID_ANY);
	
	// Email row
	auto *emailLabel = new wxStaticText(this, wxID_ANY, "Email:");
	emailInput_ = new wxTextCtrl(this, wxID_ANY);
	
	// Password row
	auto *passLabel = new wxStaticText(this, wxID_ANY, "Password:");
	
	// password is masked with dots/asterisks
	passInput_ = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);
	
	auto *registerBtn = new wxButton(this, wxID_ANY, "Register", wxDefaultPosition, wxSize(definedVar::BUTTON_WIDTH, definedVar::BUTTON_HEIGHT));
	auto *backBtn = new wxButton(this, wxID_ANY, "Back", wxDefaultPosition, wxSize(definedVar::BUTTON_WIDTH, definedVar::BUTTON_HEIGHT));

	sizer->Add(userLabel, 0, wxALL, definedVar::BORDER_WIDTH);
	sizer->Add(userInput_, 0, wxALL | wxEXPAND, definedVar::BORDER_WIDTH);	
	sizer->Add(emailLabel, 0, wxALL, definedVar::BORDER_WIDTH);
	sizer->Add(emailInput_, 0, wxALL | wxEXPAND, definedVar::BORDER_WIDTH);	
	sizer->Add(passLabel, 0, wxALL, definedVar::BORDER_WIDTH);
	sizer->Add(passInput_, 0, wxALL | wxEXPAND, definedVar::BORDER_WIDTH);	
	sizer->Add(registerBtn, 0, wxALL | wxALIGN_CENTER, definedVar::BORDER_WIDTH);
	sizer->Add(backBtn, 0, wxALIGN_CENTER | wxALL, definedVar::BORDER_WIDTH);

	SetSizer(sizer);
	
	// Connects the button's click event to the handler function below.
	registerBtn->Bind(wxEVT_BUTTON, &RegisterPanel::OnRegisterClick, this);
	backBtn->Bind(wxEVT_BUTTON, &RegisterPanel::OnBackClick, this);
}	

void RegisterPanel::OnRegisterClick(wxCommandEvent & /*event*/) {
	const wxString user = userInput_->GetValue();
	const wxString email = emailInput_->GetValue();
	const wxString pass = passInput_->GetValue();
	
	const bool validUserInfo =  validator::InitAccountValidator (user.ToStdString(), email.ToStdString(), pass.ToStdString());
	


 	if (!validUserInfo) {
		MainFrame::ShowLongMessage("Registration failed.", wxOK | wxICON_ERROR,
			     "Username must be 3 to 30 characters long."
			     "It can contain letters, numbers, underscores '_', dots'.', and hyphens'-'."
			     "It must begin with a letter or number."
			     "It must end with a letter or number." 
			     "The email must be a valid email address in the format: name@example.com."
			     "The part before the '@' may contain letters, numbers, and the symbol '.', '_' '%', '+', '-'."
			     "The part after the @ must include a domain and an ending such as .com, .org, or .co.uk (atleast 2 letters)."
			     "The password must be 12 to 128 characters long."
			     "Any character is allowed.");
		return;
	}
	
	try {  
		const std::optional<userSettings::userAccount> newAccount = MainFrame::getRegistry().create(user.ToStdString(), email.ToStdString(), pass.ToStdString());
		/*
		const std::optional<userSettings::userAccount> nameFound = MainFrame::getRegistry().find_name(user.ToStdString());
		const std::optional<userSettings::userAccount> emailFound = MainFrame::getRegistry().find_email(user.ToStdString());

		if (!(nameFound && emailFound)) {
			
			MainFrame::ShowLongMessage("Registration successful.", wxOK | wxICON_INFORMATION, "success");
		
		} else if (nameFound || emailFound) {
		
			MainFrame::ShowLongMessage("Registration failed.", wxOK | wxICON_ERROR, "That username or email is already registered");

		}
		*/ 
		
		if (newAccount) {
		
			MainFrame::ShowLongMessage("Registration successful.", wxOK | wxICON_INFORMATION, "success");

		} else {	
			
			MainFrame::ShowLongMessage("Registration failed.", wxOK | wxICON_ERROR, "That username or email is already registered");

		} 
		
	} catch (const std::exception &e) {
		std::cerr << "Database error: " << e.what() << "\n";
		MainFrame::ShowLongMessage("Registration failed.", wxOK | wxICON_ERROR, "Could not reach the database. Please try again.");
	  }
}

void RegisterPanel::OnBackClick(wxCommandEvent & /*event*/) {
	mainFrame_->ShowWelcome();
}
