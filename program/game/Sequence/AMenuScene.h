#ifndef SEQUENCE_AMENUSCENE_H_
#define SEQUENCE_AMENUSCENE_H_

#include <string>
#include <vector>
#include "IScene.h"

namespace Sequence
{
class AMenuScene : public IScene
{
public:
	AMenuScene(const std::string&);
	virtual ~AMenuScene();

protected:
	void MenuSelect(int&);

protected:
	std::vector<std::vector<std::string>> m_sceneDatas;
	std::string m_sceneTitle;
	int m_sceneTextPosX, m_sceneTextPosY;
	std::vector<std::string> m_msgList;
	int m_menuTextPosX, m_menuTextPosY;
	int m_alignmentX, m_alignmentY;
	int m_cursorPos;
};

} // namespace Sequence

#endif // #ifndef SEQUENCE_AMENUSCENE_H_