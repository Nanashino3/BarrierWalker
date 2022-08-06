#ifndef GAMEOBJECT_S_FLOOR_INFO_H_
#define GAMEOBJECT_S_FLOOR_INFO_H_

namespace GameObject
{
struct S_FLOOR_INFO{
	float posX;		// ���WX
	float posY;		// ���WY
	float prevPosX;	// �O��̍��WX
	float prevPosY;	// �O��̍��WY
	float velocity;	// ���x
	bool isAppear;	// �o��������
	bool isRespawn;	// ���X�|�[��������
};

} // namespace GameObject

#endif // #ifndef GAMEOBJECT_S_FLOOR_INFO_H_