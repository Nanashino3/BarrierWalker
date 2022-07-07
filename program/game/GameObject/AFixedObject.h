#ifndef GAMEOBJECT_AFIXEDOBJECT_H_
#define GAMEOBJECT_AFIXEDOBJECT_H_

#include "IObject.h"

namespace GameObject
{
class AFixedObject : public IObject
{
public:
	AFixedObject();
	virtual ~AFixedObject();

	virtual void Initialize(ShareInfo::CDocGameInfo&) override;
	virtual void Move(tnl::Vector3&, tnl::Vector3&, ShareInfo::CDocGameInfo&) final;
	virtual void Collision(tnl::Vector3&, tnl::Vector3&, ShareInfo::CDocGameInfo&) override;
	virtual void Update(ShareInfo::CDocGameInfo&) override;
	virtual void Draw(ShareInfo::CDocGameInfo&) override;
};

} // namespace GameObject

#endif // #ifndef GAMEOBJECT_AFIXEDOBJECT_H_