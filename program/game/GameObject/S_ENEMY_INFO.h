#ifndef GAMEOBJECT_S_ENEMY_INFO_H_
#define GAMEOBJECT_S_ENEMY_INFO_H_

#include <vector>
#include "../ShareInfo/S_BULLET_INFO.h"

namespace GameObject
{
struct S_ENEMY_INFO
{
	int imageID;		// �摜ID
	int attribute;		// ����
	float posX;			// ���WX
	float posY;			// ���WY
	bool isDefeat;		// �폜�\��

	bool isShoot;		// �����Ă��邩
	int direction;		// �U������
	int bulletCount;	// �e�J�E���g
	float elapsed;		// ���ˎ���
	float interval;		// ���ˊԊu
	float disitance;	// ���ˋ���
	float respawnPos;	// ���ŋ���

	// �e�z��
	std::vector<ShareInfo::S_BULLET_INFO> bullets;
};

} // namespace GameObject

#endif // #ifndef GAMEOBJECT_S_ENEMY_INFO_H_