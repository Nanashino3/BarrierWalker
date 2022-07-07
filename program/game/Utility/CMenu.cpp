#include "CMenu.h"
#include "../Sound/CSoundManager.h"
#include "../ShareInfo/CONST_GAME_VALUE.h"
#include "../../dxlib_ext/dxlib_ext.h"

namespace {
int cursor_pos = 0;
}

namespace Utility
{
void SelectReset()
{
	// 何かを選択した場合カーソル位置を初期化しておく
	cursor_pos = 0;
}

//********************
// メニュー選択
//********************
std::tuple<bool, std::string> MenuSelect(std::vector<std::string> text)
{
	if(tnl::Input::IsKeyDownTrigger(eKeys::KB_W)){
		cursor_pos = (cursor_pos + text.size() - 1) % text.size();
		Sound::CSoundManager::GetInstance()->PlaySE(SE_ID_MENU_SELECT);
	}
	else if(tnl::Input::IsKeyDownTrigger(eKeys::KB_S)){
		cursor_pos = (cursor_pos + 1) % text.size();
		Sound::CSoundManager::GetInstance()->PlaySE(SE_ID_MENU_SELECT);
	}

	// メニュー決定
	std::tuple<bool, std::string> result = std::make_tuple(false, "");
	if(tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)){
		Sound::CSoundManager::GetInstance()->PlaySE(SE_ID_MENU_DECIDE);
		result = std::make_tuple(true, text[cursor_pos]);
	}

	return result;
}

//********************
// メニュー描画
//********************
void MenuDraw(std::vector<std::string> text,
			  int pos_x, int pos_y,
			  int offset_x, int offset_y,
			  int text_color, int cursor_color)
{
	// メニュー文字描画
	for(int i = 0; i < text.size(); i++){
		DrawStringEx(pos_x, pos_y + i * offset_y, text_color, text[i].c_str());
	}
	// メニュー選択
	DrawStringEx(pos_x - offset_x, pos_y + cursor_pos * offset_y, cursor_color, "→");
}

} // namespace Utility