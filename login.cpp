#include "userRegistry.hpp"
#include "validator.hpp"
#include <wx/artprov.h>
#include <wx/statline.h>
#include <wx/wx.h>

namespace definedVar {
constexpr int BORDER_WIDTH = 5;
constexpr int BORDER_WIDTH_ = 15;
constexpr int BUTTON_WIDTH = 100;
constexpr int BUTTON_HEIGHT = 30;
constexpr int TEXT_WRAP = 500;
constexpr int ErrorBtn_BORDER_WIDTH = 15;
constexpr int pixel_x = 400;
constexpr int pixel_y = 300;
constexpr int mainSizer_BORDER_WIDTH_ = 10;
constexpr int BtnRow_WIDTH = 10;
} // namespace definedVar

// unordered map for the user registry
// userRegistry registry;
			
// LoginFrame the actual window the user sees
class MainFrame;

class WelcomePanel : public wxPanel {
 
public:
	WelcomePanel(wxWindow *parent, MainFrame *mainFrame);	

private:
	
	MainFrame *MainFrame_;
	void OnLoginClick(wxCommandEvent & /*event*/);
	void OnRegisterClick(wxCommandEvent & /*event*/);
};

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

class RegisterPanel : public wxPanel {
public: 
	RegisterPanel(wxWindow *parent, MainFrame *mainFrame);
private:
	MainFrame *mainFrame_;
	wxTextCtrl *userInput_;
	wxTextCtrl *emailInput_;
	wxTextCtrl *passInput_;
	void OnRegisterClick(wxCommandEvent & /*event*/);	
	void OnBackClick(wxCommandEvent & /*event*/);
	
};

// MainFrame - owns all three panels, switches which size is visible.
class MainFrame : public wxFrame {
public: 
	MainFrame() : wxFrame(nullptr, wxID_ANY, "Account System", wxDefaultPosition, wxSize(definedVar::pixel_x, definedVar::pixel_y)) {

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
	
	void ShowWelcome() {
		welcomePanel_->Show();
		loginPanel_->Hide();
		registerPanel_->Hide();
		Layout();
	}

		
	void ShowLogin() {
		welcomePanel_->Hide();
		loginPanel_->Show();
		registerPanel_->Hide();
		Layout();
	}
	
	void ShowRegister() {
		welcomePanel_->Hide();
		loginPanel_->Hide();
		registerPanel_->Show();
		Layout();
	}

	
		
	static userAccRegistry::userRegistry& getRegistry(){
		static userAccRegistry::userRegistry instance;
		return instance;
	}		
	
	static void ShowLongMessage(const wxString& title, int64_t style, const wxString& message) {
		
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

private:
	wxBoxSizer *sizer_;
	WelcomePanel *welcomePanel_;
	LoginPanel *loginPanel_;
	RegisterPanel *registerPanel_;
};


// WelcomePanel
WelcomePanel::WelcomePanel(wxWindow *parent, MainFrame *mainFrame) : wxPanel(parent), MainFrame_(mainFrame) {
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
	MainFrame_->ShowLogin();
}

void WelcomePanel::OnRegisterClick(wxCommandEvent & /*event*/){
	MainFrame_->ShowRegister();
}


// LoginPanel
LoginPanel::LoginPanel(wxWindow *parent, MainFrame *mainFrame) : wxPanel(parent), mainFrame_(mainFrame) {
	auto *sizer = new wxBoxSizer(wxVERTICAL);
	
	auto *userLabel = new wxStaticText(this, wxID_ANY, "Username:");
	userInput_ = new wxTextCtrl(this, wxID_ANY);
	
	auto *passLabel = new wxStaticText(this, wxID_ANY, "Password:");
	passInput_ = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);
	
	auto *loginBtn = new wxButton(this, wxID_ANY, "Login", wxDefaultPosition, wxSize(definedVar::BUTTON_WIDTH, definedVar::BUTTON_HEIGHT));
	auto *backBtn = new wxButton(this, wxID_ANY, "Back", wxDefaultPosition, wxSize(definedVar::BUTTON_WIDTH, definedVar::BUTTON_HEIGHT));

	sizer->Add(userLabel, 0, wxALL, definedVar::BORDER_WIDTH);	
	sizer->Add(userInput_, 0, wxALL | wxEXPAND, definedVar::BORDER_WIDTH);		
	sizer->Add(passLabel, 0, wxALL, definedVar::BORDER_WIDTH);
	sizer->Add(passInput_, 0, wxALL | wxEXPAND, definedVar::BORDER_WIDTH);	
	sizer->Add(loginBtn, 0, wxALIGN_CENTER | wxALL, definedVar::BORDER_WIDTH);		
	sizer->Add(backBtn, 0, wxALIGN_CENTER | wxALL, definedVar::BORDER_WIDTH);

	SetSizer(sizer);

	loginBtn->Bind(wxEVT_BUTTON, &LoginPanel::OnLoginClick, this);
	backBtn->Bind(wxEVT_BUTTON, &LoginPanel::OnBackClick, this);
	
}

void LoginPanel::OnLoginClick(wxCommandEvent & /*event*/) {
	const wxString user = userInput_->GetValue();
	const wxString pass = passInput_->GetValue();


	// wire in login logic here.
        // const bool success = validateLogin(validateLogin.user.ToStdString(), pass.ToStdString());
	// test value
	const bool success = true;
	if (success) {
		MainFrame::ShowLongMessage("Login successful.", wxOK | wxICON_INFORMATION, "success");
	} else {
		MainFrame::ShowLongMessage("Login failed.", wxOK | wxICON_ERROR, "failure");
	} 
}

void LoginPanel::OnBackClick(wxCommandEvent & /*event*/){
	mainFrame_->ShowWelcome();
}

RegisterPanel::RegisterPanel(wxWindow *parent, MainFrame *mainFrame) : wxPanel(parent), mainFrame_(mainFrame) {
	auto *sizer = new wxBoxSizer(wxVERTICAL);

	// Username row
	auto *userLabel = new wxStaticText(this, wxID_ANY, "Username:");
	userInput_ = new wxTextCtrl(this, wxID_ANY);
	
	// Email row
	auto *emailLabel = new wxStaticText(this, wxID_ANY, "Email:");
	emailInput_ = new wxTextCtrl(this, wxID_ANY);
	
	// Password row
	auto *passLabel = new wxStaticText(this, wxID_ANY, "Password:");
	
	// password is masked with dots/asterisks
	passInput_ = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);
	
