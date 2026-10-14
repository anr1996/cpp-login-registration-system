#include "registerPanel.hpp"
#include "mainFrame.hpp"
#include "appConstants.hpp"
#include "userProfile.hpp"
#include "validator.hpp"
#include <exception>
#include <iostream>
#include <optional>


RegisterPanel::RegisterPanel(wxWindow *parent, MainFrame *mainFrame) : wxPanel(parent), mainFrame_(mainFrame) {
	
	SetBackgroundColour(definedVar::GetPanelColor());
	
	auto *card = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SIMPLE);
	card->SetBackgroundColour(wxColour(176,34,12));	
	
	auto *cardSizer = new wxBoxSizer(wxVERTICAL);
	
	// Username row
	auto *userLabel = new wxStaticText(card, wxID_ANY, "Username:");
	userLabel->SetForegroundColour(wxColour(255,255,255));
	userInput_ = new wxTextCtrl(card, wxID_ANY, "", wxDefaultPosition, wxSize(definedVar::BUTTON_WIDTH, -1));
	
	// Email row
	auto *emailLabel = new wxStaticText(card, wxID_ANY, "Email:");
	emailLabel->SetForegroundColour(wxColour(255,255,255));
	emailInput_ = new wxTextCtrl(card, wxID_ANY, "", wxDefaultPosition, wxSize(definedVar::BUTTON_WIDTH, -1));
	
	// Password row
	auto *passLabel = new wxStaticText(card, wxID_ANY, "Password:");
	passLabel->SetForegroundColour(wxColour(255,255,255));

	// password is masked with dots/asterisks
	passInput_ = new wxTextCtrl(card, wxID_ANY, "", wxDefaultPosition, wxSize(definedVar::BUTTON_WIDTH, -1), wxTE_PASSWORD);

	// first name row
	auto *firstNameLabel = new wxStaticText(card, wxID_ANY, "First name:");
	firstNameLabel->SetForegroundColour(wxColour(255,255,255));
	firstNameInput_ = new wxTextCtrl(card, wxID_ANY, "", wxDefaultPosition, wxSize(definedVar::BUTTON_WIDTH, -1));
	
	// last name row
	auto *lastNameLabel = new wxStaticText(card, wxID_ANY, "Last name:");
	lastNameLabel->SetForegroundColour(wxColour(255,255,255));
	lastNameInput_ = new wxTextCtrl(card, wxID_ANY, "", wxDefaultPosition, wxSize(definedVar::BUTTON_WIDTH, -1));

	// date of birth row
	auto *DOBLabel = new wxStaticText(card, wxID_ANY, "Date of birth:");
	DOBLabel->SetForegroundColour(wxColour(255,255,255));
	DOBInput_ = new wxTextCtrl(card, wxID_ANY, "", wxDefaultPosition, wxSize(definedVar::BUTTON_WIDTH, -1));

	// phone number row
	auto *phoneLabel = new wxStaticText(card, wxID_ANY, "Phone number:");
	phoneLabel->SetForegroundColour(wxColour(255,255,255));
	phoneInput_ = new wxTextCtrl(card, wxID_ANY, "", wxDefaultPosition, wxSize(definedVar::BUTTON_WIDTH, -1));

	// gender row
	auto *genderLabel = new wxStaticText(card, wxID_ANY, "Gender:");
	genderLabel->SetForegroundColour(wxColour(255,255,255));
	genderInput_ = new wxTextCtrl(card, wxID_ANY, "", wxDefaultPosition, wxSize(definedVar::BUTTON_WIDTH, -1));

	// Location row
	auto *locationLabel = new wxStaticText(card, wxID_ANY, "Location:");
	locationLabel->SetForegroundColour(wxColour(255,255,255));
	locationInput_ = new wxTextCtrl(card, wxID_ANY, "", wxDefaultPosition, wxSize(definedVar::BUTTON_WIDTH, -1));
	
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
	cardSizer->Add(firstNameLabel, 0, wxALL, definedVar::BORDER_WIDTH);
	cardSizer->Add(firstNameInput_, 0, wxALL | wxEXPAND, definedVar::BORDER_WIDTH);	
	cardSizer->Add(lastNameLabel, 0, wxALL, definedVar::BORDER_WIDTH);
	cardSizer->Add(lastNameInput_, 0, wxALL | wxEXPAND, definedVar::BORDER_WIDTH);	
	cardSizer->Add(DOBLabel, 0, wxALL, definedVar::BORDER_WIDTH);
	cardSizer->Add(DOBInput_, 0, wxALL | wxEXPAND, definedVar::BORDER_WIDTH);	
	cardSizer->Add(phoneLabel, 0, wxALL, definedVar::BORDER_WIDTH);
	cardSizer->Add(phoneInput_, 0, wxALL | wxEXPAND, definedVar::BORDER_WIDTH);	
	cardSizer->Add(genderLabel, 0, wxALL, definedVar::BORDER_WIDTH);
	cardSizer->Add(genderInput_, 0, wxALL | wxEXPAND, definedVar::BORDER_WIDTH);	
	cardSizer->Add(locationLabel, 0, wxALL, definedVar::BORDER_WIDTH);
	cardSizer->Add(locationInput_, 0, wxALL | wxEXPAND, definedVar::BORDER_WIDTH);	
	cardSizer->Add(registerBtn, 0, wxALL | wxALIGN_CENTER, definedVar::BORDER_WIDTH);
	cardSizer->Add(backBtn, 0, wxALIGN_CENTER | wxALL, definedVar::BORDER_WIDTH);
	
	card->SetSizer(cardSizer);
	card->Fit();
	
	auto *outer = new wxBoxSizer(wxVERTICAL);
	outer->AddStretchSpacer(1);
	outer->Add(card, 0, wxALIGN_CENTER_HORIZONTAL);
	outer->AddStretchSpacer();
	SetSizer(outer);
	
	// Connects the button's click event to the handler function below.
	registerBtn->Bind(wxEVT_BUTTON, &RegisterPanel::OnRegisterClick, this);
	backBtn->Bind(wxEVT_BUTTON, &RegisterPanel::OnBackClick, this);
}	

