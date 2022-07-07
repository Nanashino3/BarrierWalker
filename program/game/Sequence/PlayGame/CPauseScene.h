#ifndef SEQUENCE_PLAYGAME_CPAUSESCENE_H_
#define SEQUENCE_PLAYGAME_CPAUSESCENE_H_

#include "IScene.h"

namespace Sound{ class CSoundManager; }
namespace Sequence
{
namespace PlayGame
{
class CPauseScene : public IScene
{
public:
	CPauseScene();
	virtual ~CPauseScene();

	virtual IScene* Update(CSecondaryController&, ShareInfo::CDocGameInfo&) final;

private:
	Sound::CSoundManager* m_soundManager;
};

} // namespace PlayGame

} // namespace Sequence

#endif // #ifndef SEQUENCE_PLAYGAME_CPAUSESCENE_H_