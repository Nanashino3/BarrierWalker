#ifndef SEQUENCE_PLAYGAME_CPLAYSCENE_H_
#define SEQUENCE_PLAYGAME_CPLAYSCENE_H_

#include "IScene.h"

namespace Sound{ class CSoundManager; }
namespace Sequence
{
namespace PlayGame
{
class CPlayScene : public IScene
{
public:
	CPlayScene(bool isFadeIn = true);
	virtual ~CPlayScene();

	virtual IScene* Update(CSecondaryController&, ShareInfo::CDocGameInfo&) final;

private:
	bool m_isFadeIn;
	Sound::CSoundManager* m_soundManager;
};

} // namespace PlayGame

} // namespace Sequence

#endif // #ifndef SEQUENCE_PLAYGAME_CPLAYSCENE_H_