#ifndef GAMEOBJECT_CMOVEOBJECTMANAGER_H_
#define GAMEOBJECT_CMOVEOBJECTMANAGER_H_

#include <vector>
#include "S_FLOOR_MANAGER.h"

namespace tnl{ class Vector3; }
namespace Camera{ class CCamera2D; }
namespace ShareInfo{ class CDocGameInfo; }
namespace GameObject{ class CMoveFloor; }

namespace GameObject
{
class CMoveObjectManager
{
public:
	CMoveObjectManager();
	~CMoveObjectManager();

	void Initialize(ShareInfo::CDocGameInfo&);
	void Move(ShareInfo::CDocGameInfo&);
	void Collision(tnl::Vector3&, tnl::Vector3&, ShareInfo::CDocGameInfo&);
	void Draw(ShareInfo::CDocGameInfo&);

private:
	int m_gfxHdl;
	std::vector<GameObject::S_FLOOR_MANAGER> m_floorManager;
};

} // namespace GameObject

#endif // #ifndef FUNCTION_CFUNCTIONCONTROLLER_H_