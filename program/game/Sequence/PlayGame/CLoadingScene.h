#ifndef SEQUENCE_PLAYGAME_CLOADINGSCENE_H_
#define SEQUENCE_PLAYGAME_CLOADINGSCENE_H_

#include "IScene.h"

namespace Sequence
{
namespace PlayGame
{
class CLoadingScene : public IScene
{
public:
	CLoadingScene();
	virtual ~CLoadingScene();

	virtual IScene* Update(CSecondaryController&, ShareInfo::CDocGameInfo&) final;

private:
	bool m_init;
};

} // namespace PlayGame

} // namespace Sequence

#endif // #ifndef SEQUENCE_PLAYGAME_CLOADINGSCENE_H_