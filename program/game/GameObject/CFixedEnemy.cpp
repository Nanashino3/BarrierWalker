//*************************************************************
// �Œ�G�N���X
// TODO�F�G���e������I�ɕێ����Ă���̂͂����̂�
// �@�@�@�ǂ����̃^�C�~���O�Ő؂藣��
#include "CFixedEnemy.h"
#include "CONST_GAMEOBJECT_VALUE.h"

#include "../Camera/CCamera2D.h"
#include "../ShareInfo/CDocGameInfo.h"
#include "../Sound/CSoundManager.h"
#include "../Utility/CMath.h"
#include "../Utility/CScreenEffect.h"
#include "../Utility/CImageManager.h"

namespace GameObject
{
CFixedEnemy::CFixedEnemy()
: m_totalDefeat(0)
, m_defeatCount(0)
, m_scheduleCount(0)
{
	// �G�L�����摜�Ǎ�
	LoadDivGraph("resource/block_ds.png", MAX_MAP_CHIP, MAP_CHIP_X_NUM, MAP_CHIP_Y_NUM, MAP_CHIP_W, MAP_CHIP_H, m_gfxHdl);
	
	// �e�摜�Ǎ�
	std::vector<std::string> bulletImgList = {
		"resource/sticon1b-3.png",	// ��
		"resource/sticon1d-3.png",	// ��
		"resource/sticon1c-3.png"	// ��
	};
	for(const auto& image : bulletImgList){
		int bulletGfxHdl = Utility::CImageManager::GetInstance()->LoadGraphEx(image.c_str());
		m_bulletGfxHdl.push_back(bulletGfxHdl);
	}
	m_soundManager = Sound::CSoundManager::GetInstance();
}

CFixedEnemy::~CFixedEnemy()
{
	for(int i = 0; i < MAX_MAP_CHIP; i++){
		DeleteGraph(m_gfxHdl[i]);
	}
	for(const auto& gfxHdl : m_bulletGfxHdl){
		Utility::CImageManager::GetInstance()->DeleteGraphEx(gfxHdl);
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
	for(int i = 0; i < m_enemys.size(); i++){
		m_enemys[i].bullets.clear();
		m_enemys[i].bullets.shrink_to_fit();
	}
	m_enemys.clear();
	m_enemys.shrink_to_fit();

	// �����j��
	m_totalDefeat = info.GetScoreCalcSrcInfo(ACCESS_KEY_DEFEAT);

	// �GCSV�Ǎ�
	int index = info.GetStageIndex();
	std::string fileList[] = {
		"resource/enemy/enemy_layout_1.csv",
		"resource/enemy/enemy_layout_2.csv",
		"resource/enemy/enemy_layout_3.csv"
	};
	std::vector<std::vector<std::string>> enemyDetas = tnl::LoadCsv(fileList[index]);

	Camera::CCamera2D* camera = info.GetCamera();

	// TODO�F���G�̏ꍇ�����_���ő������擾����K�v������̂ň�U����������
	PriEnemyRandom();
	for(int i = 0; i < enemyDetas.size(); i++){
		// CSV�̓��e�ŏ�����
		S_ENEMY_INFO enemyInfo;
		enemyInfo.imageID = atoi(enemyDetas[i][0].c_str());
		enemyInfo.attribute = PriConvertImageIDToAttribute(enemyInfo.imageID);
		enemyInfo.direction = atoi(enemyDetas[i][1].c_str());
		enemyInfo.interval = atof(enemyDetas[i][3].c_str());
		enemyInfo.disitance = atof(enemyDetas[i][4].c_str());
		enemyInfo.respawnPos = atof(enemyDetas[i][5].c_str());
		enemyInfo.posX = atof(enemyDetas[i][6].c_str()) - (info.GetScreenWidth() >> 1);
		enemyInfo.posY = atof(enemyDetas[i][7].c_str()) - (info.GetScreenHeight() >> 1);

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
			bullet.speed = atoi(enemyDetas[i][2].c_str());
			bullet.attribute = enemyInfo.attribute;
			bullet.isAppear = false;
			bullet.isRespawn = false;
			enemyInfo.bullets.push_back(bullet);
		}
		m_enemys.push_back(enemyInfo);
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
	for(int enemyNum = 0; enemyNum < m_enemys.size(); enemyNum++){
		// �c�e���Ǘ�
		if(m_enemys[enemyNum].bulletCount == MAX_BULLET){
			m_enemys[enemyNum].bulletCount = 0;
			m_enemys[enemyNum].isShoot = false;
		}

		// ���ˊJ�n�g���K�[
		if(PriShootStartTrigger(enemyNum, info)){ m_enemys[enemyNum].isShoot = true; }

		// �e���ː���(�C���^�[�o��)
		m_enemys[enemyNum].elapsed += info.GetDeltaTime();
		if(m_enemys[enemyNum].elapsed > m_enemys[enemyNum].interval){
			m_enemys[enemyNum].elapsed = 0.0f;

			// ���ˉ\���e���o�����Ă��Ȃ������X�|�[����ԂłȂ��e�Ȃ甭�˂���
			bool isAppear = m_enemys[enemyNum].bullets[m_enemys[enemyNum].bulletCount].isAppear;
			bool isRespawn = m_enemys[enemyNum].bullets[m_enemys[enemyNum].bulletCount].isRespawn;
			if(m_enemys[enemyNum].isShoot && !isAppear && !isRespawn){
				// ���݂̒e��\���\��Ԃɂ���
				m_enemys[enemyNum].bullets[m_enemys[enemyNum].bulletCount].isAppear = true;

				// ���e���U����
				m_enemys[enemyNum].bulletCount++;
			}
		}

		// �ړ��Ǘ�
		for(int count = 0; count < MAX_BULLET; count++){
			if(!m_enemys[enemyNum].bullets[count].isAppear){ continue; }
			bool isInit = false;
			int bulletDx = 0, bulletDy = 0, distance = 0;
			switch(m_enemys[enemyNum].direction)
			{
			case SHOOT_DIRECTION_UP:
				bulletDy = -1;
				distance = m_enemys[enemyNum].posY - m_enemys[enemyNum].respawnPos;
				if(m_enemys[enemyNum].bullets[count].posY < distance){ isInit = !isInit;	}
				break;
			case SHOOT_DIRECTION_RIGHT:
				bulletDx =  1;
				distance = m_enemys[enemyNum].posX + m_enemys[enemyNum].respawnPos;
				if(m_enemys[enemyNum].bullets[count].posX > distance){ isInit = !isInit; }
				break;
			case SHOOT_DIRECTION_DOWN:
				bulletDy =  1;
				distance = m_enemys[enemyNum].posY + m_enemys[enemyNum].respawnPos;
				if(m_enemys[enemyNum].bullets[count].posY > distance){ isInit = !isInit; }
				break;
			case SHOOT_DIRECTION_LEFT:
				bulletDx = -1;
				distance = m_enemys[enemyNum].posX - m_enemys[enemyNum].respawnPos;
				if(m_enemys[enemyNum].bullets[count].posX < distance){ isInit = !isInit; }
				break;
			}
			m_enemys[enemyNum].bullets[count].posX += m_enemys[enemyNum].bullets[count].speed * bulletDx;
			m_enemys[enemyNum].bullets[count].posY += m_enemys[enemyNum].bullets[count].speed * bulletDy;

			// ��񏉊���
			if(!isInit){ continue; }
			m_enemys[enemyNum].bullets[count].isAppear = false;
			m_enemys[enemyNum].bullets[count].isRespawn = false;
			m_enemys[enemyNum].bullets[count].posX = m_enemys[enemyNum].posX;
			m_enemys[enemyNum].bullets[count].posY = m_enemys[enemyNum].posY;
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
	for(int i = 0; i < m_enemys.size(); i++){
		//************************
		// ���菀��(�f�[�^�̏���)
		//************************
		S_ENEMY_INFO enemy = m_enemys[i];
		tnl::Vector3 enemyPos = tnl::Vector3(enemy.posX, enemy.posY, 0);

		//************************
		// �o���A�ƓG�̔���
		//************************

		// �o���A�̍��W�ƃv���C���[�̍��W�͓����Ȃ̂ŗ��p����
		tnl::Vector3 tempPos = currentPos;
		if(isBarrier && !m_enemys[i].isDefeat){
			if(IsIntersectRectToCorrectPosition(tempPos, prevPos, FIXEDENEMY_P_RECT_W, PLAYER_RECT_H, enemyPos, (MAP_CHIP_W << 1), (MAP_CHIP_H << 1)) == CORRECT_UP){
				int judge = PriIsJudgeTrilemma(info.GetBarrierColor(BARRIER_CURRENT), enemy.attribute);
				if(judge == JUDGE_WIN){
					m_enemys[i].isDefeat = true;
					m_defeatCount++;
					m_soundManager->PlaySE(SE_ID_DEFEAT);
				}
			}
		}

		//************************
		// �v���C���[�ƓG�̔���
		//************************
		if(enemy.isDefeat){ continue; }
		int correctType = IsIntersectRectToCorrectPosition(currentPos, prevPos, FIXEDENEMY_P_RECT_W, PLAYER_RECT_H, enemyPos, (MAP_CHIP_W << 1), (MAP_CHIP_H << 1));
		if(correctType != 0){
			info.SetIsCollision(true);
			info.SetCurrentPos(currentPos);
			info.SetCorrectType(ShareInfo::COLLISION_OBJECT_ENMEY, correctType);
		}

		//************************
		// �e�̏Փ˔���(Private)
		//************************
		if(PriBulletCollision(tempPos, m_enemys[i], info) && !info.GetIsPlayerHit()){
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
	for(int i = 0; i < m_enemys.size(); i++){
		S_ENEMY_INFO enemy = m_enemys[i];

		//***************************
		// �G�`��
		//***************************
		if(enemy.isDefeat){ continue; }
		int viewEnemyPosX = enemy.posX - camera->GetPosition().x + screenHalfW;
		int viewEnemyPosY = enemy.posY - camera->GetPosition().y + screenHalfH;
		DrawRotaGraph(viewEnemyPosX, viewEnemyPosY, 2.0f, 0, m_gfxHdl[enemy.imageID], true);

		//***************************
		// �e�`��
		//***************************
		for(int bullet_cnt = 0; bullet_cnt < MAX_BULLET; bullet_cnt++){
			if(!enemy.bullets[bullet_cnt].isAppear || enemy.bullets[bullet_cnt].isRespawn){ continue; }

			int viewBulletPosX = enemy.bullets[bullet_cnt].posX - camera->GetPosition().x + screenHalfW;
			int viewBulletPosY = enemy.bullets[bullet_cnt].posY - camera->GetPosition().y + screenHalfH;
			DrawRotaGraph(viewBulletPosX, viewBulletPosY, 1.0f, 0, m_bulletGfxHdl[enemy.bullets[bullet_cnt].attribute], true);
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
	if(m_enemys[enemyNum].disitance == 0){ return true; }

	// �v���C���[���ǂ̕����ɂ��邩���擾
	tnl::Vector3 currentPos = info.GetCurrentPos();
	tnl::Vector3 enemyPos = tnl::Vector3(m_enemys[enemyNum].posX, m_enemys[enemyNum].posY, 0);
	int direction = tnl::GetRegionPointAndRect(currentPos, enemyPos, (MAP_CHIP_W << 1), (MAP_CHIP_H << 1));
	if(m_enemys[enemyNum].direction == direction){
		// �v���C���[���߂Â��Ă�����e�𔭎ˉ\
		float distance = Utility::GetDistance2D(enemyPos, currentPos);
		if(distance < m_enemys[enemyNum].disitance){ return true; }
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
				int ret = PriIsJudgeTrilemma(barrierColor, attribute);

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

//****************************************************************************
// �֐����FPriEnemyRandom
// �T�@�v�F�G�����_��(Private)
// ���@���F�Ȃ�
// �߂�l�F�Ȃ�
// �ځ@�ׁF����ȓG�̓����_������
//****************************************************************************
void CFixedEnemy::PriEnemyRandom()
{
	srand((unsigned int)time(NULL));

	std::vector<int> scheduleList = {0, 1, 2, 2, 1, 0, 0, 1, 2, 2, 1, 0, 0, 1, 2};

	int i = scheduleList.size() - 1, j = 0;
	while (i > 0) {
		j = rand() % (i + 1);


		int temp = scheduleList[j];
		scheduleList[j] = scheduleList[i];
		scheduleList[i] = temp;

		i--;
	}

	m_scheduleList = scheduleList;
}

//****************************************************************************
// �֐����FPriIsJudgeTrilemma
// �T�@�v�F�O�����ݔ���(Private)
// ���@���F��1�����@���茳
// �@�@�@�@��2�����@�����
// �߂�l�F������(0)�A����(1)�A����(2)
// �ځ@�ׁF�O�����ݔ��菈��
//****************************************************************************
int CFixedEnemy::PriIsJudgeTrilemma(int src, int dst)
{
	int ret = JUDGE_EVEN;
	// ������
	if (src == dst) { return ret; }

	// �ԁF0�A�΁F1�A�F2
	if ((src == ATTRIBUTE_TYPE_RED && dst == ATTRIBUTE_TYPE_GREEN) ||
		(src == ATTRIBUTE_TYPE_GREEN && dst == ATTRIBUTE_TYPE_BLUE) ||
		(src == ATTRIBUTE_TYPE_BLUE && dst == ATTRIBUTE_TYPE_RED))
	{
		ret = JUDGE_WIN;
	}
	else {
		ret = JUDGE_LOSE;
	}

	return ret;
}

//****************************************************************************
// �֐����FPriConvertImageIDToAttribute
// �T�@�v�F�摜ID���瑮���֕ϊ�(Private)
// ���@���F��1�����@���茳
// �@�@�@�@��2�����@�����
// �߂�l�F����(0�F�ԁA1�F�΁A2�F��)
// �ځ@�ׁFTODO:ID�͈�U�K��
//****************************************************************************
int CFixedEnemy::PriConvertImageIDToAttribute(int id)
{
	int attribute = 0;
	switch (id)
	{
	case ENEMY_IMAGE_ID_RED:
		attribute = ATTRIBUTE_TYPE_RED;
		break;
	case ENEMY_IMAGE_ID_GREEN:
		attribute = ATTRIBUTE_TYPE_GREEN;
		break;
	case ENEMY_IMAGE_ID_BLUE:
		attribute = ATTRIBUTE_TYPE_BLUE;
		break;
	case ENEMY_IMAGE_ID_BLACK:
		attribute = m_scheduleList[m_scheduleCount];
		m_scheduleCount++;
		break;
	default:
		break;
	}
	return attribute;
}

} // namespace GameObject