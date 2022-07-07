#ifndef GAMEOBJECT_CFIXEDENEMY_H_
#define GAMEOBJECT_CFIXEDENEMY_H_

#include "AFixedObject.h"
#include "S_ENEMY_INFO.h"

namespace Sound{ class CSoundManager; }

namespace GameObject
{
class CFixedEnemy : public AFixedObject
{
public:
	CFixedEnemy();
	virtual ~CFixedEnemy();

	virtual void Initialize(ShareInfo::CDocGameInfo&) final;
	virtual void Collision(tnl::Vector3&, tnl::Vector3&, ShareInfo::CDocGameInfo&) final;
	virtual void Update(ShareInfo::CDocGameInfo&) final;
	virtual void Draw(ShareInfo::CDocGameInfo&) final;

private:
	bool PriShootStartTrigger(int, ShareInfo::CDocGameInfo&);
	bool PriBulletCollision(tnl::Vector3&, S_ENEMY_INFO&, ShareInfo::CDocGameInfo&);

private:
	unsigned int m_totalDefeat;
	unsigned int m_defeatCount;

	Sound::CSoundManager* m_soundManager;
};

} // namespace GameObject

#endif // #ifndef GAMEOBJECT_CFIXEDENEMY_H_