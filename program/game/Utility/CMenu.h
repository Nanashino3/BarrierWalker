#ifndef UTILITY_CMENU_H_
#define UTILITY_CMENU_H_

#include <vector>
#include <string>

namespace Utility
{
	void SelectReset();
	std::tuple<bool, std::string> MenuSelect(std::vector<std::string>);
	void MenuDraw(std::vector<std::string> text,
				  int pos_x, int pos_y,
				  int offset_x, int offset_y,
				  int text_color = -1, int cursor_color = -1);
} // namespace Utility

#endif // #ifndef UTILITY_CMENU_H_