#ifndef GAMEOBJECT_CITEMMANAGER_H_
#define GAMEOBJECT_CITEMMANAGER_H_

#include "AFixedObject.h"

namespace tnl{ class Vector3; }
namespace Sound{ class CSoundManager; }

namespace GameObject
{
class CItemManager : public AFixedObject
{
public:
	CItemManager();
	virtual ~CItemManager();

	virtual void Initialize(ShareInfo::CDocGameInfo&) final;
	virtual void Collision(tnl::Vector3&, tnl::Vector3&, ShareInfo::CDocGameInfo&) final;
	virtual void Draw(ShareInfo::CDocGameInfo&) final;

private:
	bool PriCollision(tnl::Vector3&, tnl::Vector3&);

private:
	Sound::CSoundManager* m_soundManager;
};

} // namespace GameObject

#endif // #ifndef GAMEOBJECT_CITEMMANAGER_H_