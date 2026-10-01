#include "loginPanel.hpp"
#include "mainFrame.hpp"
#include "appConstants.hpp"

#include <exception>
#include <iostream>
#include <optional>

// LoginPanel
LoginPanel::LoginPanel(wxWindow *parent, MainFrame *mainFrame) : wxPanel(parent), mainFrame_(mainFrame) {

	// Set the background colour for the LoginPanel.
	SetBackgroundColour(wxColour(241, 233, 210));

	auto *card = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SIMPLE);
	card->SetBackgroundColour(wxColour(176,34,12));

	
	auto *cardSizer = new wxBoxSizer(wxVERTICAL);
	
	auto *userLabel = new wxStaticText(card, wxID_ANY, "Username:");	
	userLabel->SetForegroundColour(wxColour(255,255,255));
	
	/*
	Text fields are parented to "card" and sized to match the buttons
	below them, so the whole card reads as one consistent column.
	*/
	userInput_ = new wxTextCtrl(card, wxID_ANY, "", wxDefaultPosition, wxSize(definedVar::BUTTON_WIDTH, -1));

	auto *passLabel = new wxStaticText(card, wxID_ANY, "Password:");
	passLabel->SetForegroundColour(wxColour(255,255,255));
	passInput_ = new wxTextCtrl(card, wxID_ANY, "", wxDefaultPosition, wxSize(definedVar::BUTTON_WIDTH, -1), wxTE_PASSWORD);
	
	auto *loginBtn = new wxButton(card, wxID_ANY, "Login", wxDefaultPosition, wxSize(definedVar::BUTTON_WIDTH, definedVar::BUTTON_HEIGHT));
	loginBtn->SetBackgroundColour(wxColour(0,0,0));
	
	auto *backBtn = new wxButton(card, wxID_ANY, "Back", wxDefaultPosition, wxSize(definedVar::BUTTON_WIDTH, definedVar::BUTTON_HEIGHT));
	backBtn->SetBackgroundColour(wxColour(0,0,0));
	
	
	cardSizer->Add(userLabel, 0, wxALL, definedVar::BORDER_WIDTH);	
	cardSizer->Add(userInput_, 0, wxALL, definedVar::BORDER_WIDTH);		
	cardSizer->Add(passLabel, 0, wxALL, definedVar::BORDER_WIDTH);
	cardSizer->Add(passInput_, 0, wxALL, definedVar::BORDER_WIDTH);	
	cardSizer->Add(loginBtn, 0, wxALL, definedVar::BORDER_WIDTH);		
	cardSizer->Add(backBtn, 0, wxALL, definedVar::BORDER_WIDTH);

	card->SetSizer(cardSizer);
	card->Fit();

	/*
	To center the card (instead of pinning it to a corner like the welcome screen),
	add a strech spacer on both sides. each one expands to claim an equal share of leftover space,
	which mathematically centers whatever sits between them.
	*/
	auto *outer = new wxBoxSizer(wxVERTICAL);
	outer->AddStretchSpacer(1);
	outer->Add(card, 0, wxALIGN_CENTER_HORIZONTAL);
	outer->AddStretchSpacer();
	SetSizer(outer);

	
	loginBtn->Bind(wxEVT_BUTTON, &LoginPanel::OnLoginClick, this);
	backBtn->Bind(wxEVT_BUTTON, &LoginPanel::OnBackClick, this);
	
}

void LoginPanel::OnLoginClick(wxCommandEvent & /*event*/) {
	const wxString user = userInput_->GetValue();
	const wxString pass = passInput_->GetValue();
	
	try {
		/*
		find_name() looks up the username in Postgres (case insensitive via citext)
		and returns std::nullopt if no matching account exists.
		*/
		const std::optional<userSettings::userAccount> accountFound = MainFrame::getRegistry().find_name(user.ToStdString());
		if (accountFound && accountFound->check_password(pass.ToStdString())) {
			MainFrame::ShowLongMessage("Login successful.", wxOK | wxICON_INFORMATION, "success");
		} else {
			/*
			 Return the same message whether the username or password was wrong. 
			*/
			MainFrame::ShowLongMessage("Login failed.", wxOK | wxICON_ERROR, "Username or password is incorrect.");
		} 

		
	} catch (const std::exception &e) {
		/*
		If Postgres is not reachable, lipqxx throws. Catch it here so
		the whole app doesn't crash. Show a generic message and log the real
		technical details to the terminal instead.
		*/
		std::cerr << "Database error: " << e.what() << "\n";
		MainFrame::ShowLongMessage("Login failed.", wxOK | wxICON_ERROR, "Could not reach the database. Please try again.");
	}

}

void LoginPanel::OnBackClick(wxCommandEvent & /*event*/){
	mainFrame_->ShowWelcome();
}
