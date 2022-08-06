#include "CScoreScene.h"
#include "CSecondaryController.h"

#include "../../CGameDirector.h"
#include "../../ShareInfo/CDocGameInfo.h"
#include "../../Utility/CScreenEffect.h"

namespace Sequence
{
namespace PlayGame
{
CScoreScene::CScoreScene()
: AMenuScene("resource/sceneInfo/ScoreSceneInfo.csv")
, m_elapsed(0.0f)
, m_keyCount(0)
, m_selectMenu(-1)
{
	std::vector<std::vector<std::string>> scoreMsgDatas = 
		tnl::LoadCsv("resource/sceneInfo/message/ScoreSceneMessage.csv");
	std::vector<std::string> msgList = scoreMsgDatas[0];

	for(int i = 1; i < msgList.size(); ++i){
		std::string msg = msgList[i];
		m_scoreMsgList.push_back(msg);
	}

	m_msgPosX = atoi(scoreMsgDatas[1][1].c_str());
	m_msgPosY = atoi(scoreMsgDatas[1][2].c_str());
	m_msgAlignmentPosX = atoi(scoreMsgDatas[2][1].c_str());
	m_msgAlignmentPosY = atoi(scoreMsgDatas[2][2].c_str());
}
	
CScoreScene::~CScoreScene()
{}

IScene* CScoreScene::Update(CSecondaryController& controller, ShareInfo::CDocGameInfo& info)
{
	IScene* nextScene = this;

	// ゲーム画面描画
	controller.GetGameInstance()->Draw();
	DrawStringEx(m_sceneTextPosX, m_sceneTextPosY, GetColor(255, 255, 0), m_sceneTitle.c_str());

	// スコア表示
	m_elapsed += info.GetDeltaTime();
	if(m_elapsed > 1.0f){
		m_elapsed = 0.0f;
		if(m_keyCount < MAX_ACCESS_KEY){ m_keyCount++; }
	}
	for(int count = 0; count < m_keyCount; count++){
		DrawStringEx(m_msgPosX, m_msgPosY + count * m_msgAlignmentPosY, GetColor(255, 255, 0), "%s : %d", m_scoreMsgList[count].c_str(), info.GetScoreCalcSrcInfo(count));
	}

	// 全てのスコアを表示した場合選択メニューを出す
	if(m_keyCount == MAX_ACCESS_KEY){
		MenuSelect(m_selectMenu);
		if(m_selectMenu < 0){ return nextScene; }

		// 選択したメニュー毎の処理
		if(m_selectMenu == 0){
			if(!Utility::CScreenEffect::GetInstance()->FadeOut()){ return nextScene; }
			controller.MoveToScene(CSecondaryController::PRIMARY_ID_TITLE);
		}else{
			tnl::DebugTrace("CScoreScene->Error No Text\n");
		}
	}

	return nextScene;
}

} // namespace PlayGame

} // namespace Sequence