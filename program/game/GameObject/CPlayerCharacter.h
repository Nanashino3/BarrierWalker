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
	float m_posX;				// X���W
	float m_posY;				// Y���W

	unsigned int m_actionType;	// �A�N�V������
	float m_velocityX;			// X���̑��x
	float m_velocityY;			// Y���̑��x

	int m_animType;				// �A�j���[�V������
	int m_animFrame;			// �A�j���[�V�����̃t���[����

	Sound::CSoundManager* m_soundManager;
};

} // namespace GameObject

#endif // #ifndef GAMEOBJECT_CPLAYERCHARACTER_H_