#ifndef SEQUENCE_PLAYGAME_CCLEARSCENE_H_
#define SEQUENCE_PLAYGAME_CCLEARSCENE_H_

#include "IScene.h"

namespace Sequence
{
namespace PlayGame
{
class CClearScene : public IScene
{
public:
	CClearScene();
	virtual ~CClearScene();

	virtual IScene* Update(CSecondaryController&, ShareInfo::CDocGameInfo&) final;

private:
	float m_elapsed;
};

} // namespace PlayGame

} // namespace Sequence

#endif // #ifndef SEQUENCE_PLAYGAME_CCLEARSCENE_H_