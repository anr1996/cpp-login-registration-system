#include "appConstants.hpp"
#include "homePanel.hpp"
#include "mainFrame.hpp"
#include <cctype>
#include <exception>
#include <iostream>
#include <string>
#include <vector>


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

	scrollBar_ = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL);
	scrollBar_->SetScrollRate(0, 10);

	/*
	WelcomeLabel_ is stored as a member so RefreshForCurrentUser() can 
	update its text later, once we know who logged in.
	*/
	
	welcomeLabel_ = new wxStaticText(scrollBar_, wxID_ANY, "Welcome!");
	welcomeLabel_->SetForegroundColour(wxColour(0, 0, 0));
	welcomeLabel_->SetFont(wxFontInfo(18));

	/*
	A multi-line text box for writing a post, capped at 500 characters
	to match the database CHECK constraint.
	*/
	
	auto *postCard = new wxPanel(scrollBar_, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SIMPLE);
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

	// A scrollable area: its contents are rebuilt by RefreshFeed().
	feed_ = new wxPanel(scrollBar_, wxID_ANY); 
	feed_->SetBackgroundColour(definedVar::GetPanelColor());
	feed_->SetMinSize(wxSize(500, -1));	
	feedSizer_ = new wxBoxSizer(wxVERTICAL);
	feed_->SetSizer(feedSizer_);


	auto* inner = new wxBoxSizer(wxVERTICAL);
	inner->Add(welcomeLabel_, 0, wxALL | wxALIGN_RIGHT, 30);
	inner->Add(postCard, 0, wxALIGN_CENTER_HORIZONTAL);
	inner->Add(feed_, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);


	scrollBar_->SetSizer(inner);

	/*
	outer stacks the top bar first, then the welcome message below it.
	wxEXPAND on the top bar lets it stretch to the full panel width,
	unlike the small "card" panels on the other screens.
	*/
	auto *outer = new wxBoxSizer(wxVERTICAL);
	outer->Add(topBar, 0, wxEXPAND);
	outer->Add(scrollBar_, 1, wxEXPAND);
	SetSizer(outer);
	
	logoutBtn->Bind(wxEVT_BUTTON, &HomePanel::OnLogoutClick, this);
	postBtn->Bind(wxEVT_BUTTON, &HomePanel::OnPostClick, this);
}

void HomePanel::RefreshFeed() {
	// Freeze stops repainting while we rebuild, so the list does not flicker.
	feed_->Freeze();
	feedSizer_->Clear(true);

	const userSettings::userAccount* user = mainFrame_->GetCurrentUser();
	if (user != nullptr) {
		
		try {
		
			const std::vector<userSettings::Post> posts = MainFrame::getRegistry().get_posts(user->get_username(), definedVar::MAX_FEED_POSTS);
			
			if (posts.empty()) {
				auto* empty = new wxStaticText(feed_, wxID_ANY, "No posts yet.");
				empty->SetForegroundColour(wxColour(0,0,0));
				feedSizer_->Add(empty, 0, wxALL, 5);
			}
			
			for (const userSettings::Post& post : posts) {
				
				auto* card = new wxPanel(feed_, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SIMPLE);				
				card->SetBackgroundColour(wxColour(192, 64, 0));	
				
				auto* userStamp = new wxPanel(card, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SIMPLE);	
				userStamp->SetBackgroundColour(wxColour(definedVar::GetPanelColor()));
				
				auto* username = new wxStaticText(userStamp, wxID_ANY, wxString::FromUTF8(user->get_username()));
				username->SetForegroundColour(wxColour(0, 0, 0));	
				auto* time = new wxStaticText(userStamp, wxID_ANY, wxString::FromUTF8(post.createdAt) + " UTC");
				time->SetForegroundColour(wxColour(0, 0, 0));
				
				auto* body = new wxStaticText(card, wxID_ANY, wxString::FromUTF8(post.body));
				body->SetForegroundColour(wxColour(0, 0, 0));
				body->Wrap(definedVar::FEED_WRAP);
			
				auto* userStampSizer = new wxBoxSizer(wxVERTICAL);	
				auto* cardSizer = new wxBoxSizer(wxVERTICAL);
				
				userStampSizer->Add(username, 0, wxALL, 2);	
				userStampSizer->Add(time, 0, wxALL, 2);	
				userStamp->SetSizer(userStampSizer);
				
				cardSizer->Add(userStamp, 0, wxLEFT | wxRIGHT | wxTOP, 10);
				cardSizer->Add(body, 0, wxALL, 10);
				card->SetSizer(cardSizer);
				
				feedSizer_->Add(card, 0, wxEXPAND | wxBOTTOM, 20);
				
								
			}
		
		} catch (const std::exception& e) {
			std::cerr << "Feed error: " << e.what() << "\n";
			auto* err = new wxStaticText(feed_, wxID_ANY, "Could not load posts.");
			err->SetForegroundColour(wxColour(0, 0, 0));
			feedSizer_->Add(err, 0, wxALL, 5);
		}	
	}
	

	feedSizer_->Layout();
	
	feed_->Fit();
	
	scrollBar_->Layout();

	scrollBar_->FitInside();
	
	feed_->Thaw();
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
	userUppercase[0] = std::toupper(static_cast<unsigned char>(userUppercase[0]));
	auto username = wxString::FromUTF8(userUppercase);
	welcomeLabel_->SetLabel(username);
	
	/*
	The label's text changed length so ask the sizer to recomputer the layout
	(otherwise the old sizing could clip or misplace the new text).
	*/
	Layout();
	
	RefreshFeed();
	scrollBar_->Scroll(0,0);
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
			RefreshFeed();
			statusLabel_->SetLabel("Posted!");
		} else {
			statusLabel_->SetLabel("Could not save your post.");
		}
	} catch (const std::exception &) {
		statusLabel_->SetLabel("Database error. Try again.");
	}

	Layout();
}
