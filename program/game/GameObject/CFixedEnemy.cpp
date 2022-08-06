//*************************************************************
// �Œ�G�N���X
// TODO�F�G���e������I�ɕێ����Ă���̂͂����̂�
// �@�@�@�ǂ����̃^�C�~���O�Ő؂藣��

#include "CFixedEnemy.h"

#include "../Utility/CMath.h"
#include "../Utility/CUtility.h"
#include "../Utility/CImageManager.h"
#include "../Camera/CCamera2D.h"
#include "../ShareInfo/CDocGameInfo.h"
#include "../ShareInfo/CONST_GAME_VALUE.h"
#include "../Sound/CSoundManager.h"

namespace{
const std::vector<std::string> g_enemyCsv = {
	//"resource/enemy/enemy_layout_1.csv",
	//"resource/enemy/enemy_layout_2.csv",
	"resource/enemy/enemy_layout_3.csv"
};
// �e�摜
const std::vector<std::string> g_bulletImage = {
	"resource/sticon1b-3.png",	// ��
	"resource/sticon1d-3.png",	// ��
	"resource/sticon1c-3.png"	// ��
};
// �e�F�e��
const int MAX_BULLET = 10;
// ��`�T�C�Y
const int P_RECT_W = 24;
const int P_RECT_H = PLAYER_RECT_H;
const int RECT_W = MAP_CHIP_W << 1;
const int RECT_H = MAP_CHIP_H << 1;
// �e�F�ł��o������
const int SHOOT_UP		= 0;
const int SHOOT_RIGHT	= 1;
const int SHOOT_BOTTOM	= 2;
const int SHOOT_LEFT	= 3;

// �e��O���t�B�b�N�X�n���h��
int g_gfxHdl[MAX_MAP_CHIP] = {0};
int g_bulletGfxHdl[MAX_ATTRIBUTE_TYPE] = {0};

// �G�L�������
std::vector<GameObject::S_ENEMY_INFO> g_enemys;
// �G�L����CSV�Ǎ��p
std::vector<std::vector<std::string>> g_enemyDetas;
}

