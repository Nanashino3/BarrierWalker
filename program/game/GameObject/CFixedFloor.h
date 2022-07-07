#ifndef GAMEOBJECT_CFIXEDFLOOR_H_
#define GAMEOBJECT_CFIXEDFLOOR_H_

#include "AFixedObject.h"

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
};

} // namespace GameObject

#endif // #ifndef GAMEOBJECT_CFIXEDFLOOR_H_