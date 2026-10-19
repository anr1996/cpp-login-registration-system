#pragma once

#include <wx/wx.h>
#include <wx/datectrl.h>

class MainFrame;

class RegisterPanel : public wxPanel {
public:
	RegisterPanel(wxWindow *parent, MainFrame *mainFrame);

private:
	MainFrame *mainFrame_;
	wxTextCtrl *userInput_;
	wxTextCtrl *emailInput_;
	wxTextCtrl *passInput_;
	wxTextCtrl *confirmPassInput_;
	wxTextCtrl *firstNameInput_;
	wxTextCtrl *lastNameInput_;
	wxDatePickerCtrl *DOBInput_;
	wxTextCtrl *phoneInput_;
	wxChoice *genderInput_;
	wxTextCtrl *locationInput_;
	wxListBox *locationSuggestions_;
	wxDateTime latestAllowed_;

	void OnRegisterClick(wxCommandEvent & /*event*/);
	void OnBackClick(wxCommandEvent & /*event*/);
	void OnLocationText(wxCommandEvent & /*event*/);
	void OnSuggestionSelected(wxCommandEvent & /*event*/);	
	void HideSuggestions();
	void ResizeCard();

};


