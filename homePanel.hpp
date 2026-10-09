#pragma once

#include <wx/wx.h>

class MainFrame;

/*
The home screen after a successful login. A small top bar holds the app name
and a log out button; a welcome message below greets the current user by name.
*/
class HomePanel : public wxPanel {
public:
	HomePanel(wxWindow *parent, MainFrame *mainFrame);

	/*
	Called by MainFrame right after login, once a new current user is set,
	so this panel can update ites welcome text to match.
	*/
	void RefreshForCurrentUser();

private:
	MainFrame *mainFrame_;
	wxStaticText *welcomeLabel_;
	wxTextCtrl *postBox_;
	wxStaticText *statusLabel_;

	void OnPostClick(wxCommandEvent & /*event*/);
	void OnLogoutClick(wxCommandEvent & /*event*/);
};
