#ifndef SEQUENCE_PLAYGAME_CGAMEOVERSCENE_H_
#define SEQUENCE_PLAYGAME_CGAMEOVERSCENE_H_

#include "AMenuScene.h"

namespace Sound{ class CSoundManager; }
namespace Sequence
{
namespace PlayGame
{
class CGameOverScene : public AMenuScene
{
public:
	CGameOverScene();
	virtual ~CGameOverScene();

	virtual IScene* Update(CSecondaryController&, ShareInfo::CDocGameInfo&) final;

private:
	int m_selectMenu;
	Sound::CSoundManager* m_soundManager;
};

} // namespace PlayGame

} // namespace Sequence

#endif // #ifndef SEQUENCE_PLAYGAME_CGAMEOVERSCENE_H_