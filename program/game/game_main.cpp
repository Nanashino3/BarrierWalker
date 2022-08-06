#include <time.h>
#include <algorithm>
#include "game_main.h"
#include "GameManager.h"

void gameMain(float delta_time) {

	GameManager* manager = GameManager::GetInstance();
	if(!manager->Update(delta_time)){
		GameManager::DestroyInstance();
		exit(EXIT_SUCCESS);
	}
	return;
}

