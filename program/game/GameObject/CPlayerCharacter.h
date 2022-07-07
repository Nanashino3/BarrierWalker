#ifndef GAMEOBJECT_CPLAYERCHARACTER_H_
#define GAMEOBJECT_CPLAYERCHARACTER_H_

#include "AMoveObject.h"

namespace Sound { class CSoundManager; }

namespace GameObject
{
class CPlayerCharacter : public AMoveObject
{
public:
	CPlayerCharacter();
	virtual ~CPlayerCharacter();

	virtual void Initialize(ShareInfo::CDocGameInfo&) final;
	virtual void Move(tnl::Vector3&, tnl::Vector3&, ShareInfo::CDocGameInfo&) final;
	virtual void Update(ShareInfo::CDocGameInfo&) final;
	virtual void Draw(ShareInfo::CDocGameInfo&) final;

private:
	void PriFunction_Dash(float);
	void PriFunction_Jump();
	void PriMoveConstrain(ShareInfo::CDocGameInfo&);
	void PriGameInfoClear(ShareInfo::CDocGameInfo&);

private:
	float m_posX;				// X座標
	float m_posY;				// Y座標

	unsigned int m_actionType;	// アクション種
	float m_velocityX;			// X軸の速度
	float m_velocityY;			// Y軸の速度

	int m_animType;				// アニメーション種
	int m_animFrame;			// アニメーションのフレーム数

	Sound::CSoundManager* m_soundManager;
};

} // namespace GameObject

#endif // #ifndef GAMEOBJECT_CPLAYERCHARACTER_H_