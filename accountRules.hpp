#pragma once

namespace accountRules {
 /*
Business rules for accounts. This header must not include wxWidgets,
so the validator stays testable without a GUI library.
*/	

inline constexpr int MIN_AGE = 18;
} // namespace accountRules
