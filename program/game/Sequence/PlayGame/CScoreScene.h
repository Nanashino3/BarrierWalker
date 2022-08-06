#ifndef SEQUENCE_PLAYGAME_CSOCRESCENE_H_
#define SEQUENCE_PLAYGAME_CSOCRESCENE_H_

#include "AMenuScene.h"

namespace Sequence
{
namespace PlayGame
{
class CScoreScene : public AMenuScene
{
public:
	CScoreScene();
	virtual ~CScoreScene();

	virtual IScene* Update(CSecondaryController&, ShareInfo::CDocGameInfo&) final;

private:
	std::vector<std::string> m_scoreMsgList;
	int m_msgPosX, m_msgPosY;
	int m_msgAlignmentPosX, m_msgAlignmentPosY;
	float m_elapsed;
	int m_keyCount;
	int m_selectMenu;
};

} // namespace PlayGame

} // namespace Sequence

#endif // #ifndef SEQUENCE_PLAYGAME_CSOCRESCENE_H_