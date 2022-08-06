#ifndef SEQUENCE_PLAYGAME_CPAUSESCENE_H_
#define SEQUENCE_PLAYGAME_CPAUSESCENE_H_

#include "AMenuScene.h"

namespace Sound{ class CSoundManager; }
namespace Sequence
{
namespace PlayGame
{
class CPauseScene : public AMenuScene
{
public:
	CPauseScene();
	virtual ~CPauseScene();

	virtual IScene* Update(CSecondaryController&, ShareInfo::CDocGameInfo&) final;

private:
	Sound::CSoundManager* m_soundManager;
	int m_selectMenu;
};

} // namespace PlayGame

} // namespace Sequence

#endif // #ifndef SEQUENCE_PLAYGAME_CPAUSESCENE_H_