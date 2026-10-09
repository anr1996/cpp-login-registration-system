#pragma once
#include <wx/colour.h>

namespace definedVar {

inline const wxColour& GetPanelColor() {
	static const wxColour panelColor(226, 215, 193); 
	return panelColor;
}

inline const wxColour& GetPanelColor_1() {
	static const wxColour panelColor(176,34,12); 
	return panelColor;
}


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


