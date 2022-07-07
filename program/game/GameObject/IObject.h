#ifndef GAMEOBJECT_IOBJECT_H_
#define	GAMEOBJECT_IOBJECT_H_

namespace tnl{ class Vector3; }
namespace ShareInfo{ class CDocGameInfo; }

namespace GameObject
{
class IObject
{
public:
	virtual void Initialize(ShareInfo::CDocGameInfo&) = 0;
	virtual void Move(tnl::Vector3&, tnl::Vector3&, ShareInfo::CDocGameInfo&) = 0;
	virtual void Collision(tnl::Vector3&, tnl::Vector3&, ShareInfo::CDocGameInfo&) = 0;
	virtual void Update(ShareInfo::CDocGameInfo&) = 0;
	virtual void Draw(ShareInfo::CDocGameInfo&) = 0;
};

} // namespace Character

#endif // #ifndef GAMEOBJECT_AOBJECT_H_