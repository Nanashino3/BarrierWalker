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
	// �O�����琶���ł��Ȃ��悤�ɂ��Ă���
	GameManager();
	GameManager(const GameManager&){}
	GameManager& operator=(const GameManager&);

private:
	static GameManager* m_instance;

	Sequence::CPrimaryController* m_sceneControl;
	ShareInfo::CDocGameInfo* m_docGameInfo;
};

#endif // #ifndef GAME_GAMEMANAGER_H_