#ifndef GAMEOBJECT_S_FLOOR_INFO_H_
#define GAMEOBJECT_S_FLOOR_INFO_H_

namespace GameObject
{
struct S_FLOOR_INFO{
	float posX;		// 座標X
	float posY;		// 座標Y
	float prevPosX;	// 前回の座標X
	float prevPosY;	// 前回の座標Y
	float velocity;	// 速度
	bool isAppear;	// 出現したか
	bool isRespawn;	// リスポーンしたか
};

} // namespace GameObject

#endif // #ifndef GAMEOBJECT_S_FLOOR_INFO_H_