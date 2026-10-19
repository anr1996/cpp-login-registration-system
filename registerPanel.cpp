#include "registerPanel.hpp"
#include "mainFrame.hpp"
#include "appConstants.hpp"
#include "accountRules.hpp"
#include "userProfile.hpp"
#include "validator.hpp"
#include <array>
#include <cstddef>
#include <exception>
#include <iostream>
#include <optional>
#include <string>
#include <vector>


namespace {
// label = what the user sees, value = what is validated and stored in the database.
struct GenderOption {
	const char* label;
	const char* value;
};

constexpr std::array<GenderOption, 4> GENDER_OPTIONS {{
	{"Male", "male"},
	{"Female", "female"},
	{"Non-binary", "non_binary"},
	{"Other", "other"}}};



} // namespace


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

	// Password is masked with dots/asterisks
	passInput_ = new wxTextCtrl(card, wxID_ANY, "", wxDefaultPosition, wxSize(definedVar::BUTTON_WIDTH, -1), wxTE_PASSWORD);

	// Confirm password row
	auto *confirmPassLabel = new wxStaticText(card, wxID_ANY, "Confirm password:");
	confirmPassLabel->SetForegroundColour(wxColour(255,255,255));

	// Confirmed password is masked with dots/asterisks
	confirmPassInput_ = new wxTextCtrl(card, wxID_ANY, "", wxDefaultPosition, wxSize(definedVar::BUTTON_WIDTH, -1), wxTE_PASSWORD);
	
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
	
	// Calendar-style picker. Dates after (today minus MIN_AGE years) are not available.
	latestAllowed_ = wxDateTime::Today();
	latestAllowed_.Subtract(wxDateSpan::Years(accountRules::MIN_AGE));

	DOBInput_ = new wxDatePickerCtrl(card, wxID_ANY, latestAllowed_, wxDefaultPosition, wxSize(definedVar::BUTTON_WIDTH, -1), wxDP_DROPDOWN);
	DOBInput_->SetRange(wxDateTime(1, wxDateTime::Jan, 1900), latestAllowed_);

	// phone number row
	auto *phoneLabel = new wxStaticText(card, wxID_ANY, "Phone number:");
	phoneLabel->SetForegroundColour(wxColour(255,255,255));
	phoneInput_ = new wxTextCtrl(card, wxID_ANY, "", wxDefaultPosition, wxSize(definedVar::BUTTON_WIDTH, -1));
	
	auto *genderLabel = new wxStaticText(card, wxID_ANY, "Gender:");
	genderLabel->SetForegroundColour(wxColour(255,255,255));
	
	// gender row
	wxArrayString genderLabels;
	for (const GenderOption& option : GENDER_OPTIONS) {
		genderLabels.Add(option.label);	
	}
 	
	genderInput_ = new wxChoice(card, wxID_ANY, wxDefaultPosition, wxSize(definedVar::BUTTON_WIDTH, -1), genderLabels);
	genderInput_->SetBackgroundColour((wxColour(0, 0, 0)));

	// Location row
	auto *locationLabel = new wxStaticText(card, wxID_ANY, "Location:");
	locationLabel->SetForegroundColour(wxColour(255,255,255));
	locationInput_ = new wxTextCtrl(card, wxID_ANY, "", wxDefaultPosition, wxSize(definedVar::BUTTON_WIDTH, -1));

	locationSuggestions_ = new wxListBox(card, wxID_ANY, wxDefaultPosition, wxSize(definedVar::BUTTON_WIDTH, definedVar::SUGGESTION_BOX_HEIGHT));
	locationSuggestions_->Hide();	
	
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
	cardSizer->Add(confirmPassLabel, 0, wxALL, definedVar::BORDER_WIDTH);
	cardSizer->Add(confirmPassInput_, 0, wxALL | wxEXPAND, definedVar::BORDER_WIDTH);	
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
	cardSizer->Add(locationSuggestions_, 0, wxLEFT | wxRIGHT | wxEXPAND, definedVar::BORDER_WIDTH);
	
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
	locationInput_->Bind(wxEVT_TEXT, &RegisterPanel::OnLocationText, this);
	locationSuggestions_->Bind(wxEVT_LISTBOX, &RegisterPanel::OnSuggestionSelected, this);
}	

