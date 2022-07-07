#ifndef GAMEOBJECT_S_ENEMY_INFO_H_
#define GAMEOBJECT_S_ENEMY_INFO_H_

#include <vector>
#include "../ShareInfo/S_BULLET_INFO.h"

namespace GameObject
{
struct S_ENEMY_INFO
{
	int imageID;		// 画像ID
	int attribute;		// 属性
	float posX;			// 座標X
	float posY;			// 座標Y
	bool isDefeat;		// 削除可能か

	bool isShoot;		// 撃っているか
	int direction;		// 攻撃方向
	int bulletCount;	// 弾カウント
	float elapsed;		// 発射時間
	float interval;		// 発射間隔
	float disitance;	// 発射距離
	float respawnPos;	// 消滅距離

	// 弾配列
	std::vector<ShareInfo::S_BULLET_INFO> bullets;
};

} // namespace GameObject

#endif // #ifndef GAMEOBJECT_S_ENEMY_INFO_H_