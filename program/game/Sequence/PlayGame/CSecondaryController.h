// �񎟊Ǘ���
#ifndef SEQUENCE_PLAYGAME_CSECONDARYCONTROLLER_H_
#define SEQUENCE_PLAYGAME_CSECONDARYCONTROLLER_H_

#include "../IScene.h"

class CGameDirector;
namespace Sequence{
namespace PlayGame{
	class IScene;
}}

namespace Sequence
{
namespace PlayGame
{
class CSecondaryController : public Sequence::IScene
{
	typedef Sequence::CPrimaryController GrandController;
public:
	enum PRIMARY_SCENE_ID
	{
		PRIMARY_ID_TITLE = 0,	// �^�C�g�����
		PRIMARY_ID_ENDING,		// �G���f�B���O���
		PRIMARY_ID_NONE			// �J�ڂȂ�
	};

	enum SECONDARY_SCENE_ID
	{
		SECONDARY_ID_LOADING = 0,	// ���[�f�B���O���
		SECONDARY_ID_HELP,			// �w���v���
	};

	CSecondaryController(unsigned int);
	virtual ~CSecondaryController();

	virtual Sequence::IScene* Update(const GrandController&, ShareInfo::CDocGameInfo&) override;

	void StartLoading();
	CGameDirector* GetGameInstance(){ return m_gameDirector; }

	// �V�[���ړ�
	void MoveToScene(PRIMARY_SCENE_ID id){ m_primarySceneId = id; }
private:
	PRIMARY_SCENE_ID m_primarySceneId;
	CGameDirector* m_gameDirector;
	Sequence::PlayGame::IScene* m_currentScene;
};

} // namespace PlayGame

} // namespace Sequence

#endif // #ifndef SEQUENCE_PLAYGAME_CSECONDARYCONTROLLER_H_