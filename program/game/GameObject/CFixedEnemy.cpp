//*************************************************************
// 固定敵クラス
// TODO：敵が弾を内部的に保持しているのはきついので
// 　　　どっかのタイミングで切り離す
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
	// 敵キャラ画像読込
	LoadDivGraph("resource/block_ds.png", MAX_MAP_CHIP, MAP_CHIP_X_NUM, MAP_CHIP_Y_NUM, MAP_CHIP_W, MAP_CHIP_H, m_gfxHdl);
	
	// 弾画像読込
	std::vector<std::string> bulletImgList = {
		"resource/sticon1b-3.png",	// 赤
		"resource/sticon1d-3.png",	// 緑
		"resource/sticon1c-3.png"	// 青
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
// 関数名：Initialize
// 概　要：初期化処理
// 引　数：第1引数　ゲーム情報
// 戻り値：なし
// 詳　細：敵情報(CSV)の読込やデータ設定
//****************************************************************************
void CFixedEnemy::Initialize(ShareInfo::CDocGameInfo& info)
{
	// 要素の初期化とメモリサイズを合わせる
	for(int i = 0; i < m_enemys.size(); i++){
		m_enemys[i].bullets.clear();
		m_enemys[i].bullets.shrink_to_fit();
	}
	m_enemys.clear();
	m_enemys.shrink_to_fit();

	// 総撃破数
	m_totalDefeat = info.GetScoreCalcSrcInfo(ACCESS_KEY_DEFEAT);

	// 敵CSV読込
	int index = info.GetStageIndex();
	std::string fileList[] = {
		"resource/enemy/enemy_layout_1.csv",
		"resource/enemy/enemy_layout_2.csv",
		"resource/enemy/enemy_layout_3.csv"
	};
	std::vector<std::vector<std::string>> enemyDetas = tnl::LoadCsv(fileList[index]);

	Camera::CCamera2D* camera = info.GetCamera();

	// TODO：黒敵の場合ランダムで属性を取得する必要があるので一旦初期化する
	PriEnemyRandom();
	for(int i = 0; i < enemyDetas.size(); i++){
		// CSVの内容で初期化
		S_ENEMY_INFO enemyInfo;
		enemyInfo.imageID = atoi(enemyDetas[i][0].c_str());
		enemyInfo.attribute = PriConvertImageIDToAttribute(enemyInfo.imageID);
		enemyInfo.direction = atoi(enemyDetas[i][1].c_str());
		enemyInfo.interval = atof(enemyDetas[i][3].c_str());
		enemyInfo.disitance = atof(enemyDetas[i][4].c_str());
		enemyInfo.respawnPos = atof(enemyDetas[i][5].c_str());
		enemyInfo.posX = atof(enemyDetas[i][6].c_str()) - (info.GetScreenWidth() >> 1);
		enemyInfo.posY = atof(enemyDetas[i][7].c_str()) - (info.GetScreenHeight() >> 1);

		// 固定値で初期化
		enemyInfo.isDefeat = false;
		enemyInfo.isShoot = false;
		enemyInfo.elapsed = 0.0f;
		enemyInfo.bulletCount = 0;

		// 弾関連初期化(TODO：時間がないので敵クラス内でもうやる)
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
// 関数名：Update
// 概　要：更新処理
// 引　数：第1引数　ゲーム情報
// 戻り値：なし
// 詳　細：特になし(現状弾の更新処理になっているので切り離したい)
//****************************************************************************
void CFixedEnemy::Update(ShareInfo::CDocGameInfo& info)
{
	//**************************
	// 弾関連処理
	//**************************
	for(int enemyNum = 0; enemyNum < m_enemys.size(); enemyNum++){
		// 残弾数管理
		if(m_enemys[enemyNum].bulletCount == MAX_BULLET){
			m_enemys[enemyNum].bulletCount = 0;
			m_enemys[enemyNum].isShoot = false;
		}

		// 発射開始トリガー
		if(PriShootStartTrigger(enemyNum, info)){ m_enemys[enemyNum].isShoot = true; }

		// 弾発射制御(インターバル)
		m_enemys[enemyNum].elapsed += info.GetDeltaTime();
		if(m_enemys[enemyNum].elapsed > m_enemys[enemyNum].interval){
			m_enemys[enemyNum].elapsed = 0.0f;

			// 発射可能かつ弾が出現していないかつリスポーン状態でない弾なら発射する
			bool isAppear = m_enemys[enemyNum].bullets[m_enemys[enemyNum].bulletCount].isAppear;
			bool isRespawn = m_enemys[enemyNum].bullets[m_enemys[enemyNum].bulletCount].isRespawn;
			if(m_enemys[enemyNum].isShoot && !isAppear && !isRespawn){
				// 現在の弾を表示可能状態にする
				m_enemys[enemyNum].bullets[m_enemys[enemyNum].bulletCount].isAppear = true;

				// 時弾装填準備
				m_enemys[enemyNum].bulletCount++;
			}
		}

		// 移動管理
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

			// 情報初期化
			if(!isInit){ continue; }
			m_enemys[enemyNum].bullets[count].isAppear = false;
			m_enemys[enemyNum].bullets[count].isRespawn = false;
			m_enemys[enemyNum].bullets[count].posX = m_enemys[enemyNum].posX;
			m_enemys[enemyNum].bullets[count].posY = m_enemys[enemyNum].posY;
		}
	}

	// 撃破数を更新
	if(info.GetIsClear()){ info.SetScoreCalcSrcInfo(ACCESS_KEY_DEFEAT, m_totalDefeat + m_defeatCount); }
}

//****************************************************************************
// 関数名：Collision
// 概　要：衝突判定処理
// 引　数：第1引数　現在位置
// 　　　　第2引数　前回位置
// 　　　　第3引数　ゲーム情報
// 戻り値：なし
// 詳　細：敵(弾)の衝突判定処理
//****************************************************************************
void CFixedEnemy::Collision(tnl::Vector3& currentPos, tnl::Vector3& prevPos, ShareInfo::CDocGameInfo& info)
{
	// バリアが起動しているか
	unsigned int functionType = info.GetFunctionType();
	bool isBarrier = functionType & FUNCTION_TYPE_BARRIER;

	info.SetIsPlayerHit(false);
	for(int i = 0; i < m_enemys.size(); i++){
		//************************
		// 判定準備(データの準備)
		//************************
		S_ENEMY_INFO enemy = m_enemys[i];
		tnl::Vector3 enemyPos = tnl::Vector3(enemy.posX, enemy.posY, 0);

		//************************
		// バリアと敵の判定
		//************************

		// バリアの座標とプレイヤーの座標は同じなので流用する
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
		// プレイヤーと敵の判定
		//************************
		if(enemy.isDefeat){ continue; }
		int correctType = IsIntersectRectToCorrectPosition(currentPos, prevPos, FIXEDENEMY_P_RECT_W, PLAYER_RECT_H, enemyPos, (MAP_CHIP_W << 1), (MAP_CHIP_H << 1));
		if(correctType != 0){
			info.SetIsCollision(true);
			info.SetCurrentPos(currentPos);
			info.SetCorrectType(ShareInfo::COLLISION_OBJECT_ENMEY, correctType);
		}

		//************************
		// 弾の衝突判定(Private)
		//************************
		if(PriBulletCollision(tempPos, m_enemys[i], info) && !info.GetIsPlayerHit()){
			info.SetIsPlayerHit(true);
		}
	}
}

//****************************************************************************
// 関数名：Draw
// 概　要：描画処理
// 引　数：第1引数　ゲーム情報
// 戻り値：なし
// 詳　細：敵(弾)の描画処理
//****************************************************************************
void CFixedEnemy::Draw(ShareInfo::CDocGameInfo& info)
{
	Camera::CCamera2D* camera = info.GetCamera();
	int screenHalfW = info.GetScreenWidth() >> 1;
	int screenHalfH = info.GetScreenHeight() >> 1;
	for(int i = 0; i < m_enemys.size(); i++){
		S_ENEMY_INFO enemy = m_enemys[i];

		//***************************
		// 敵描画
		//***************************
		if(enemy.isDefeat){ continue; }
		int viewEnemyPosX = enemy.posX - camera->GetPosition().x + screenHalfW;
		int viewEnemyPosY = enemy.posY - camera->GetPosition().y + screenHalfH;
		DrawRotaGraph(viewEnemyPosX, viewEnemyPosY, 2.0f, 0, m_gfxHdl[enemy.imageID], true);

		//***************************
		// 弾描画
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
// 関数名：PriShootStartTrigger
// 概　要：発射開始トリガー
// 引　数：第1引数　敵番号
// 　　　　第2引数　ゲーム情報
// 戻り値：なし
// 詳　細：敵が弾を発射することが可能かを判断する
//****************************************************************************
bool CFixedEnemy::PriShootStartTrigger(int enemyNum, ShareInfo::CDocGameInfo& info)
{
	// 常に発射する
	if(m_enemys[enemyNum].disitance == 0){ return true; }

	// プレイヤーがどの方向にいるかを取得
	tnl::Vector3 currentPos = info.GetCurrentPos();
	tnl::Vector3 enemyPos = tnl::Vector3(m_enemys[enemyNum].posX, m_enemys[enemyNum].posY, 0);
	int direction = tnl::GetRegionPointAndRect(currentPos, enemyPos, (MAP_CHIP_W << 1), (MAP_CHIP_H << 1));
	if(m_enemys[enemyNum].direction == direction){
		// プレイヤーが近づいてきたら弾を発射可能
		float distance = Utility::GetDistance2D(enemyPos, currentPos);
		if(distance < m_enemys[enemyNum].disitance){ return true; }
	}

	return false;
}

//****************************************************************************
// 関数名：PriBulletCollision
// 概　要：弾衝突判定(Private)
// 引　数：第1引数　現在位置
// 　　　　第2引数　敵情報
// 　　　　第3引数　ゲーム情報
// 戻り値：なし
// 詳　細：弾の衝突判定処理(三すくみ判定も実施)
//****************************************************************************
bool CFixedEnemy::PriBulletCollision(tnl::Vector3& current_pos, S_ENEMY_INFO& enemy_info, ShareInfo::CDocGameInfo& info)
{
	unsigned int functionType = info.GetFunctionType();
	bool isBarrier = functionType & FUNCTION_TYPE_BARRIER;

	int isPlayerHit = false;
	for(int count = 0; count < MAX_BULLET; count++){
		
		if(!enemy_info.bullets[count].isAppear || enemy_info.bullets[count].isRespawn){ continue; }

		// 矩形A(自機とバリア)
		int left_a	 = current_pos.x - 4;
		int right_a  = current_pos.x + 4;
		int top_a	 = current_pos.y - 48 / 2;
		int bottom_a = current_pos.y + 48 / 2;

		// 矩形B(弾)
		float bulletPosX = enemy_info.bullets[count].posX;
		float bulletPosY = enemy_info.bullets[count].posY;
		tnl::Vector3 bullet_pos = tnl::Vector3(bulletPosX, bulletPosY, 0);

		int left_b	 = bulletPosX - 24 / 2;
		int right_b	 = bulletPosX + 24 / 2;
		int top_b	 = bulletPosY - 24 / 2;
		int bottom_b = bulletPosY + 24 / 2;

		// 補正なし衝突判定
		if(tnl::IsIntersectRectPrimitive(left_a, right_a, top_a, bottom_a, left_b, right_b, top_b, bottom_b)){
			isPlayerHit = true;
			if(isBarrier){
				// バリアがある場合は属性を確認
				int barrierColor = info.GetBarrierColor(BARRIER_CURRENT);
				int attribute = enemy_info.bullets[count].attribute;
				
				// 三すくみ判定
				int ret = PriIsJudgeTrilemma(barrierColor, attribute);

				// 必要なフラグを操作する
				bool isInit = (ret == JUDGE_EVEN || ret == JUDGE_WIN) ? true : false;
				isPlayerHit = (ret == JUDGE_LOSE) ? true : false;

				// 弾関連の初期化
				if(!isInit){ continue; }
				// リスポーンフラグを立てる(至近距離対策)
				// ある位置まで透明の弾が移動する形にする
				enemy_info.bullets[count].isRespawn = true;

				// 弾ガード音
				m_soundManager->PlaySE(SE_ID_GUARD);
			}
		}
	}

	return isPlayerHit;
}

//****************************************************************************
// 関数名：PriEnemyRandom
// 概　要：敵ランダム(Private)
// 引　数：なし
// 戻り値：なし
// 詳　細：特殊な敵はランダム生成
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
// 関数名：PriIsJudgeTrilemma
// 概　要：三すくみ判定(Private)
// 引　数：第1引数　判定元
// 　　　　第2引数　判定先
// 戻り値：あいこ(0)、勝利(1)、負け(2)
// 詳　細：三すくみ判定処理
//****************************************************************************
int CFixedEnemy::PriIsJudgeTrilemma(int src, int dst)
{
	int ret = JUDGE_EVEN;
	// あいこ
	if (src == dst) { return ret; }

	// 赤：0、緑：1、青：2
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
// 関数名：PriConvertImageIDToAttribute
// 概　要：画像IDから属性へ変換(Private)
// 引　数：第1引数　判定元
// 　　　　第2引数　判定先
// 戻り値：属性(0：赤、1：緑、2：青)
// 詳　細：TODO:IDは一旦適当
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