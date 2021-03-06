#ifndef GAMEOBJECT_S_ENEMY_INFO_H_
#define GAMEOBJECT_S_ENEMY_INFO_H_

#include <vector>
#include "../ShareInfo/S_BULLET_INFO.h"

namespace GameObject
{
struct S_ENEMY_INFO
{
	int imageID;		// æID
	int attribute;		// ®«
	float posX;			// ÀWX
	float posY;			// ÀWY
	bool isDefeat;		// íÂ\©

	bool isShoot;		// ÁÄ¢é©
	int direction;		// Uûü
	int bulletCount;	// eJEg
	float elapsed;		// ­ËÔ
	float interval;		// ­ËÔu
	float disitance;	// ­Ë£
	float respawnPos;	// ÁÅ£

	// ezñ
	std::vector<ShareInfo::S_BULLET_INFO> bullets;
};

} // namespace GameObject

#endif // #ifndef GAMEOBJECT_S_ENEMY_INFO_H_