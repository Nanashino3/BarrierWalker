#ifndef GAMEOBJECT_S_ITEM_INFO_H_
#define GAMEOBJECT_S_ITEM_INFO_H_

namespace GameObject
{
struct S_ITEM_INFO
{
	int imageID;	// 画像ID
	int item_kind;	// アイテム種
	float pos_x;	// 座標X
	float pos_y;	// 座標Y
	bool isEnable;	// 有効か
};

} // namespace GameObject

#endif // #ifndef GAMEOBJECT_S_ITEM_INFO_H_