namespace GameObject
{
CFixedEnemy::CFixedEnemy()
: m_totalDefeat(0)
, m_defeatCount(0)
{
	// �G�L�����摜�Ǎ�
	LoadDivGraph("resource/block_ds.png", MAX_MAP_CHIP, MAP_CHIP_X_NUM, MAP_CHIP_Y_NUM, MAP_CHIP_W, MAP_CHIP_H, g_gfxHdl);
	for(int i = 0; i < g_bulletImage.size(); i++){
		// �e�摜�Ǎ�
		g_bulletGfxHdl[i] = Utility::CImageManager::GetInstance()->LoadGraphEx(g_bulletImage[i].c_str());
	}

	m_soundManager = Sound::CSoundManager::GetInstance();
}

CFixedEnemy::~CFixedEnemy()
{
	for(int i = 0; i < MAX_MAP_CHIP; i++){
		DeleteGraph(g_gfxHdl[i]);
	}
	for(int i = 0; i < g_bulletImage.size(); i++){
		Utility::CImageManager::GetInstance()->DeleteGraphEx(g_bulletGfxHdl[i]);
	}
}

//****************************************************************************
// �֐����FInitialize
// �T�@�v�F����������
// ���@���F��1�����@�Q�[�����
// �߂�l�F�Ȃ�
// �ځ@�ׁF�G���(CSV)�̓Ǎ���f�[�^�ݒ�
//****************************************************************************
void CFixedEnemy::Initialize(ShareInfo::CDocGameInfo& info)
{
	// �v�f�̏������ƃ������T�C�Y�����킹��
	g_enemyDetas.clear();
	g_enemyDetas.shrink_to_fit();
	for(int i = 0; i < g_enemys.size(); i++){
		g_enemys[i].bullets.clear();
		g_enemys[i].bullets.shrink_to_fit();
	}
	g_enemys.clear();
	g_enemys.shrink_to_fit();

	// �����j��
	m_totalDefeat = info.GetScoreCalcSrcInfo(ACCESS_KEY_DEFEAT);

	// �GCSV�Ǎ�
	int index = info.GetStageIndex();
	g_enemyDetas = tnl::LoadCsv(g_enemyCsv[index].c_str());

	Camera::CCamera2D* camera = info.GetCamera();

	// TODO�F���G�̏ꍇ�����_���ő������擾����K�v������̂ň�U����������
	Utility::Initialize();
	for(int i = 0; i < g_enemyDetas.size(); i++){
		// CSV�̓��e�ŏ�����
		S_ENEMY_INFO enemyInfo;
		enemyInfo.imageID = atoi(g_enemyDetas[i][0].c_str());
		enemyInfo.attribute = Utility::ConvertImageIDToAttribute(enemyInfo.imageID);
		enemyInfo.direction = atoi(g_enemyDetas[i][1].c_str());
		enemyInfo.interval = atof(g_enemyDetas[i][3].c_str());
		enemyInfo.disitance = atof(g_enemyDetas[i][4].c_str());
		enemyInfo.respawnPos = atof(g_enemyDetas[i][5].c_str());
		enemyInfo.posX = atof(g_enemyDetas[i][6].c_str()) - (info.GetScreenWidth() >> 1);
		enemyInfo.posY = atof(g_enemyDetas[i][7].c_str()) - (info.GetScreenHeight() >> 1);

		// �Œ�l�ŏ�����
		enemyInfo.isDefeat = false;
		enemyInfo.isShoot = false;
		enemyInfo.elapsed = 0.0f;
		enemyInfo.bulletCount = 0;

		// �e�֘A������(TODO�F���Ԃ��Ȃ��̂œG�N���X���ł������)
		for(int count = 0; count < MAX_BULLET; count++){
			ShareInfo::S_BULLET_INFO bullet;
			bullet.posX = enemyInfo.posX;
			bullet.posY = enemyInfo.posY;
			bullet.speed = atoi(g_enemyDetas[i][2].c_str());
			bullet.attribute = enemyInfo.attribute;
			bullet.isAppear = false;
			bullet.isRespawn = false;
			enemyInfo.bullets.push_back(bullet);
		}
		g_enemys.push_back(enemyInfo);
	}
}

//****************************************************************************
// �֐����FUpdate
// �T�@�v�F�X�V����
// ���@���F��1�����@�Q�[�����
// �߂�l�F�Ȃ�
// �ځ@�ׁF���ɂȂ�(����e�̍X�V�����ɂȂ��Ă���̂Ő؂藣������)
//****************************************************************************
void CFixedEnemy::Update(ShareInfo::CDocGameInfo& info)
{
	//**************************
	// �e�֘A����
	//**************************
	for(int enemyNum = 0; enemyNum < g_enemys.size(); enemyNum++){
		// �c�e���Ǘ�
		if(g_enemys[enemyNum].bulletCount == MAX_BULLET){
			g_enemys[enemyNum].bulletCount = 0;
			g_enemys[enemyNum].isShoot = false;
		}

		// ���ˊJ�n�g���K�[
		if(PriShootStartTrigger(enemyNum, info)){ g_enemys[enemyNum].isShoot = true; }

		// �e���ː���(�C���^�[�o��)
		g_enemys[enemyNum].elapsed += info.GetDeltaTime();
		if(g_enemys[enemyNum].elapsed > g_enemys[enemyNum].interval){
			g_enemys[enemyNum].elapsed = 0.0f;

			// ���ˉ\���e���o�����Ă��Ȃ������X�|�[����ԂłȂ��e�Ȃ甭�˂���
			bool isAppear = g_enemys[enemyNum].bullets[g_enemys[enemyNum].bulletCount].isAppear;
			bool isRespawn = g_enemys[enemyNum].bullets[g_enemys[enemyNum].bulletCount].isRespawn;
			if(g_enemys[enemyNum].isShoot && !isAppear && !isRespawn){
				// ���݂̒e��\���\��Ԃɂ���
				g_enemys[enemyNum].bullets[g_enemys[enemyNum].bulletCount].isAppear = true;

				// ���e���U����
				g_enemys[enemyNum].bulletCount++;
			}
		}

		// �ړ��Ǘ�
		for(int count = 0; count < MAX_BULLET; count++){
			if(!g_enemys[enemyNum].bullets[count].isAppear){ continue; }
			bool isInit = false;
			int bulletDx = 0, bulletDy = 0, distance = 0;
			switch(g_enemys[enemyNum].direction)
			{
			case SHOOT_UP:
				bulletDy = -1;
				distance = g_enemys[enemyNum].posY - g_enemys[enemyNum].respawnPos;
				if(g_enemys[enemyNum].bullets[count].posY < distance){ isInit = !isInit;	}
				break;
			case SHOOT_RIGHT:
				bulletDx =  1;
				distance = g_enemys[enemyNum].posX + g_enemys[enemyNum].respawnPos;
				if(g_enemys[enemyNum].bullets[count].posX > distance){ isInit = !isInit; }
				break;
			case SHOOT_BOTTOM:
				bulletDy =  1;
				distance = g_enemys[enemyNum].posY + g_enemys[enemyNum].respawnPos;
				if(g_enemys[enemyNum].bullets[count].posY > distance){ isInit = !isInit; }
				break;
			case SHOOT_LEFT:
				bulletDx = -1;
				distance = g_enemys[enemyNum].posX - g_enemys[enemyNum].respawnPos;
				if(g_enemys[enemyNum].bullets[count].posX < distance){ isInit = !isInit; }
				break;
			}
			g_enemys[enemyNum].bullets[count].posX += g_enemys[enemyNum].bullets[count].speed * bulletDx;
			g_enemys[enemyNum].bullets[count].posY += g_enemys[enemyNum].bullets[count].speed * bulletDy;

			// ��񏉊���
			if(!isInit){ continue; }
			g_enemys[enemyNum].bullets[count].isAppear = false;
			g_enemys[enemyNum].bullets[count].isRespawn = false;
			g_enemys[enemyNum].bullets[count].posX = g_enemys[enemyNum].posX;
			g_enemys[enemyNum].bullets[count].posY = g_enemys[enemyNum].posY;
		}
	}

	// ���j�����X�V
	if(info.GetIsClear()){ info.SetScoreCalcSrcInfo(ACCESS_KEY_DEFEAT, m_totalDefeat + m_defeatCount); }
}

//****************************************************************************
// �֐����FCollision
// �T�@�v�F�Փ˔��菈��
// ���@���F��1�����@���݈ʒu
// �@�@�@�@��2�����@�O��ʒu
// �@�@�@�@��3�����@�Q�[�����
// �߂�l�F�Ȃ�
// �ځ@�ׁF�G(�e)�̏Փ˔��菈��
//****************************************************************************
void CFixedEnemy::Collision(tnl::Vector3& currentPos, tnl::Vector3& prevPos, ShareInfo::CDocGameInfo& info)
{
	// �o���A���N�����Ă��邩
	unsigned int functionType = info.GetFunctionType();
	bool isBarrier = functionType & FUNCTION_TYPE_BARRIER;

	info.SetIsPlayerHit(false);
	for(int i = 0; i < g_enemys.size(); i++){
		//************************
		// ���菀��(�f�[�^�̏���)
		//************************
		S_ENEMY_INFO enemy = g_enemys[i];
		tnl::Vector3 enemyPos = tnl::Vector3(enemy.posX, enemy.posY, 0);

		//************************
		// �o���A�ƓG�̔���
		//************************

		// �o���A�̍��W�ƃv���C���[�̍��W�͓����Ȃ̂ŗ��p����
		tnl::Vector3 tempPos = currentPos;
		if(isBarrier && !g_enemys[i].isDefeat){
			if(IsIntersectRectToCorrectPosition(tempPos, prevPos, P_RECT_W, P_RECT_H, enemyPos, RECT_W, RECT_H) == CORRECT_UP){
				int judge = Utility::IsJudgeTrilemma(info.GetBarrierColor(BARRIER_CURRENT), enemy.attribute);
				if(judge == JUDGE_WIN){
					g_enemys[i].isDefeat = true;
					m_defeatCount++;
					m_soundManager->PlaySE(SE_ID_DEFEAT);
				}
			}
		}

		//************************
		// �v���C���[�ƓG�̔���
		//************************
		if(enemy.isDefeat){ continue; }
		int correctType = IsIntersectRectToCorrectPosition(currentPos, prevPos, P_RECT_W, P_RECT_H, enemyPos, RECT_W, RECT_H);
		if(correctType != 0){
			info.SetIsCollision(true);
			info.SetCurrentPos(currentPos);
			info.SetCorrectType(ShareInfo::COLLISION_OBJECT_ENMEY, correctType);
		}

		//************************
		// �e�̏Փ˔���(Private)
		//************************
		if(PriBulletCollision(tempPos, g_enemys[i], info) && !info.GetIsPlayerHit()){
			info.SetIsPlayerHit(true);
		}
	}
}

//****************************************************************************
// �֐����FDraw
// �T�@�v�F�`�揈��
// ���@���F��1�����@�Q�[�����
// �߂�l�F�Ȃ�
// �ځ@�ׁF�G(�e)�̕`�揈��
//****************************************************************************
void CFixedEnemy::Draw(ShareInfo::CDocGameInfo& info)
{
	Camera::CCamera2D* camera = info.GetCamera();
	int screenHalfW = info.GetScreenWidth() >> 1;
	int screenHalfH = info.GetScreenHeight() >> 1;
	for(int i = 0; i < g_enemys.size(); i++){
		S_ENEMY_INFO enemy = g_enemys[i];

		//***************************
		// �G�`��
		//***************************
		if(enemy.isDefeat){ continue; }
		int viewEnemyPosX = enemy.posX - camera->GetPosition().x + screenHalfW;
		int viewEnemyPosY = enemy.posY - camera->GetPosition().y + screenHalfH;
		DrawRotaGraph(viewEnemyPosX, viewEnemyPosY, 2.0f, 0, g_gfxHdl[enemy.imageID], true);

		//***************************
		// �e�`��
		//***************************
		for(int bullet_cnt = 0; bullet_cnt < MAX_BULLET; bullet_cnt++){
			if(!enemy.bullets[bullet_cnt].isAppear || enemy.bullets[bullet_cnt].isRespawn){ continue; }

			int viewBulletPosX = enemy.bullets[bullet_cnt].posX - camera->GetPosition().x + screenHalfW;
			int viewBulletPosY = enemy.bullets[bullet_cnt].posY - camera->GetPosition().y + screenHalfH;
			DrawRotaGraph(viewBulletPosX, viewBulletPosY, 1.0f, 0, g_bulletGfxHdl[enemy.bullets[bullet_cnt].attribute], true);
		}
	}
}

//****************************************************************************
// �֐����FPriShootStartTrigger
// �T�@�v�F���ˊJ�n�g���K�[
// ���@���F��1�����@�G�ԍ�
// �@�@�@�@��2�����@�Q�[�����
// �߂�l�F�Ȃ�
// �ځ@�ׁF�G���e�𔭎˂��邱�Ƃ��\���𔻒f����
//****************************************************************************
bool CFixedEnemy::PriShootStartTrigger(int enemyNum, ShareInfo::CDocGameInfo& info)
{
	// ��ɔ��˂���
	if(g_enemys[enemyNum].disitance == 0){ return true; }

	// �v���C���[���ǂ̕����ɂ��邩���擾
	tnl::Vector3 currentPos = info.GetCurrentPos();
	tnl::Vector3 enemyPos = tnl::Vector3(g_enemys[enemyNum].posX, g_enemys[enemyNum].posY, 0);
	int direction = tnl::GetRegionPointAndRect(currentPos, enemyPos, RECT_W, RECT_H);
	if(g_enemys[enemyNum].direction == direction){
		// �v���C���[���߂Â��Ă�����e�𔭎ˉ\
		float distance = Utility::GetDistance2D(enemyPos, currentPos);
		if(distance < g_enemys[enemyNum].disitance){ return true; }
	}

	return false;
}

//****************************************************************************
// �֐����FPriBulletCollision
// �T�@�v�F�e�Փ˔���(Private)
// ���@���F��1�����@���݈ʒu
// �@�@�@�@��2�����@�G���
// �@�@�@�@��3�����@�Q�[�����
// �߂�l�F�Ȃ�
// �ځ@�ׁF�e�̏Փ˔��菈��(�O�����ݔ�������{)
//****************************************************************************
bool CFixedEnemy::PriBulletCollision(tnl::Vector3& current_pos, S_ENEMY_INFO& enemy_info, ShareInfo::CDocGameInfo& info)
{
	unsigned int functionType = info.GetFunctionType();
	bool isBarrier = functionType & FUNCTION_TYPE_BARRIER;

	int isPlayerHit = false;
	for(int count = 0; count < MAX_BULLET; count++){
		
		if(!enemy_info.bullets[count].isAppear || enemy_info.bullets[count].isRespawn){ continue; }

		// ��`A(���@�ƃo���A)
		int left_a	 = current_pos.x - 4;
		int right_a  = current_pos.x + 4;
		int top_a	 = current_pos.y - 48 / 2;
		int bottom_a = current_pos.y + 48 / 2;

		// ��`B(�e)
		float bulletPosX = enemy_info.bullets[count].posX;
		float bulletPosY = enemy_info.bullets[count].posY;
		tnl::Vector3 bullet_pos = tnl::Vector3(bulletPosX, bulletPosY, 0);

		int left_b	 = bulletPosX - 24 / 2;
		int right_b	 = bulletPosX + 24 / 2;
		int top_b	 = bulletPosY - 24 / 2;
		int bottom_b = bulletPosY + 24 / 2;

		// �␳�Ȃ��Փ˔���
		if(tnl::IsIntersectRectPrimitive(left_a, right_a, top_a, bottom_a, left_b, right_b, top_b, bottom_b)){
			isPlayerHit = true;
			if(isBarrier){
				// �o���A������ꍇ�͑������m�F
				int barrierColor = info.GetBarrierColor(BARRIER_CURRENT);
				int attribute = enemy_info.bullets[count].attribute;
				
				// �O�����ݔ���
				int ret = Utility::IsJudgeTrilemma(barrierColor, attribute);

				// �K�v�ȃt���O�𑀍삷��
				bool isInit = (ret == JUDGE_EVEN || ret == JUDGE_WIN) ? true : false;
				isPlayerHit = (ret == JUDGE_LOSE) ? true : false;

				// �e�֘A�̏�����
				if(!isInit){ continue; }
				// ���X�|�[���t���O�𗧂Ă�(���ߋ����΍�)
				// ����ʒu�܂œ����̒e���ړ�����`�ɂ���
				enemy_info.bullets[count].isRespawn = true;

				// �e�K�[�h��
				m_soundManager->PlaySE(SE_ID_GUARD);
			}
		}
	}

	return isPlayerHit;
}

} // namespace GameObject