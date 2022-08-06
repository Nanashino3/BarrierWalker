#ifndef GAMEOBJECT_S_FLOOR_MANAGER_H_
#define GAMEOBJECT_S_FLOOR_MANAGER_H_

#include <vector>
#include "S_FLOOR_INFO.h"

namespace GameObject
{
struct S_FLOOR_MANAGER{
	float posX;
	float posY;

	int maxNum;
	int direction;
	int counter;
	float elapsed;
	float interval;

	std::vector<S_FLOOR_INFO> floors;
};

} // namespace GameObject

#endif // #ifndef GAMEOBJECT_S_FLOOR_MANAGER_H_