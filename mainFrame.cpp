#include "mainFrame.hpp"

MainFrame::MainFrame() : wxFrame(nullptr, wxID_ANY, "Account System", wxDefaultPosition, wxSize(definedVar::pixel_x, definedVar::pixel_y)) {

	welcomePanel_ = new WelcomePanel(this, this);
	loginPanel_ = new LoginPanel(this, this);
	registerPanel_ = new RegisterPanel(this, this);
	sizer_ = new wxBoxSizer(wxVERTICAL);
	sizer_->Add(welcomePanel_, 1, wxEXPAND);
	sizer_->Add(loginPanel_, 1, wxEXPAND);	
	sizer_->Add(registerPanel_, 1, wxEXPAND);
	SetSizer(sizer_);
	
	ShowWelcome(); // Start on the welcome screen.
}

void MainFrame::ShowWelcome() {
	welcomePanel_->Show();
	loginPanel_->Hide();
	registerPanel_->Hide();
	Layout();
}

	
void MainFrame::ShowLogin() {
	welcomePanel_->Hide();
	loginPanel_->Show();
	registerPanel_->Hide();
	Layout();
}

void MainFrame::ShowRegister() {
	welcomePanel_->Hide();
	loginPanel_->Hide();
	registerPanel_->Show();
	Layout();
}


	
userAccRegistry::userRepository& MainFrame::getRegistry() {
	static const std::unique_ptr<userAccRegistry::userRepository> instance = userAccRegistry::makePostgresUserRepository();
	return *instance;
}		


void MainFrame::ShowLongMessage(const wxString& title, int64_t style, const wxString& message) {

	style = wxOK | wxICON_ERROR;

	wxDialog dlg(nullptr, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);

	// Top-level vertical sizer (stacks: content row, then line, then buttons).
	auto* mainSizer = new wxBoxSizer(wxVERTICAL);

	// Content row: icon and text side by side.			
	auto* contentSizer = new wxBoxSizer(wxHORIZONTAL);

	// Pick the correct art ID based on the style flag
	wxArtID artId;
	if ((style & wxICON_ERROR) != 0) {artId = wxART_ERROR;}			
	else if ((style & wxICON_WARNING) != 0) {artId = wxART_WARNING;}
	else if ((style & wxICON_INFORMATION) != 0) {artId = wxART_INFORMATION;}
	else if ((style & wxICON_QUESTION) != 0) {artId = wxART_QUESTION;}
		
	// If an icon was requested, fetch it and then add it.
	if(!artId.empty()){
		auto bmp = wxArtProvider::GetBitmap(artId, wxART_MESSAGE_BOX);
		contentSizer->Add(new wxStaticBitmap(&dlg, wxID_ANY, bmp), 0, wxALL | wxALIGN_TOP, definedVar::BORDER_WIDTH_);
	}

	// Wrap test at ~500px wide
	auto* text = new wxStaticText(&dlg, wxID_ANY, message);
	text->Wrap(definedVar::TEXT_WRAP);
	contentSizer->Add(text, 1, wxALL | wxALIGN_CENTER_VERTICAL, definedVar::ErrorBtn_BORDER_WIDTH);

	mainSizer->Add(contentSizer, 1, wxEXPAND);

	// Separate line.
	auto* line = new wxStaticLine(&dlg);
	mainSizer->Add(line, 0, wxEXPAND | wxLEFT | wxRIGHT, definedVar::mainSizer_BORDER_WIDTH_);

	// Button row (right-aligned OK button
	auto* btnSizer = new wxBoxSizer(wxHORIZONTAL);
	auto* okBtn = new wxButton(&dlg, wxID_OK, "OK");				
	btnSizer->AddStretchSpacer();
	btnSizer->Add(okBtn, 0, wxALL, definedVar::BtnRow_WIDTH);
	mainSizer->Add(btnSizer, 0, wxEXPAND);

	dlg.SetSizerAndFit(mainSizer);
	dlg.Centre();
	dlg.ShowModal();
}
