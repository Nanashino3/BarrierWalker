#ifndef GAME_GAMEMANAGER_H_
#define GAME_GAMEMANAGER_H_

namespace Sequence{ class CPrimaryController; }
namespace ShareInfo{ class CDocGameInfo; }

class GameManager
{
public:
	static GameManager* GetInstance();
	static void DestroyInstance();

	bool Update(float);
private:
	static GameManager* m_instance;

	Sequence::CPrimaryController* m_sceneControl;
	ShareInfo::CDocGameInfo* m_docGameInfo;

	// 外部から生成できないようにしておく
	GameManager();
	GameManager(const GameManager&){}
	GameManager& operator=(const GameManager&);
};

#endif // #ifndef GAME_GAMEMANAGER_H_