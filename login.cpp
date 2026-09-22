#include "validator.hpp"
#include <wx/wx.h>
namespace {
constexpr int BORDER_WIDTH = 5;
} // namespace
			
bool InitAccountValidator (const wxString& user_, const wxString& email_, const wxString& pass_) {
	if (!(validator::isValidUsername(user_.ToStdString()))) {return false;}  	
	if (!(validator::isValidEmail(email_.ToStdString()))) {return false;}  	
	if (!(validator::isValidPassword(pass_.ToStdString()))) {return false;}
	return true;
}

// LoginFrame the actual window the user sees
class RegisterFrame : public wxFrame {
public: 
	RegisterFrame() : wxFrame(nullptr, wxID_ANY, "Register") {
		
		// wx panel to house widgets directly
		auto *panel = new wxPanel(this);
		
		// a sizer arranging the widgets automatically (vertical stack).
		auto *sizer = new wxBoxSizer(wxVERTICAL);

		// Username row
		auto *userLabel = new wxStaticText(panel, wxID_ANY, "Username:");
		userInput = new wxTextCtrl(panel, wxID_ANY);
		
		// Email row
		auto *emailLabel = new wxStaticText(panel, wxID_ANY, "Email:");
		emailInput = new wxTextCtrl(panel, wxID_ANY);
		
		// Password row
		auto *passLabel = new wxStaticText(panel, wxID_ANY, "Password:");
		
		// password is masked with dots/asterisks
		passInput = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);
		
		// Login Button
		auto *RegisterBtn = new wxButton(panel, wxID_ANY, "Register");
		
		/*
		Each widget is added to the sizer in the order from top to bottom.
		Arguments: 
			- widget
			- proportion
			- flags
			- border-in-pixels
		wxALL = add border on all 4 sides
		wxEXPAND = let it stretch to fill available width 
		*/
		sizer->Add(userLabel, 0, wxALL, BORDER_WIDTH);
		sizer->Add(userInput, 0, wxALL | wxEXPAND, BORDER_WIDTH);	
		sizer->Add(emailLabel, 0, wxALL, BORDER_WIDTH);
		sizer->Add(emailInput, 0, wxALL | wxEXPAND, BORDER_WIDTH);	
		sizer->Add(passLabel, 0, wxALL, BORDER_WIDTH);
		sizer->Add(passInput, 0, wxALL | wxEXPAND, BORDER_WIDTH);	
		sizer->Add(RegisterBtn, 0, wxALL | wxALIGN_CENTER, BORDER_WIDTH);

		// Tell the panel to actually use this sizer for the layout.
		panel->SetSizer(sizer);
		
		// Connects the button's click event to the handler function below.
		RegisterBtn->Bind(wxEVT_BUTTON, &RegisterFrame::OnRegisterClick, this);
	}

	private:	
		wxTextCtrl *userInput; // Member variables - the handler needs them to read.
		wxTextCtrl *emailInput; // Member variables - the handler needs them to read.
		wxTextCtrl *passInput; // These are needed later, after the constructor has returned.
		
			
		void OnRegisterClick(wxCommandEvent & /*event*/) {
			const wxString user = userInput->GetValue();
			const wxString email = emailInput->GetValue();
			const wxString pass = passInput->GetValue();
			
			const bool success =  InitAccountValidator (user.ToStdString(), email.ToStdString(), pass.ToStdString());

			if (success) {
				wxMessageBox("Registration successful!", "Success", wxOK | wxICON_INFORMATION);
			} else {
				 wxMessageBox("Registration failed, please try again.", "Error", wxOK | wxICON_ERROR);
				
			}

		}

		
};

/*
MyApp - the application object
*/

class MyApp : public wxApp {
public:
	bool OnInit() override {
		auto *frame = new RegisterFrame();
		frame->Show(true);
		return true;
	}
};

wxIMPLEMENT_APP(MyApp);
