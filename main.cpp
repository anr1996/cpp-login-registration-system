#include "mainFrame.hpp"

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
