#ifndef GAMEOBJECT_CFIXEDFLOOR_H_
#define GAMEOBJECT_CFIXEDFLOOR_H_

#include <vector>
#include <string>
#include "AFixedObject.h"
#include "../ShareInfo/CONST_GAME_VALUE.h"

namespace tnl{ class Vector3; }
namespace ShareInfo{ class CDocGameInfo; }

namespace GameObject
{
class CFixedFloor : public AFixedObject
{
public:
	CFixedFloor();
	virtual ~CFixedFloor();

	virtual void Initialize(ShareInfo::CDocGameInfo&) final;
	virtual void Collision(tnl::Vector3&, tnl::Vector3&, ShareInfo::CDocGameInfo&) final;
	virtual void Draw(ShareInfo::CDocGameInfo&) final;

private:
	int m_firstPosX;
	int m_firstPosY;
	int m_gfxHdl[MAX_MAP_CHIP];
	std::vector<std::vector<std::string>> m_stageDatas;
};

} // namespace GameObject

#endif // #ifndef GAMEOBJECT_CFIXEDFLOOR_H_