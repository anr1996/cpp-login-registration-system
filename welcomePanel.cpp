#include "welcomePanel.hpp"
#include "mainFrame.hpp"
#include "appConstants.hpp"


// WelcomePanel
WelcomePanel::WelcomePanel(wxWindow *parent, MainFrame *mainFrame) : wxPanel(parent), mainFrame_(mainFrame) {
	auto *sizer = new wxBoxSizer(wxVERTICAL);
	
	auto *loginBtn = new wxButton(this, wxID_ANY, "Login", wxDefaultPosition, wxSize(definedVar::BUTTON_WIDTH, definedVar::BUTTON_HEIGHT));
	auto * registerBtn = new wxButton(this, wxID_ANY, "register", wxDefaultPosition, wxSize(definedVar::BUTTON_WIDTH, definedVar::BUTTON_HEIGHT));
	
	sizer->AddStretchSpacer();
	sizer->Add(loginBtn, 0, wxALIGN_CENTER | wxALL, definedVar::BORDER_WIDTH);
	sizer->Add(registerBtn, 0, wxALIGN_CENTER | wxALL, definedVar::BORDER_WIDTH);
	sizer->AddStretchSpacer();

	SetSizer(sizer);

	loginBtn->Bind(wxEVT_BUTTON, &WelcomePanel::OnLoginClick, this);	
	registerBtn->Bind(wxEVT_BUTTON, &WelcomePanel::OnRegisterClick, this);
}	

void WelcomePanel::OnLoginClick(wxCommandEvent & /*event*/){
	mainFrame_->ShowLogin();
}

void WelcomePanel::OnRegisterClick(wxCommandEvent & /*event*/){
	mainFrame_->ShowRegister();
}