	auto *registerBtn = new wxButton(this, wxID_ANY, "Register", wxDefaultPosition, wxSize(definedVar::BUTTON_WIDTH, definedVar::BUTTON_HEIGHT));
	auto *backBtn = new wxButton(this, wxID_ANY, "Back", wxDefaultPosition, wxSize(definedVar::BUTTON_WIDTH, definedVar::BUTTON_HEIGHT));

		
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
	sizer->Add(userLabel, 0, wxALL, definedVar::BORDER_WIDTH);
	sizer->Add(userInput_, 0, wxALL | wxEXPAND, definedVar::BORDER_WIDTH);	
	sizer->Add(emailLabel, 0, wxALL, definedVar::BORDER_WIDTH);
	sizer->Add(emailInput_, 0, wxALL | wxEXPAND, definedVar::BORDER_WIDTH);	
	sizer->Add(passLabel, 0, wxALL, definedVar::BORDER_WIDTH);
	sizer->Add(passInput_, 0, wxALL | wxEXPAND, definedVar::BORDER_WIDTH);	
	sizer->Add(registerBtn, 0, wxALL | wxALIGN_CENTER, definedVar::BORDER_WIDTH);
	sizer->Add(backBtn, 0, wxALIGN_CENTER | wxALL, definedVar::BORDER_WIDTH);

	SetSizer(sizer);
	
	// Connects the button's click event to the handler function below.
	registerBtn->Bind(wxEVT_BUTTON, &RegisterPanel::OnRegisterClick, this);
	backBtn->Bind(wxEVT_BUTTON, &RegisterPanel::OnBackClick, this);
}	

void RegisterPanel::OnRegisterClick(wxCommandEvent & /*event*/) {
	const wxString user = userInput_->GetValue();
	const wxString email = emailInput_->GetValue();
	const wxString pass = passInput_->GetValue();
	
	const bool validUserInfo =  validator::InitAccountValidator (user.ToStdString(), email.ToStdString(), pass.ToStdString());
	bool success = false;

	const userSettings::userAccount* NewUserAccount = MainFrame::getRegistry().create(user.ToStdString(), email.ToStdString(), pass.ToStdString());
	if (validUserInfo  && (NewUserAccount != nullptr)) {
	success = true;
	}
	
	if (success) {
		MainFrame::ShowLongMessage("Registration successful.", wxOK | wxICON_INFORMATION,"success");
	} else {
		MainFrame::ShowLongMessage("Registration failed.", wxOK | wxICON_ERROR,
			     "Username must be 3 to 30 characters long."
			     "It can contain letters, numbers, underscores '_', dots'.', and hyphens'-'."
			     "It must begin with a letter or number."
			     "It must end with a letter or number." 
			     "The email must be a valid email address in the format: name@example.com."
			     "The part before the '@' may contain letters, numbers, and the symbol '.', '_' '%', '+', '-'."
			     "The part after the @ must include a domain and an ending such as .com, .org, or .co.uk (atleast 2 letters)."
			     "The password must be 12 to 128 characters long."
			     "Any character is allowed.");
	}


}

void RegisterPanel::OnBackClick(wxCommandEvent & /*event*/) {
	mainFrame_->ShowWelcome();
}

/*
MyApp - the application object
*/

class MyApp : public wxApp {
public:
	bool OnInit() override {
		auto *frame = new MainFrame();
		frame->Show(true);
		return true;
	}
};

wxIMPLEMENT_APP(MyApp);
