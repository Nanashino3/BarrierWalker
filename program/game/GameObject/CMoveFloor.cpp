#include "CMoveFloor.h"

#include "../Camera/CCamera2D.h"
#include "../Utility/CImageManager.h"
#include "../ShareInfo/CDocGameInfo.h"

namespace{
const unsigned int RECT_W = 64;
const unsigned int RECT_H = 24;
}

namespace GameObject
{
CMoveFloor::CMoveFloor(int dx, int dy, float px, float py, float vx, float vy)
: m_gfxHdl(0)
, m_posX(0.0f), m_posY(0.0f)
, m_firstPosX(px), m_firstPosY(py)
, m_directionX(dx), m_directionY(dy)
, m_velocityX(vx), m_velocityY(vy)
{
	m_gfxHdl = Utility::LoadGraphEx("resource/move_floor.png");
}
	
CMoveFloor::~CMoveFloor()
{
	Utility::DeleteGraphEx(m_gfxHdl);
}

void CMoveFloor::Initialize(ShareInfo::CDocGameInfo& info)
{
	m_posX = m_firstPosX - (info.GetScreenWidth() >> 1);
	m_posY = m_firstPosY - (info.GetScreenHeight() >> 1);
}

void CMoveFloor::Move(ShareInfo::CDocGameInfo&)
{
	m_posX += m_velocityX * m_directionX;
	m_posY += m_velocityY * m_directionY;
}

void CMoveFloor::Collision(tnl::Vector3& currentPos, tnl::Vector3& prevPos, ShareInfo::CDocGameInfo& info)
{
	tnl::Vector3 myPos(m_posX, m_posY, 0);
	int correctType = tnl::IsIntersectRectToCorrectPosition(currentPos, prevPos, PLAYER_RECT_W, PLAYER_RECT_H, myPos, RECT_W, RECT_H);
	if(correctType == 3){
		info.SetIsCollision(true);
		info.SetCurrentPos(currentPos);
		info.SetCorrectType(ShareInfo::COLLISION_OBJECT_MOVE_FLOOR, correctType);
	}
}

void CMoveFloor::Update(tnl::Vector3& currentPos, tnl::Vector3&, ShareInfo::CDocGameInfo& info)
{
	Camera::CCamera2D* camera = info.GetCamera();
	int screenHalfW = info.GetScreenWidth() >> 1;
	int screenHalfH = info.GetScreenHeight() >> 1;

	bool isDirX = (m_directionX != 0) ? true : false;
	bool isDirY = (m_directionY != 0) ? true : false;

	int direction = -1;
	if(isDirY){
		direction = (m_directionY > 0) ? 2 : 0;
	}

	switch(direction)
	{
	case 0: // è„ï˚å¸
		if(m_posY < camera->GetPosition().y - screenHalfH - RECT_H){
			m_posY = m_firstPosY - screenHalfH;
		}
		break;
	case 2:	// â∫ï˚å¸
		if(m_posY > camera->GetPosition().y + screenHalfH + RECT_H){
			m_posY = m_firstPosY - screenHalfH;
		}
		break;
	default:
		tnl::DebugTrace("CMoveFloor->Err : Update\n");
		break;
	}
	currentPos = tnl::Vector3(m_posX, m_posY, 0);
}

void CMoveFloor::Draw(ShareInfo::CDocGameInfo& info)
{
	tnl::Vector3 camPos = info.GetCamera()->GetPosition();
	int viewPosX = m_posX - camPos.x + (info.GetScreenWidth() >> 1);
	int viewPosY = m_posY - camPos.y + (info.GetScreenHeight() >> 1);
	DrawRotaGraph(viewPosX, viewPosY, 1.0f, 0, m_gfxHdl, true);
}

} // namespace GameObject