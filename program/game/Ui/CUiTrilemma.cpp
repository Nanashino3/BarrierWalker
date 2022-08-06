//****************************************************************************
// ファイル名：CUiTrilemma(三すくみUI)
// 作　成　日：2022/08/06
#include <string>
#include "CUiTrilemma.h"
#include "../Utility/CImageManager.h"
#include "../ShareInfo/CONST_GAME_VALUE.h"

namespace Ui
{
CUiTrilemma::CUiTrilemma()
{
	// 三すくみ画像登録
	std::vector<std::string> fileList = {
		"resource/sticon1b-3.png",	// 赤
		"resource/sticon1d-3.png",	// 緑
		"resource/sticon1c-3.png"	// 青
	};
	for(int i = 0; i < fileList.size(); i++){
		int hdl = Utility::CImageManager::GetInstance()->LoadGraphEx(fileList[i].c_str());
		m_attributeGfxHdl.push_back(hdl);
	}
	// アイコン画像を登録
	LoadDivGraph("resource/ui/pipo-emotion.png",
				 MAX_ICON_IMAGE_NUM,
				 ICON_IMAGE_NUMX, ICON_IMAGE_NUMY,
				 ICON_RECT_SIZE, ICON_RECT_SIZE,
				 m_iconGfxHdl);
}

CUiTrilemma::~CUiTrilemma()
{
	for(int i = 0; i < m_attributeGfxHdl.size(); i++){
		Utility::CImageManager::GetInstance()->DeleteGraphEx(m_attributeGfxHdl[i]);
	}
	for(int i = 0; i < MAX_ICON_IMAGE_NUM; i++){
		DeleteGraph(m_iconGfxHdl[i]);
	}
}

//****************************************************************************
// 関数名：Draw
// 概　要：描画処理
// 引　数：第1引数　ゲーム情報
// 戻り値：なし
// 詳　細：三すくみUI描画
//****************************************************************************
void CUiTrilemma::Draw(ShareInfo::CDocGameInfo&)
{
	DrawRotaGraph(350, 120, TRILEMMA_EXRATE, 0, m_attributeGfxHdl[ATTRIBUTE_TYPE_RED],   true); // 赤
	DrawRotaGraph(380,  80, TRILEMMA_EXRATE, 0, m_attributeGfxHdl[ATTRIBUTE_TYPE_GREEN], true); // 緑
	DrawRotaGraph(410, 120, TRILEMMA_EXRATE, 0, m_attributeGfxHdl[ATTRIBUTE_TYPE_BLUE],  true); // 青

	DrawRotaGraph(380, 120, TRILEMMA_EXRATE, 0, m_iconGfxHdl[TRILEMMA_ARROW_LEFT], true);
	DrawRotaGraph(360, 100, TRILEMMA_EXRATE, 0, m_iconGfxHdl[TRILEMMA_ARROW_RIGHT_UP], true);
	DrawRotaGraph(395,  95, TRILEMMA_EXRATE, 0, m_iconGfxHdl[TRILEMMA_ARROW_RIGHT_DOWN], true);
}

} // namespace Ui