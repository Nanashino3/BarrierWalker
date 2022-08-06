//****************************************************************************
// ファイル名：CHelpScene(ヘルプ画面)
// 作　成　日：2022/08/05
#include "CHelpScene.h"
#include "CPauseScene.h"
#include "CSecondaryController.h"

#include "../../CGameDirector.h"
#include "../../ShareInfo/CDocGameInfo.h"

namespace Sequence
{
namespace PlayGame
{
CHelpScene::CHelpScene()
: AMenuScene("resource/sceneInfo/HelpSceneInfo.csv")
, m_msgPosX(0), m_msgPosY(0)
, m_msgAlignmentPosX(0), m_msgAlignmentPosY(0)
, m_selectMenu(-1)
{
	m_helpMsgDatas = tnl::LoadCsv("resource/sceneInfo/message/HelpSceneMessage.csv");

	// メッセージリスト
	std::vector<std::string> commandList = m_helpMsgDatas[0];
	std::vector<std::string> operationList = m_helpMsgDatas[1];
	if(commandList.size() == operationList.size()){
		for (int i = 1; i < commandList.size(); ++i) {
			std::string command = commandList[i].c_str();
			std::string operation = operationList[i].c_str();
			m_helpList.push_back(std::make_pair(command, operation));
		}
	}else{
		tnl::DebugTrace("CHelpScene->Error List size\n");
	}

	m_msgPosX = atoi(m_helpMsgDatas[2][1].c_str());
	m_msgPosY = atoi(m_helpMsgDatas[2][2].c_str());
	m_msgAlignmentPosX = atoi(m_helpMsgDatas[3][1].c_str());
	m_msgAlignmentPosY = atoi(m_helpMsgDatas[3][2].c_str());
}

CHelpScene::~CHelpScene()
{}

//****************************************************************************
// 関数名：Update
// 概　要：画面更新
// 引　数：第1引数	一次管理者
//		   第2引数	ゲーム情報
// 戻り値：なし
// 詳　細：各種画面で必要な処理を行う
//****************************************************************************
IScene* CHelpScene::Update(CSecondaryController& controller, ShareInfo::CDocGameInfo& info)
{
	IScene* nextScene = this;

	// ゲーム画面でヘルプ画面を表示
	if(info.GetPrevScene() == PAUSE_SCENE){
		controller.GetGameInstance()->Draw();
	}

	int counter = 0;
	for(auto& [command, operation] : m_helpList){
		DrawStringEx(m_msgPosX,						 m_msgPosY + counter * m_alignmentY, GetColor(255, 255, 0), "%s", command.c_str());
		DrawStringEx(m_msgPosX + 75,				 m_msgPosY + counter * m_alignmentY, GetColor(255, 255, 0), "%s", "：");
		DrawStringEx(m_msgPosX + m_msgAlignmentPosX, m_msgPosY + counter * m_alignmentY, GetColor(255, 255, 0), "%s", operation.c_str());
		counter++;
	}

	// メニュー選択
	MenuSelect(m_selectMenu);
	if(m_selectMenu < 0){ return nextScene; }

	// 選択したメニュー毎の処理
	if(m_selectMenu == 0){
		if(info.GetPrevScene() == PAUSE_SCENE){
			// ポーズ画面に戻る
			nextScene = new CPauseScene();
		}else{
			// タイトル画面に戻る
			controller.MoveToScene(CSecondaryController::PRIMARY_ID_TITLE);
		}
	}else{
		tnl::DebugTrace("CHelpScene->Error No Text\n");
	}
	info.SetPrevScene(HELP_SCENE);

	return nextScene;
}

} // namespace PlayGame

} // namespace Sequence