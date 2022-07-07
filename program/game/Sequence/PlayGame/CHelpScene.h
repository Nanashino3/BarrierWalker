#ifndef SEQUENCE_PLAYGAME_CHELPSCENE_H_
#define SEQUENCE_PLAYGAME_CHELPSCENE_H_

#include "IScene.h"

namespace Sequence
{
namespace PlayGame
{
class CHelpScene : public IScene
{
public:
	CHelpScene();
	virtual ~CHelpScene();

	virtual IScene* Update(CSecondaryController&, ShareInfo::CDocGameInfo&) final;
};

} // namespace PlayGame

} // namespace Sequence

#endif // #ifndef SEQUENCE_PLAYGAME_CHELPSCENE_H_