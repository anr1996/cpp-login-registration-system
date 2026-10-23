#pragma once

#include <cstddef>
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

inline const wxColour& GetFeedColor() {
	static const wxColour panelColor(220, 205, 174);
	return panelColor;
}

inline constexpr int BORDER_WIDTH = 5;
inline constexpr int BORDER_WIDTH_ = 15;
inline constexpr int BUTTON_WIDTH = 100;
inline constexpr int BUTTON_HEIGHT = 30;
inline constexpr int TEXT_WRAP = 500;
inline constexpr int ErrorBtn_BORDER_WIDTH = 15;
inline constexpr int pixel_x = 400;
inline constexpr int pixel_y = 300;
inline constexpr int mainSizer_BORDER_WIDTH_ = 10;
inline constexpr int BtnRow_WIDTH = 10;
inline constexpr std::size_t MIN_LOCATION_CHARS = 3; // typed characters before suggestions start
inline constexpr int MAX_SUGGESTIONS = 10; // cap on rows fetched per keystroke
inline constexpr int SUGGESTION_BOX_HEIGHT = 120;
inline constexpr int MAX_FEED_POSTS = 50; // newest posts shown on the home screen
inline constexpr int FEED_WRAP = 460; // pixel width at which post text wraps
} // namespace definedVar


