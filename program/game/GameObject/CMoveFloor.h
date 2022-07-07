#ifndef GAMEOBJECT_CMOVEFLOOR_H_
#define GAMEOBJECT_CMOVEFLOOR_H_

#include <tuple>

namespace tnl{ class Vector3; }
namespace ShareInfo{ class CDocGameInfo; }

namespace GameObject
{
class CMoveFloor
{
public:
	CMoveFloor(int, int, float, float,float, float);
	~CMoveFloor();

	void Initialize(ShareInfo::CDocGameInfo&);
	void Move(ShareInfo::CDocGameInfo&);
	void Collision(tnl::Vector3&, tnl::Vector3&, ShareInfo::CDocGameInfo&);
	void Update(tnl::Vector3&, tnl::Vector3&, ShareInfo::CDocGameInfo&);
	void Draw(ShareInfo::CDocGameInfo&);

	std::tuple<int, int> GetDirection() const { return std::make_tuple(m_directionX, m_directionY); }

private:
	int m_gfxHdl;
	float m_posX;
	float m_posY;

	int m_directionX;
	int m_directionY;
	float m_firstPosX;
	float m_firstPosY;
	float m_velocityX;
	float m_velocityY;
};

}

#endif // #ifndef GAMEOBJECT_CMOVEFLOOR_H_