#ifndef SEQUENCE_PLAYGAME_CHELPSCENE_H_
#define SEQUENCE_PLAYGAME_CHELPSCENE_H_

#include "AMenuScene.h"

namespace Sequence
{
namespace PlayGame
{
class CHelpScene : public AMenuScene
{
public:
	CHelpScene();
	virtual ~CHelpScene();

	virtual IScene* Update(CSecondaryController&, ShareInfo::CDocGameInfo&) final;

private:
	std::vector<std::vector<std::string>> m_helpMsgDatas;
	std::vector<std::pair<std::string, std::string>> m_helpList;
	int m_msgPosX, m_msgPosY;
	int m_msgAlignmentPosX, m_msgAlignmentPosY;
	int m_selectMenu;
};

} // namespace PlayGame

} // namespace Sequence

#endif // #ifndef SEQUENCE_PLAYGAME_CHELPSCENE_H_