void RegisterPanel::OnRegisterClick(wxCommandEvent & /*event*/) {

	const int genderIndex = genderInput_->GetSelection();
	const std::string genderValue = (genderIndex == wxNOT_FOUND) ? "" : GENDER_OPTIONS.at(static_cast<std::size_t>(genderIndex)).value;

	/*
	Resolve the typed location to the official city label. Empty means
	"not found", and the validation below turns that into a message.
	*/
	std::string officialLocation {};
	wxString locationMessage {};
	const std::string typedLocation = locationInput_->GetValue().ToStdString();
	
	if (typedLocation.empty()) {
		locationMessage = "Please choose a location.\n";
	} else {
		try {
			const std::optional<std::string> found = MainFrame::getRegistry().find_city(typedLocation);
			if (found) {
				officialLocation = *found;
			} else {
				locationMessage = "Please pick a location from the suggestions in the form 'City, ST, CC' (for example 'Boston, MA, US').\n";
			}
		} catch (const std::exception &e) {
			std::cerr << "City check error: " << e.what() << "\n";
			locationMessage = "Could not verify the location. Please try again.\n";
		}
	}

	const userSettings::UserProfile profile {
		.firstName = firstNameInput_->GetValue().ToStdString(),
		.lastName = lastNameInput_->GetValue().ToStdString(),
		.dateOfBirth = DOBInput_->GetValue().FormatISODate().ToStdString(),
		.phoneNumber = phoneInput_->GetValue().ToStdString(),
		.gender = genderValue,
		.location = officialLocation
	
	};

	const std::string user = userInput_->GetValue().ToStdString(); 
	const std::string email = emailInput_->GetValue().ToStdString(); 
	const std::string pass = passInput_->GetValue().ToStdString();
	const std::string confirmPass = confirmPassInput_->GetValue().ToStdString();	
													
	wxString message {};
	
	if (pass != confirmPass) {
		message += "password and confirmation password must match.\n";
	}

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
		message += "Please choose a gender from the list.\n";
	}

	if (!(validator::isValidDateOfBirth(profile.dateOfBirth))) {
		message += "Please provide a valid date of birth in the format 'YYYY-MM-DD'.\n";
	} else if (!(validator::isOldEnough(profile.dateOfBirth, accountRules::MIN_AGE))) {
		std::string requiredAge = std::to_string(accountRules::MIN_AGE);
		message += "You must be at least " + requiredAge + " years old to register.\n";
	}

	message += locationMessage;
	
 	if (!message.empty()) {
		MainFrame::ShowLongMessage("Registration failed.", wxOK | wxICON_ERROR, message);
		return;
	}

	wxString successMessage = "success";
	wxString registrationFailedMessage = "The username or email is already registered.\n";
	wxString databaseErrorMessage = "Could not reach the database. Please try again.\n";
	
		
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
		 	 confirmPassInput_->Clear();
		         emailInput_->Clear();
			 firstNameInput_->Clear();
			 lastNameInput_->Clear();
			 DOBInput_->SetValue(latestAllowed_);
			 phoneInput_->Clear();
			 genderInput_->SetSelection(wxNOT_FOUND);
			 locationInput_->Clear();
			 HideSuggestions();

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

void RegisterPanel::OnLocationText(wxCommandEvent & /*event*/) {
	const wxString typed = locationInput_->GetValue();
	
	// Too short: a 1-2 letter prefix would match thousands of cities.
	if (typed.length() < definedVar::MIN_LOCATION_CHARS) {
		
		HideSuggestions();
		return;
	}
	
	try {
		const std::vector<std::string> labels = 
			MainFrame::getRegistry().search_cities(typed.ToStdString(), definedVar::MAX_SUGGESTIONS);

		locationSuggestions_->Clear();			
		for (const std::string &label : labels) {
			locationSuggestions_->Append(wxString::FromUTF8(label));
		}

		// Show the list only when there is something to pick.
		if (labels.empty()) {
			HideSuggestions();
			
		} else {
			locationSuggestions_->Show();
			ResizeCard();
		}
	
	} catch (const std::exception &e) {
		// Suggestions are a convenience, so a database problem here is logged but not shown.
		std::cerr << "City search error: " << e.what() << "\n";
	}
	
}

void RegisterPanel::HideSuggestions() {
	
	if (locationSuggestions_->IsShown()) {
		locationSuggestions_->Hide();
		ResizeCard();	
	}
}

// The card's sizer must recompute its size after a child is shown or hidden.
void RegisterPanel::ResizeCard() {
	locationInput_->GetParent()->Layout();
	locationInput_->GetParent()->Fit();
	Layout();
}

void RegisterPanel::OnSuggestionSelected(wxCommandEvent & /*event*/) {
	
	const int index = locationSuggestions_->GetSelection();
	if (index == wxNOT_FOUND) {
		return;
	}
/*
Clicking an item copies it into the box. Changing the text fires wxEVT_TEXT, which would rebuild the list. 
ChangeValue() sets the text without firing that event. so the list stays closed after a pick.
*/
locationInput_->ChangeValue(locationSuggestions_->GetString(static_cast<unsigned>(index)));
	HideSuggestions();
}


