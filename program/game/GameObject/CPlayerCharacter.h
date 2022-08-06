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
	// �A�j���[�V�����̎��
	enum E_ANIM_TYPE{
		ANIM_TYPE_RIGHT = 0,
		ANIM_TYPE_LEFT,

		MAX_ANIM_TYPE
	};

	float m_posX;				// X���W
	float m_posY;				// Y���W

	unsigned int m_actionType;	// �A�N�V������
	float m_velocityX;			// X���̑��x
	float m_velocityY;			// Y���̑��x

	int m_animType;				// �A�j���[�V������
	int m_animFrame;			// �A�j���[�V�����̃t���[����

	// �O���t�B�b�N�X�n���h��(�ő�A�j���[�V������ * �ő�t���[����)
	enum{ MAX_PLAYER_ANIMFRAME = 4 };
	int m_gfxHdl[MAX_ANIM_TYPE][MAX_PLAYER_ANIMFRAME];
	
	Sound::CSoundManager* m_soundManager;
};

} // namespace GameObject

#endif // #ifndef GAMEOBJECT_CPLAYERCHARACTER_H_