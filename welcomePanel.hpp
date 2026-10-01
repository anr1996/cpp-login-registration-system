#pragma once

#include <wx/wx.h>

class MainFrame;

class WelcomePanel : public wxPanel {
public:
	WelcomePanel(wxWindow *parent, MainFrame *mainFrame);

private:
	MainFrame *mainFrame_;
	void OnLoginClick(wxCommandEvent & /*event*/);
	void OnRegisterClick(wxCommandEvent & /*event*/);
	
};
