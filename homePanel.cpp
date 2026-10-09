#include "appConstants.hpp"
#include "homePanel.hpp"
#include "mainFrame.hpp"
#include "cctype"

HomePanel::HomePanel(wxWindow *parent, MainFrame *mainFrame) : wxPanel(parent), mainFrame_(mainFrame) {
	SetBackgroundColour(definedVar::GetPanelColor());

	/*
	Top bar: a thin strip across the top holding the app name on the left
	and a Log out button on the right.
	*/
	auto *topBar = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SIMPLE);
	topBar->SetBackgroundColour(wxColour(176, 34, 12));
	
	auto *appNameLabel = new wxStaticText(topBar, wxID_ANY, "Rich Social");
	appNameLabel->SetForegroundColour(wxColour(255, 255, 255));
	appNameLabel->SetFont(wxFontInfo(14).Bold());

	auto *logoutBtn = new wxButton(topBar, wxID_ANY, "Log out");
	logoutBtn->SetBackgroundColour(wxColour(0, 0, 0));
	
	/*
	topBarSizer is HORIZONTAL: app name on the left, a stretch spacer
	pushes the logout button to the right edge.
	*/

	auto *topBarSizer = new wxBoxSizer(wxHORIZONTAL);
	topBarSizer->Add(appNameLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, definedVar::BORDER_WIDTH);
	topBarSizer->AddStretchSpacer(1);
	topBarSizer->Add(logoutBtn, 0, wxALL | wxALIGN_CENTER_VERTICAL, definedVar::BORDER_WIDTH);
	topBar->SetSizer(topBarSizer);

	/*
	WelcomeLabel_ is stored as a member so RefreshForCurrentUser() can 
	update its text later, once we know who logged in.
	*/
	
	welcomeLabel_ = new wxStaticText(this, wxID_ANY, "Welcome!");
	welcomeLabel_->SetForegroundColour(wxColour(0, 0, 0));
	welcomeLabel_->SetFont(wxFontInfo(18));

	/*
	A multi-line text box for writing a post, capped at 500 characters
	to match the database CHECK constraint.
	*/
	
	auto *postCard = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SIMPLE);
	postCard->SetBackgroundColour(wxColour(176, 34, 12));

	auto *postCardSizer = new wxBoxSizer(wxVERTICAL);
	
	postBox_ = new wxTextCtrl(postCard, wxID_ANY, "", wxDefaultPosition, wxSize(500,100), wxTE_MULTILINE);
	postBox_->SetMaxLength(500);

	/*work in progress*/
	postBox_->SetBackgroundColour(wxColour(0, 0, 0));

	auto *postBtn = new wxButton(postCard, wxID_ANY, "Post");
	postBtn->SetBackgroundColour(wxColour(0, 0, 0));
	
	/*
	Shows "Posted!" or an error under the button.
	*/
	statusLabel_ = new wxStaticText(postCard, wxID_ANY, "");
	statusLabel_->SetForegroundColour(wxColour(0, 0, 0));

	postCardSizer->Add(postBox_, 0, wxALIGN_CENTER_HORIZONTAL | wxBOTTOM, 10);
	postCardSizer->Add(postBtn, 0, wxALIGN_CENTER_HORIZONTAL | wxBOTTOM, 10);
	postCardSizer->Add(statusLabel_, 0, wxALIGN_CENTER_HORIZONTAL);	
	
	postCard->SetSizer(postCardSizer);
	postCard->Fit();
	
	/*
	outer stacks the top bar first, then the welcome message below it.
	wxEXPAND on the top bar lets it stretch to the full panel width,
	unlike the small "card" panels on the other screens.
	*/
	auto *outer = new wxBoxSizer(wxVERTICAL);
	outer->Add(topBar, 0, wxEXPAND);
	outer->Add(welcomeLabel_, 0, wxALL | wxALIGN_RIGHT, 30);
	outer->AddStretchSpacer(1);	
	outer->Add(postCard, 0, wxALIGN_CENTER_HORIZONTAL);
	outer->AddStretchSpacer(1);
	SetSizer(outer);
	
	logoutBtn->Bind(wxEVT_BUTTON, &HomePanel::OnLogoutClick, this);
	postBtn->Bind(wxEVT_BUTTON, &HomePanel::OnPostClick, this);
}

void HomePanel::RefreshForCurrentUser() {
	
	postBox_->Clear();
	statusLabel_->SetLabel("");	

	/*
	Ask MainFrame who is currently logged in. GetCurrentUser() returns
	nullptr if nobody is. (Not likey to happen here but it is an extra precaution).
	*/
	const userSettings::userAccount* user = mainFrame_->GetCurrentUser();
	if (user == nullptr) {
		welcomeLabel_->SetLabel("Welcome!");
		return;
	}
	
	/*
	get_username() returns a string_view; wxString can be built 
	directly from a std::string, so we convert explicitly here.
	*/

	std::string userUppercase = user->get_username();
	userUppercase[0] = std::toupper(userUppercase[0]);
	auto username = wxString::FromUTF8(userUppercase);
	welcomeLabel_->SetLabel(username);
	
	/*
	The label's text changed length so ask the sizer to recomputer the layout
	(otherwise the old sizing could clip or misplace the new text).
	*/
	Layout();
}

void HomePanel::OnLogoutClick(wxCommandEvent & /*event*/) {
	mainFrame_->Logout();
}

void HomePanel::OnPostClick(wxCommandEvent & /*event*/) {
	const userSettings::userAccount* user = mainFrame_->GetCurrentUser();
	if (user == nullptr) {
		return;
	}
	
	// Trim() removes whitespace on the right; Trim(false) on the left
	wxString text = postBox_->GetValue();
	text.Trim().Trim(false);

	if (text.empty()) {
		statusLabel_->SetLabel("Writing something before posting.");
		Layout();
		return;
	}
	
	try {
		const bool saved = MainFrame::getRegistry().create_post(user->get_username(), text.ToStdString());
	
		if (saved) {
			postBox_->Clear();
			statusLabel_->SetLabel("Posted!");
		} else {
			statusLabel_->SetLabel("Could not save your post.");
		}
	} catch (const std::exception &) {
		statusLabel_->SetLabel("Database error. Try again.");
	}

	Layout();
}
