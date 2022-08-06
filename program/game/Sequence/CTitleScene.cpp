//****************************************************************************
// ファイル名：CTitleScene(タイトル画面)
// 作　成　日：2022/05/25
#include "CTitleScene.h"
#include "PlayGame/CSecondaryController.h"

#include "../ShareInfo/CDocGameInfo.h"
#include "../Sound/CSoundManager.h"
#include "../Utility/CImageManager.h"
#include "../Utility/CScreenEffect.h"


namespace Sequence
{
CTitleScene::CTitleScene()
: AMenuScene("resource/sceneInfo/TitleSceneInfo.csv")
, m_bgGfxHdl(0)
, m_titleGfxHdl(0)
, m_alphaValue(0)
, m_elapsed(0.0f)
, m_selectMenu(-1)
{
	// 背景画像の読み込み
	m_bgGfxHdl = Utility::CImageManager::GetInstance()->LoadGraphEx("resource/pipo-bg003a.jpg");
	m_soundManager = Sound::CSoundManager::GetInstance();
	m_titleGfxHdl = Utility::CImageManager::GetInstance()->LoadGraphEx("resource/string/title1.png");
}

CTitleScene::~CTitleScene()
{
	Utility::CImageManager::GetInstance()->DeleteGraphEx(m_bgGfxHdl);
	Utility::CImageManager::GetInstance()->DeleteGraphEx(m_titleGfxHdl);
}

//****************************************************************************
// 関数名：Update
// 概　要：画面更新
// 引　数：第1引数	一次管理者
//		   第2引数	ゲーム情報
// 戻り値：なし
// 詳　細：各種画面で必要な処理を行う
//****************************************************************************
IScene* CTitleScene::Update(const CPrimaryController&, ShareInfo::CDocGameInfo& info)
{
	IScene* nextScene = this;

	// 背景画像表示
	DrawRotaGraph(512, 384, 1.6f, 0, m_bgGfxHdl, true);

	// BGMがなっていないときだけ再生
	if (!m_soundManager->IsPlayBGM(BGM_ID_TITLE)) { m_soundManager->PlayBGM(BGM_ID_TITLE); }
	// タイトル画面のアニメーション
	if(PriTitleAnimation(info)){ return nextScene; }
	
	// メニュー選択処理
	MenuSelect(m_selectMenu);
	if(m_selectMenu < 0){ return nextScene; }

	// 選択したメニュー毎の処理
	if(m_selectMenu == 0){
		if(!Utility::CScreenEffect::GetInstance()->FadeOut()){ return nextScene; }
		nextScene = new PlayGame::CSecondaryController(PlayGame::CSecondaryController::SECONDARY_ID_LOADING);
		m_soundManager->StopBGM(BGM_ID_TITLE);
	}else if(m_selectMenu == 1){
		nextScene = new PlayGame::CSecondaryController(PlayGame::CSecondaryController::SECONDARY_ID_HELP);
	}else if(m_selectMenu == 2){
		return nullptr;
	}else{
		tnl::DebugTrace("CTitleScene->Error No Text\n");
	}
	info.SetPrevScene(TITLE_SCENE);

	return nextScene;
}

//****************************************************************************
// 関数名：PriTitleAnimation
// 概　要：タイトル画面のアニメーション(Private)
// 引　数：第1引数	ゲーム情報
// 戻り値：なし
// 詳　細：タイトル画面のアニメーション処理
//****************************************************************************
bool CTitleScene::PriTitleAnimation(ShareInfo::CDocGameInfo& info)
{
	m_elapsed += info.GetDeltaTime();
	if(m_elapsed > 0.05f){
		m_elapsed = 0.0f;
		if(m_alphaValue != 255){
			m_alphaValue += 5;
		}else{
			m_alphaValue = 255;
		}
	}
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_alphaValue);
	DrawRotaGraph(512, 300, 1.0f, 0, m_titleGfxHdl, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	return (m_alphaValue != 255) ? true: false;
}

} // namespace Sequence