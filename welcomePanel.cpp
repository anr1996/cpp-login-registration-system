#include "welcomePanel.hpp"
#include "mainFrame.hpp"
#include "appConstants.hpp"


// WelcomePanel
WelcomePanel::WelcomePanel(wxWindow *parent, MainFrame *mainFrame) : wxPanel(parent), mainFrame_(mainFrame) {
	
	// Sets the background color for the panel.
	SetBackgroundColour(wxColour(0,0,0));


	// The "card" is a second, smaller panel living inside the WelcomePanel.
	auto *card = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SIMPLE);
	card->SetBackgroundColour(wxColour(176,34,12));
	
	// cardSizer stacks the card's own widgets vertically.
	auto *cardSizer = new wxBoxSizer(wxVERTICAL);
	
	// the buttons belong to "card", not "this". They are a part of the small colored panel.
	auto *loginBtn = new wxButton(card, wxID_ANY, "Login", wxDefaultPosition, wxSize(definedVar::BUTTON_WIDTH, definedVar::BUTTON_HEIGHT));
	loginBtn->SetBackgroundColour(wxColour(0,0,0));
	
	auto * registerBtn = new wxButton(card, wxID_ANY, "register", wxDefaultPosition, wxSize(definedVar::BUTTON_WIDTH, definedVar::BUTTON_HEIGHT));
	registerBtn->SetBackgroundColour(wxColour(0,0,0));
		
	cardSizer->Add(loginBtn, 0, wxALL, definedVar::BORDER_WIDTH);
	cardSizer->Add(registerBtn, 0, wxALL, definedVar::BORDER_WIDTH);

	card->SetSizer(cardSizer);
	
	// Fit() shrinks the card down to exactly the size its two buttons need instead of leaving it to
	// an arbitrary/default size.
	card->Fit();

	
	/*
	outer is "this" panel's own sizer. It decides where the
	already-correctly-sized card sits: pinned to the top-right corner.
	A single stretch spacer below the card consumes all leftover vertical space, 
	holding the card up against the top edge.
	*/
	auto *outer = new wxBoxSizer(wxVERTICAL);
	outer->Add(card, 0, wxALIGN_RIGHT | wxTOP | wxRIGHT, 30);
	outer->AddStretchSpacer();
	SetSizer(outer);

	loginBtn->Bind(wxEVT_BUTTON, &WelcomePanel::OnLoginClick, this);	
	registerBtn->Bind(wxEVT_BUTTON, &WelcomePanel::OnRegisterClick, this);
}	

void WelcomePanel::OnLoginClick(wxCommandEvent & /*event*/){
	mainFrame_->ShowLogin();
}

void WelcomePanel::OnRegisterClick(wxCommandEvent & /*event*/){
	mainFrame_->ShowRegister();
}


