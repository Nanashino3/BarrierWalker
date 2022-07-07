// 二次管理者
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
		PRIMARY_ID_TITLE = 0,	// タイトル画面
		PRIMARY_ID_ENDING,		// エンディング画面
		PRIMARY_ID_NONE			// 遷移なし
	};

	enum SECONDARY_SCENE_ID
	{
		SECONDARY_ID_LOADING = 0,	// ローディング画面
		SECONDARY_ID_HELP,			// ヘルプ画面
	};

	CSecondaryController(unsigned int);
	virtual ~CSecondaryController();

	virtual Sequence::IScene* Update(const GrandController&, ShareInfo::CDocGameInfo&) override;

	void StartLoading();
	CGameDirector* GetGameInstance(){ return m_gameDirector; }

	// シーン移動
	void MoveToScene(PRIMARY_SCENE_ID id){ m_primarySceneId = id; }
private:
	PRIMARY_SCENE_ID m_primarySceneId;
	CGameDirector* m_gameDirector;
	Sequence::PlayGame::IScene* m_currentScene;
};

} // namespace PlayGame

} // namespace Sequence

#endif // #ifndef SEQUENCE_PLAYGAME_CSECONDARYCONTROLLER_H_