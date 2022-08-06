//****************************************************************************
// ファイル名：CMoveObjectManager(移動オブジェクト管理者)
// 作　成　日：2022/08/06
#include "CMoveObjectManager.h"
#include "CONST_GAMEOBJECT_VALUE.h"

#include <string>
#include "../Camera/CCamera2D.h"
#include "../ShareInfo/CDocGameInfo.h"
#include "../Utility/CImageManager.h"

namespace GameObject
{
CMoveObjectManager::CMoveObjectManager()
{
	m_gfxHdl = Utility::CImageManager::GetInstance()->LoadGraphEx("resource/move_floor.png");
}

CMoveObjectManager::~CMoveObjectManager()
{
	Utility::CImageManager::GetInstance()->DeleteGraphEx(m_gfxHdl);
}

//****************************************************************************
// 関数名：Initialize
// 概　要：初期化処理
// 引　数：第1引数	ゲーム情報
// 戻り値：なし
// 詳　細：
//****************************************************************************
void CMoveObjectManager::Initialize(ShareInfo::CDocGameInfo& info)
{
	// 要素の初期化とメモリサイズを合わせる
	for(int i = 0; i < m_floorManager.size(); i++){
		m_floorManager[i].floors.clear();
		m_floorManager[i].floors.shrink_to_fit();
	}
	m_floorManager.clear();
	m_floorManager.shrink_to_fit();

	// ステージ番号から読み込むCSVを特定
	int index = info.GetStageIndex();
	std::string fileList[] = {
		//"",
		//"resource/stage/move_floor_2.csv",
		""
	};
	std::vector<std::vector<std::string>> objectDatas;
	if(strncmp(fileList[index].c_str(), "", sizeof(fileList[index].c_str())) != 0){
		objectDatas = tnl::LoadCsv(fileList[index].c_str());
	}

	// 取得したデータ設定
	for(int i = 0; i < objectDatas.size(); i++){
		S_FLOOR_MANAGER floorManager;
		floorManager.posX = atof(objectDatas[i][0].c_str()) - (info.GetScreenWidth() >> 1);
		floorManager.posY = atof(objectDatas[i][1].c_str()) - (info.GetScreenHeight() >> 1);
		floorManager.maxNum = atoi(objectDatas[i][2].c_str());
		floorManager.direction = atoi(objectDatas[i][3].c_str());
		floorManager.interval = atof(objectDatas[i][4].c_str());
		floorManager.elapsed = 0.0f;
		floorManager.counter = 0;

		for(int count = 0; count < floorManager.maxNum; count++){
			S_FLOOR_INFO floor;
			floor.posX		= floorManager.posX;
			floor.posY		= floorManager.posY;
			floor.prevPosX	= floor.posX;
			floor.prevPosY	= floor.posY;
			floor.velocity	= atof(objectDatas[i][5].c_str());
			floor.isAppear	= false;
			floor.isRespawn = false;

			floorManager.floors.push_back(floor);
		}
		m_floorManager.push_back(floorManager);
	}
}

//****************************************************************************
// 関数名：Move
// 概　要：移動処理
// 引　数：第1引数	ゲーム情報
// 戻り値：なし
// 詳　細：
//****************************************************************************
void CMoveObjectManager::Move(ShareInfo::CDocGameInfo& info)
{
	tnl::Vector3 camPos = info.GetCamera()->GetPosition();
	int screenHalfW = info.GetScreenWidth() >> 1;
	int screenHalfH = info.GetScreenHeight() >> 1;

	for(int i = 0; i < m_floorManager.size(); i++){
		// 移動床数管理
		if(m_floorManager[i].counter == m_floorManager[i].maxNum){
			m_floorManager[i].counter = 0;
		}

		// 床移動制御(インターバル)
		m_floorManager[i].elapsed += info.GetDeltaTime();
		if(m_floorManager[i].elapsed > m_floorManager[i].interval){
			m_floorManager[i].elapsed = 0.0f;

			bool isAppear = m_floorManager[i].floors[m_floorManager[i].counter].isAppear;
			bool isRespawn = m_floorManager[i].floors[m_floorManager[i].counter].isRespawn;
			if(!isAppear && !isRespawn){
				m_floorManager[i].floors[m_floorManager[i].counter].isAppear = true;

				m_floorManager[i].counter++;
			}
		}

		// 移動管理
		for(int count = 0; count < m_floorManager[i].floors.size(); count++){
			if(!m_floorManager[i].floors[count].isAppear){ continue; }

			bool isInit = false;
			int dx = 0, dy = 0, distance = 0;

			// 移動する前の座標を記憶
			m_floorManager[i].floors[count].prevPosX = m_floorManager[i].floors[count].posX;
			m_floorManager[i].floors[count].prevPosY = m_floorManager[i].floors[count].posY;
			switch(m_floorManager[i].direction)
			{
			case MOVE_UP:
				dy = -1;
				distance = camPos.y - screenHalfH;
				if(m_floorManager[i].floors[count].posY < distance){ isInit = !isInit; }
				break;
			case MOVE_RIGHT:
				dx =  1;
				break;
			case MOVE_BOTTOM:
				dy =  1;
				distance = camPos.y + screenHalfH;
				if(m_floorManager[i].floors[count].posY > distance){ isInit = !isInit; }
				break;
			case MOVE_LEFT:
				dx = -1;
				break;
			}
			m_floorManager[i].floors[count].posX += m_floorManager[i].floors[count].velocity * dx;
			m_floorManager[i].floors[count].posY += m_floorManager[i].floors[count].velocity * dy;

			// 床の初期化(ある地点まで行ったら元に戻す)
			if(!isInit){ continue; }
			m_floorManager[i].floors[count].posX = m_floorManager[i].posX;
			m_floorManager[i].floors[count].posY = m_floorManager[i].posY;
			m_floorManager[i].floors[count].isAppear = false;
		}
	}
}

//****************************************************************************
// 関数名：Collision
// 概　要：衝突処理
// 引　数：第1引数	現在の位置
//		   第2引数	前回の位置
//		   第3引数	ゲーム情報
// 戻り値：なし
// 詳　細：
//****************************************************************************
void CMoveObjectManager::Collision(tnl::Vector3& currentPos, tnl::Vector3& prevPos, ShareInfo::CDocGameInfo& info)
{
	// TODO：下からのすり抜けは一旦諦める
	for(const auto& manager : m_floorManager){
		for(int count = 0; count < manager.floors.size(); count++){

			tnl::Vector3 floorPos = tnl::Vector3(manager.floors[count].posX, manager.floors[count].posY, 0);
			int correctType = tnl::IsIntersectRectToCorrectPosition(currentPos, prevPos, (PLAYER_RECT_W >> 1), PLAYER_RECT_H, floorPos, MOVE_OBJ_RECTW, MOVE_OBJ_RECTH);
			if(correctType == NO_HIT){ continue; }
			
			if(correctType == CORRECT_UP){
				float offsetY = (PLAYER_RECT_H >> 1) + (MOVE_OBJ_RECTH >> 1);
				currentPos.y = floorPos.y - offsetY;

				int dy = 0;
				switch(manager.direction)
				{
				case MOVE_UP:
					dy = -1;	break;
				case MOVE_BOTTOM:
					dy = 1;		break;
				default:
					break;
				}
				currentPos.y += manager.floors[count].velocity * dy;
			}
			info.SetIsCollision(true);
			info.SetCurrentPos(currentPos);
			info.SetCorrectType(ShareInfo::COLLISION_OBJECT_MOVE_FLOOR, correctType);
		}
	}
}

//****************************************************************************
// 関数名：Draw
// 概　要：描画処理
// 引　数：第1引数	ゲーム情報
// 戻り値：なし
// 詳　細：
//****************************************************************************
void CMoveObjectManager::Draw(ShareInfo::CDocGameInfo& info)
{
	tnl::Vector3 camPos = info.GetCamera()->GetPosition();
	for(int i = 0; i < m_floorManager.size(); i++){
		S_FLOOR_MANAGER manager = m_floorManager[i];
		for(int count = 0; count < manager.floors.size(); count++){
			if(!manager.floors[count].isAppear){ continue; }
			int viewPosX = manager.floors[count].posX - camPos.x + (info.GetScreenWidth() >> 1);
			int viewPosY = manager.floors[count].posY - camPos.y + (info.GetScreenHeight() >> 1);
			DrawRotaGraph(viewPosX, viewPosY, 1.0f, 0, m_gfxHdl, true);
		}
	}
}

} // namespace GameObject