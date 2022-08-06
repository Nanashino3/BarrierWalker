#ifndef SEQUENCE_CTITLESCENE_H_
#define SEQUENCE_CTITLESCENE_H_

#include "AMenuScene.h"

namespace Sound{ class CSoundManager; }
namespace Sequence
{
class CTitleScene : public AMenuScene
{
public:
	CTitleScene();
	virtual ~CTitleScene();

	virtual IScene* Update(const CPrimaryController&, ShareInfo::CDocGameInfo&) final;

private:
	bool PriTitleAnimation(ShareInfo::CDocGameInfo&);

private:
	int m_bgGfxHdl;
	int m_titleGfxHdl;
	int m_alphaValue;
	float m_elapsed;
	int m_selectMenu;
	Sound::CSoundManager* m_soundManager;
};

} // namespace Sequence

#endif // #ifndef SEQUENCE_CTITLESCENE_H_