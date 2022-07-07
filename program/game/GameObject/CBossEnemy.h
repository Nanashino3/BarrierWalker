#ifndef GAMEOBJECT_CBOSSOBJECT_H_
#define GAMEOBJECT_CBOSSOBJECT_H_

#include "AMoveObject.h"

namespace GameObject
{
class CBossObject : public AMoveObject
{
public:
	CBossObject();
	virtual ~CBossObject();

	virtual void Initialize(ShareInfo::CDocGameInfo&) final;
	virtual void Collision(tnl::Vector3&, tnl::Vector3&, ShareInfo::CDocGameInfo&) final;
	virtual void Update(ShareInfo::CDocGameInfo&) final;
	virtual void Draw(ShareInfo::CDocGameInfo&) final;

private:
	int PriBossAction_Idle(tnl::Vector3&, int, float);
	int PriBossAction_Move(tnl::Vector3&, int, float);
	int PriBossAction_Attack();
};

} // namespace GameObject

#endif // #ifndef GAMEOBJECT_CBOSSOBJECT_H_