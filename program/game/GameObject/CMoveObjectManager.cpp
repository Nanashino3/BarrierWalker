#include "CMoveObjectManager.h"

#include "../Camera/CCamera2D.h"
#include "../Utility/CImageManager.h"
#include "../ShareInfo/CDocGameInfo.h"

namespace{
const unsigned int MAX_FLOOR_OBJECT = 3;
const unsigned int RECT_W = 64;
const unsigned int RECT_H = 24;

const int MOVE_UP     = 0;
const int MOVE_RIGHT  = 1;
const int MOVE_BOTTOM = 2;
const int MOVE_LEFT   = 3;
const std::vector<std::string> g_readFiles = {
	//"",
	//"resource/stage/move_floor_2.csv",
	""
};

std::vector<std::vector<std::string>> g_readCsv;
struct S_FLOOR_INFO{
	float posX;
	float posY;
	float prevPosX;
	float prevPosY;
	float velocity;
	bool isAppear;
	bool isRespawn;
};
struct S_FLOOR_MANAGER{
	float posX;
	float posY;

	int maxNum;
	int direction;
	int counter;
	float elapsed;
	float interval;

	std::vector<S_FLOOR_INFO> floors;
};
std::vector<S_FLOOR_MANAGER> g_floorManager;
}

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

void CMoveObjectManager::Initialize(ShareInfo::CDocGameInfo& info)
{
	// 要素の初期化とメモリサイズを合わせる
	g_readCsv.clear();
	g_readCsv.shrink_to_fit();
	for(int i = 0; i < g_floorManager.size(); i++){
		g_floorManager[i].floors.clear();
		g_floorManager[i].floors.shrink_to_fit();
	}
	g_floorManager.clear();
	g_floorManager.shrink_to_fit();

	// ステージ番号から読み込むCSVを特定
	int index = info.GetStageIndex();
	if(strncmp(g_readFiles[index].c_str(), "", sizeof(g_readFiles[index].c_str())) != 0){
		g_readCsv = tnl::LoadCsv(g_readFiles[index].c_str());
	}

	for(int i = 0; i < g_readCsv.size(); i++){
		S_FLOOR_MANAGER floorManager;
		floorManager.posX = atof(g_readCsv[i][0].c_str()) - (info.GetScreenWidth() >> 1);
		floorManager.posY = atof(g_readCsv[i][1].c_str()) - (info.GetScreenHeight() >> 1);
		floorManager.maxNum = atoi(g_readCsv[i][2].c_str());
		floorManager.direction = atoi(g_readCsv[i][3].c_str());
		floorManager.interval = atof(g_readCsv[i][4].c_str());
		floorManager.elapsed = 0.0f;
		floorManager.counter = 0;

		for(int count = 0; count < floorManager.maxNum; count++){
			S_FLOOR_INFO floor;
			floor.posX		= floorManager.posX;
			floor.posY		= floorManager.posY;
			floor.prevPosX	= floor.posX;
			floor.prevPosY	= floor.posY;
			floor.velocity	= atof(g_readCsv[i][5].c_str());
			floor.isAppear	= false;
			floor.isRespawn = false;

			floorManager.floors.push_back(floor);
		}
		g_floorManager.push_back(floorManager);
	}
}

void CMoveObjectManager::Move(ShareInfo::CDocGameInfo& info)
{
	tnl::Vector3 camPos = info.GetCamera()->GetPosition();
	int screenHalfW = info.GetScreenWidth() >> 1;
	int screenHalfH = info.GetScreenHeight() >> 1;

	for(int i = 0; i < g_floorManager.size(); i++){
		// 移動床数管理
		if(g_floorManager[i].counter == g_floorManager[i].maxNum){
			g_floorManager[i].counter = 0;
		}

		// 床移動制御(インターバル)
		g_floorManager[i].elapsed += info.GetDeltaTime();
		if(g_floorManager[i].elapsed > g_floorManager[i].interval){
			g_floorManager[i].elapsed = 0.0f;

			bool isAppear = g_floorManager[i].floors[g_floorManager[i].counter].isAppear;
			bool isRespawn = g_floorManager[i].floors[g_floorManager[i].counter].isRespawn;
			if(!isAppear && !isRespawn){
				g_floorManager[i].floors[g_floorManager[i].counter].isAppear = true;

				g_floorManager[i].counter++;
			}
		}

		// 移動管理
		for(int count = 0; count < g_floorManager[i].floors.size(); count++){
			if(!g_floorManager[i].floors[count].isAppear){ continue; }

			bool isInit = false;
			int dx = 0, dy = 0, distance = 0;

			// 移動する前の座標を記憶
			g_floorManager[i].floors[count].prevPosX = g_floorManager[i].floors[count].posX;
			g_floorManager[i].floors[count].prevPosY = g_floorManager[i].floors[count].posY;
			switch(g_floorManager[i].direction)
			{
			case MOVE_UP:
				dy = -1;
				distance = camPos.y - screenHalfH;
				if(g_floorManager[i].floors[count].posY < distance){ isInit = !isInit; }
				break;
			case MOVE_RIGHT:
				dx =  1;
				break;
			case MOVE_BOTTOM:
				dy =  1;
				distance = camPos.y + screenHalfH;
				if(g_floorManager[i].floors[count].posY > distance){ isInit = !isInit; }
				break;
			case MOVE_LEFT:
				dx = -1;
				break;
			}
			g_floorManager[i].floors[count].posX += g_floorManager[i].floors[count].velocity * dx;
			g_floorManager[i].floors[count].posY += g_floorManager[i].floors[count].velocity * dy;

			// 床の初期化(ある地点まで行ったら元に戻す)
			if(!isInit){ continue; }
			g_floorManager[i].floors[count].posX = g_floorManager[i].posX;
			g_floorManager[i].floors[count].posY = g_floorManager[i].posY;
			g_floorManager[i].floors[count].isAppear = false;
		}
	}
}

void CMoveObjectManager::Collision(tnl::Vector3& currentPos, tnl::Vector3& prevPos, ShareInfo::CDocGameInfo& info)
{
	// TODO：下からのすり抜けは一旦諦める
	for(int i = 0; i < g_floorManager.size(); i++){
		S_FLOOR_MANAGER manager = g_floorManager[i];
		for(int count = 0; count < manager.floors.size(); count++){

			tnl::Vector3 floorPos = tnl::Vector3(manager.floors[count].posX, manager.floors[count].posY, 0);
			int correctType = tnl::IsIntersectRectToCorrectPosition(currentPos, prevPos, (PLAYER_RECT_W >> 1), PLAYER_RECT_H, floorPos, RECT_W, RECT_H);
			if(correctType == NO_HIT){ continue; }
			
			if(correctType == CORRECT_UP){
				float offsetY = (PLAYER_RECT_H >> 1) + (RECT_H >> 1);
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

void CMoveObjectManager::Draw(ShareInfo::CDocGameInfo& info)
{
	tnl::Vector3 camPos = info.GetCamera()->GetPosition();
	for(int i = 0; i < g_floorManager.size(); i++){
		S_FLOOR_MANAGER manager = g_floorManager[i];
		for(int count = 0; count < manager.floors.size(); count++){
			if(!manager.floors[count].isAppear){ continue; }
			int viewPosX = manager.floors[count].posX - camPos.x + (info.GetScreenWidth() >> 1);
			int viewPosY = manager.floors[count].posY - camPos.y + (info.GetScreenHeight() >> 1);
			DrawRotaGraph(viewPosX, viewPosY, 1.0f, 0, m_gfxHdl, true);
		}
	}
}

} // namespace GameObject