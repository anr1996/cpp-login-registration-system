#pragma once

#include <wx/wx.h>

class MainFrame;

class LoginPanel : public wxPanel {
public:
	LoginPanel(wxWindow *parent, MainFrame *mainFrame);

private:
	MainFrame *mainFrame_;
	wxTextCtrl *userInput_;
	wxTextCtrl *passInput_;
	void OnLoginClick(wxCommandEvent & /*event*/);
	void OnBackClick(wxCommandEvent & /*event*/);
};
