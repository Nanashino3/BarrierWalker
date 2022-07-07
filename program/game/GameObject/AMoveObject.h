#ifndef GAMEOBJECT_AMOVEOBJECT_H_
#define GAMEOBJECT_AMOVEOBJECT_H_

#include "IObject.h"

namespace GameObject
{
class AMoveObject : public IObject
{
public:
	AMoveObject();
	virtual ~AMoveObject();

	virtual void Initialize(ShareInfo::CDocGameInfo&) override;
	virtual void Move(tnl::Vector3&, tnl::Vector3&, ShareInfo::CDocGameInfo&) override;
	virtual void Collision(tnl::Vector3&, tnl::Vector3&, ShareInfo::CDocGameInfo&) override;
	virtual void Update(ShareInfo::CDocGameInfo&) override;
	virtual void Draw(ShareInfo::CDocGameInfo&) override;
};

} // namespace GameObject

#endif // #ifndef GAMEOBJECT_AMOVEOBJECT_H_