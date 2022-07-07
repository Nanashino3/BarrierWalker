#ifndef SEQUENCE_PLAYGAME_CSOCRESCENE_H_
#define SEQUENCE_PLAYGAME_CSOCRESCENE_H_

#include "IScene.h"

namespace Sequence
{
namespace PlayGame
{
class CScoreScene : public IScene
{
public:
	CScoreScene();
	virtual ~CScoreScene();

	virtual IScene* Update(CSecondaryController&, ShareInfo::CDocGameInfo&) final;

private:
	float m_elapsed;
	int m_key_count;
};

} // namespace PlayGame

} // namespace Sequence

#endif // #ifndef SEQUENCE_PLAYGAME_CSOCRESCENE_H_