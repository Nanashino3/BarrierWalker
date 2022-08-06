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
	// アニメーションの種類
	enum E_ANIM_TYPE{
		ANIM_TYPE_RIGHT = 0,
		ANIM_TYPE_LEFT,

		MAX_ANIM_TYPE
	};

	float m_posX;				// X座標
	float m_posY;				// Y座標

	unsigned int m_actionType;	// アクション種
	float m_velocityX;			// X軸の速度
	float m_velocityY;			// Y軸の速度

	int m_animType;				// アニメーション種
	int m_animFrame;			// アニメーションのフレーム数

	// グラフィックスハンドル(最大アニメーション数 * 最大フレーム数)
	enum{ MAX_PLAYER_ANIMFRAME = 4 };
	int m_gfxHdl[MAX_ANIM_TYPE][MAX_PLAYER_ANIMFRAME];
	
	Sound::CSoundManager* m_soundManager;
};

} // namespace GameObject

#endif // #ifndef GAMEOBJECT_CPLAYERCHARACTER_H_