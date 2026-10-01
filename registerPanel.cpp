#include "registerPanel.hpp"
#include "mainFrame.hpp"
#include "appConstants.hpp"
#include "validator.hpp"

#include <exception>
#include <iostream>
#include <optional>


RegisterPanel::RegisterPanel(wxWindow *parent, MainFrame *mainFrame) : wxPanel(parent), mainFrame_(mainFrame) {
	
	SetBackgroundColour(wxColour(0,0,0));
	
	auto *card = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SIMPLE);
	card->SetBackgroundColour(wxColour(176,34,12));	
	
	auto *cardSizer = new wxBoxSizer(wxVERTICAL);
	
	// Username row
	auto *userLabel = new wxStaticText(card, wxID_ANY, "Username:");
	userInput_ = new wxTextCtrl(card, wxID_ANY, "", wxDefaultPosition, wxSize(definedVar::BUTTON_WIDTH, -1));
	
	// Email row
	auto *emailLabel = new wxStaticText(card, wxID_ANY, "Email:");
	emailInput_ = new wxTextCtrl(card, wxID_ANY, "", wxDefaultPosition, wxSize(definedVar::BUTTON_WIDTH, -1));
	
	// Password row
	auto *passLabel = new wxStaticText(card, wxID_ANY, "Password:");
	
	// password is masked with dots/asterisks
	passInput_ = new wxTextCtrl(card, wxID_ANY, "", wxDefaultPosition, wxSize(definedVar::BUTTON_WIDTH, -1), wxTE_PASSWORD);
	
	auto *registerBtn = new wxButton(card, wxID_ANY, "Register", wxDefaultPosition, wxSize(definedVar::BUTTON_WIDTH, definedVar::BUTTON_HEIGHT));
	registerBtn->SetBackgroundColour(wxColour(0,0,0));
	
	auto *backBtn = new wxButton(card, wxID_ANY, "Back", wxDefaultPosition, wxSize(definedVar::BUTTON_WIDTH, definedVar::BUTTON_HEIGHT));
	backBtn->SetBackgroundColour(wxColour(0,0,0));
	
	cardSizer->Add(userLabel, 0, wxALL, definedVar::BORDER_WIDTH);
	cardSizer->Add(userInput_, 0, wxALL | wxEXPAND, definedVar::BORDER_WIDTH);	
	cardSizer->Add(emailLabel, 0, wxALL, definedVar::BORDER_WIDTH);
	cardSizer->Add(emailInput_, 0, wxALL | wxEXPAND, definedVar::BORDER_WIDTH);	
	cardSizer->Add(passLabel, 0, wxALL, definedVar::BORDER_WIDTH);
	cardSizer->Add(passInput_, 0, wxALL | wxEXPAND, definedVar::BORDER_WIDTH);	
	cardSizer->Add(registerBtn, 0, wxALL | wxALIGN_CENTER, definedVar::BORDER_WIDTH);
	cardSizer->Add(backBtn, 0, wxALIGN_CENTER | wxALL, definedVar::BORDER_WIDTH);
	
	card->SetSizer(cardSizer);
	card->Fit();
	
	auto *outer = new wxBoxSizer(wxVERTICAL);
	outer->AddStretchSpacer(1);
	outer->Add(card, 0, wxALIGN_CENTER_HORIZONTAL);
	outer->AddStretchSpacer(1);
	SetSizer(outer);
	
	// Connects the button's click event to the handler function below.
	registerBtn->Bind(wxEVT_BUTTON, &RegisterPanel::OnRegisterClick, this);
	backBtn->Bind(wxEVT_BUTTON, &RegisterPanel::OnBackClick, this);
}	

void RegisterPanel::OnRegisterClick(wxCommandEvent & /*event*/) {
	const wxString user = userInput_->GetValue();
	const wxString email = emailInput_->GetValue();
	const wxString pass = passInput_->GetValue();
	
	/*
	Validate before touching the database. Rejects bad input early which means we never attempt
	an insert with data that fails our criteria.
	*/
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
		/*
		create() hashes the password internally and returns the std::nullopt if the username or email already exists.
		This is enforced by Postgres's 	UNIQUE constraints, checked via ON CONFLICT DO NOTHING.)
		*/ 
		const std::optional<userSettings::userAccount> newAccount = MainFrame::getRegistry().create(user.ToStdString(), email.ToStdString(), pass.ToStdString());
	
		
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
