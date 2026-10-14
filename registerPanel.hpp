#pragma once

#include <wx/wx.h>

class MainFrame;

class RegisterPanel : public wxPanel {
public:
	RegisterPanel(wxWindow *parent, MainFrame *mainFrame);

private:
	MainFrame *mainFrame_;
	wxTextCtrl *userInput_;
	wxTextCtrl *emailInput_;
	wxTextCtrl *passInput_;
	wxTextCtrl *firstNameInput_;
	wxTextCtrl *lastNameInput_;
	wxTextCtrl *DOBInput_;
	wxTextCtrl *phoneInput_;
	wxTextCtrl *genderInput_;
	wxTextCtrl *locationInput_;



	void OnRegisterClick(wxCommandEvent & /*event*/);
	void OnBackClick(wxCommandEvent & /*event*/);
};