void RegisterPanel::OnRegisterClick(wxCommandEvent & /*event*/) {

	const userSettings::UserProfile profile {
		.firstName = firstNameInput_->GetValue().ToStdString(),
		.lastName = lastNameInput_->GetValue().ToStdString(),
		.dateOfBirth = DOBInput_->GetValue().ToStdString(),
		.phoneNumber = phoneInput_->GetValue().ToStdString(),
		.gender = genderInput_->GetValue().ToStdString(),
		.location = locationInput_->GetValue().ToStdString()
	
	};

	const std::string user = userInput_->GetValue().ToStdString(); 
	const std::string email = emailInput_->GetValue().ToStdString(); 
	const std::string pass = passInput_->GetValue().ToStdString();
	
													
	wxString message {};
	if (!(validator::isValidUsername(user))) {
		message +=  "Username must be 3 to 30 characters long.\n"
			     "It can contain letters, numbers, underscores '_', dots'.', and hyphens'-'.\n"
			     "It must begin with a letter or number.\n"
			     "It must end with a letter or number.\n";
	}


	if (!(validator::isValidEmail(email))) {
		message +=  "The email must be a valid email address in the format: name@example.com.\n"
			     "The part before the '@' may contain letters, numbers, and the symbol '.', '_' '%', '+', '-'.\n"
			     "The part after the @ must include a domain and an ending such as .com, .org, or .co.uk (atleast 2 letters).\n";
	}

	if (!(validator::isValidPassword(pass))) {
		message += "The password must be 12 to 128 characters long.\n"
			     "Any character is allowed.\n";
	}

	
	if (!(validator::isValidName(profile.firstName))) {
		message += "The first name may contain letters, spaces, hyphens and apostrophes (up to 50 characters).\n";
	}

	if (!(validator::isValidName(profile.lastName))) {
		message += "The last name may contain letters, spaces, hyphens and apostrophes (up to 50 characters).\n";
	}

	if (!(validator::isValidPhone(profile.phoneNumber))) {
		message += "The phone number must be 10 digits, 0 through 9 for any digit.\n";
	}

	if (!(validator::isValidGender(profile.gender))) {
		message += "The gender must be one of the following options: male, female, non_binary, other.\n";
	}	

 	if (!message.empty()) {
		MainFrame::ShowLongMessage("Registration failed.", wxOK | wxICON_ERROR, message);
		return;
	}

	wxString successMessage = "success";
	wxString registrationFailedMessage = "The username or email is already registered.";
	wxString databaseErrorMessage = "Could not reach the database. Please try again.";

	
	try {
		/*
		create() hashes the password internally and returns the std::nullopt if the username or email already exists.
		This is enforced by Postgres's 	UNIQUE constraints, checked via ON CONFLICT DO NOTHING.)
		*/ 
		const std::optional<userSettings::userAccount> newAccount = MainFrame::getRegistry().create(
														user, 
														email, 
														pass, 
														profile	
														);
	
				
		if (newAccount) {

			 MainFrame::ShowLongMessage("Registration successful.", wxOK | wxICON_INFORMATION, successMessage);
			 mainFrame_->ShowHome(*newAccount);
			
			 userInput_->Clear();
			 passInput_->Clear();
		 	 emailInput_->Clear();
			 firstNameInput_->Clear();
			 lastNameInput_->Clear();
			 DOBInput_->Clear();
			 phoneInput_->Clear();
			 genderInput_->Clear();
			 locationInput_->Clear();

		} else {	
			
			MainFrame::ShowLongMessage("Registration failed.", wxOK | wxICON_ERROR, registrationFailedMessage);
		} 
		
	} catch (const std::exception &e) {
		std::cerr << "Database error: " << e.what() << "\n";
		MainFrame::ShowLongMessage("Registration failed.", wxOK | wxICON_ERROR, databaseErrorMessage);
	  }


}

void RegisterPanel::OnBackClick(wxCommandEvent & /*event*/) {
	mainFrame_->ShowWelcome();
}

