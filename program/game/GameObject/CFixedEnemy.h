#ifndef GAMEOBJECT_CFIXEDENEMY_H_
#define GAMEOBJECT_CFIXEDENEMY_H_

#include <vector>
#include <string>
#include "AFixedObject.h"
#include "S_ENEMY_INFO.h"
#include "../ShareInfo/CONST_GAME_VALUE.h"

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
	void PriEnemyRandom();
	bool PriShootStartTrigger(int, ShareInfo::CDocGameInfo&);
	bool PriBulletCollision(tnl::Vector3&, S_ENEMY_INFO&, ShareInfo::CDocGameInfo&);
	int PriIsJudgeTrilemma(int, int);
	int PriConvertImageIDToAttribute(int);

private:
	// �e��ł��o������
	enum E_SHOOT_DIRECTION
	{
		SHOOT_DIRECTION_UP = 0,
		SHOOT_DIRECTION_RIGHT,
		SHOOT_DIRECTION_DOWN,
		SHOOT_DIRECTION_LEFT,

		MAX_SHOOT_DIRECTION
	};
	unsigned int m_totalDefeat;
	unsigned int m_defeatCount;

	// �e��O���t�B�b�N�X�n���h��
	int m_gfxHdl[MAX_MAP_CHIP];
	std::vector<int> m_bulletGfxHdl;

	// �G�L�������
	std::vector<GameObject::S_ENEMY_INFO> m_enemys;

	std::vector<int> m_scheduleList;
	int m_scheduleCount;
	Sound::CSoundManager* m_soundManager;
};

} // namespace GameObject

#endif // #ifndef GAMEOBJECT_CFIXEDENEMY_H_