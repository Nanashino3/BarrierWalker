#include <time.h>
#include <algorithm>
#include "game_main.h"

#include "Sequence/CPrimaryController.h"
#include "ShareInfo/CDocGameInfo.h"
#include "Sound/CSoundManager.h"

namespace{
bool init = false;
Sequence::CPrimaryController* g_sceneControl = nullptr;
ShareInfo::CDocGameInfo* g_gameInfo = nullptr;
}

void gameMain(float delta_time) {

	if (!init) {
		init = true;

		Sound::CSoundManager::CreateInstance();
		g_gameInfo = ShareInfo::CDocGameInfo::GetInstance();
		g_sceneControl = new Sequence::CPrimaryController();
	}
	
	g_gameInfo->SetDeltaTime(delta_time);
	// シーン遷移をコントロール
	if(!g_sceneControl->Update(*g_gameInfo)){
		delete g_sceneControl;
		ShareInfo::CDocGameInfo::DestroyInstance();
		Sound::CSoundManager::DestroyInstance();
		exit(EXIT_SUCCESS);
	}

	return;
}

