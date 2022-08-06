//*************************************************************
// 固定敵クラス
// TODO：敵が弾を内部的に保持しているのはきついので
// 　　　どっかのタイミングで切り離す

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
// 弾画像
const std::vector<std::string> g_bulletImage = {
	"resource/sticon1b-3.png",	// 赤
	"resource/sticon1d-3.png",	// 緑
	"resource/sticon1c-3.png"	// 青
};
// 弾：弾数
const int MAX_BULLET = 10;
// 矩形サイズ
const int P_RECT_W = 24;
const int P_RECT_H = PLAYER_RECT_H;
const int RECT_W = MAP_CHIP_W << 1;
const int RECT_H = MAP_CHIP_H << 1;
// 弾：打ち出す方向
const int SHOOT_UP		= 0;
const int SHOOT_RIGHT	= 1;
const int SHOOT_BOTTOM	= 2;
const int SHOOT_LEFT	= 3;

// 各種グラフィックスハンドル
int g_gfxHdl[MAX_MAP_CHIP] = {0};
int g_bulletGfxHdl[MAX_ATTRIBUTE_TYPE] = {0};

// 敵キャラ情報
std::vector<GameObject::S_ENEMY_INFO> g_enemys;
// 敵キャラCSV読込用
std::vector<std::vector<std::string>> g_enemyDetas;
}

namespace GameObject
{
CFixedEnemy::CFixedEnemy()
: m_totalDefeat(0)
, m_defeatCount(0)
{
	// 敵キャラ画像読込
	LoadDivGraph("resource/block_ds.png", MAX_MAP_CHIP, MAP_CHIP_X_NUM, MAP_CHIP_Y_NUM, MAP_CHIP_W, MAP_CHIP_H, g_gfxHdl);
	for(int i = 0; i < g_bulletImage.size(); i++){
		// 弾画像読込
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
// 関数名：Initialize
// 概　要：初期化処理
// 引　数：第1引数　ゲーム情報
// 戻り値：なし
// 詳　細：敵情報(CSV)の読込やデータ設定
//****************************************************************************
void CFixedEnemy::Initialize(ShareInfo::CDocGameInfo& info)
{
	// 要素の初期化とメモリサイズを合わせる
	g_enemyDetas.clear();
	g_enemyDetas.shrink_to_fit();
	for(int i = 0; i < g_enemys.size(); i++){
		g_enemys[i].bullets.clear();
		g_enemys[i].bullets.shrink_to_fit();
	}
	g_enemys.clear();
	g_enemys.shrink_to_fit();

	// 総撃破数
	m_totalDefeat = info.GetScoreCalcSrcInfo(ACCESS_KEY_DEFEAT);

	// 敵CSV読込
	int index = info.GetStageIndex();
	g_enemyDetas = tnl::LoadCsv(g_enemyCsv[index].c_str());

	Camera::CCamera2D* camera = info.GetCamera();

	// TODO：黒敵の場合ランダムで属性を取得する必要があるので一旦初期化する
	Utility::Initialize();
	for(int i = 0; i < g_enemyDetas.size(); i++){
		// CSVの内容で初期化
		S_ENEMY_INFO enemyInfo;
		enemyInfo.imageID = atoi(g_enemyDetas[i][0].c_str());
		enemyInfo.attribute = Utility::ConvertImageIDToAttribute(enemyInfo.imageID);
		enemyInfo.direction = atoi(g_enemyDetas[i][1].c_str());
		enemyInfo.interval = atof(g_enemyDetas[i][3].c_str());
		enemyInfo.disitance = atof(g_enemyDetas[i][4].c_str());
		enemyInfo.respawnPos = atof(g_enemyDetas[i][5].c_str());
		enemyInfo.posX = atof(g_enemyDetas[i][6].c_str()) - (info.GetScreenWidth() >> 1);
		enemyInfo.posY = atof(g_enemyDetas[i][7].c_str()) - (info.GetScreenHeight() >> 1);

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
	for(int enemyNum = 0; enemyNum < g_enemys.size(); enemyNum++){
		// 残弾数管理
		if(g_enemys[enemyNum].bulletCount == MAX_BULLET){
			g_enemys[enemyNum].bulletCount = 0;
			g_enemys[enemyNum].isShoot = false;
		}

		// 発射開始トリガー
		if(PriShootStartTrigger(enemyNum, info)){ g_enemys[enemyNum].isShoot = true; }

		// 弾発射制御(インターバル)
		g_enemys[enemyNum].elapsed += info.GetDeltaTime();
		if(g_enemys[enemyNum].elapsed > g_enemys[enemyNum].interval){
			g_enemys[enemyNum].elapsed = 0.0f;

			// 発射可能かつ弾が出現していないかつリスポーン状態でない弾なら発射する
			bool isAppear = g_enemys[enemyNum].bullets[g_enemys[enemyNum].bulletCount].isAppear;
			bool isRespawn = g_enemys[enemyNum].bullets[g_enemys[enemyNum].bulletCount].isRespawn;
			if(g_enemys[enemyNum].isShoot && !isAppear && !isRespawn){
				// 現在の弾を表示可能状態にする
				g_enemys[enemyNum].bullets[g_enemys[enemyNum].bulletCount].isAppear = true;

				// 時弾装填準備
				g_enemys[enemyNum].bulletCount++;
			}
		}

		// 移動管理
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

			// 情報初期化
			if(!isInit){ continue; }
			g_enemys[enemyNum].bullets[count].isAppear = false;
			g_enemys[enemyNum].bullets[count].isRespawn = false;
			g_enemys[enemyNum].bullets[count].posX = g_enemys[enemyNum].posX;
			g_enemys[enemyNum].bullets[count].posY = g_enemys[enemyNum].posY;
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
	for(int i = 0; i < g_enemys.size(); i++){
		//************************
		// 判定準備(データの準備)
		//************************
		S_ENEMY_INFO enemy = g_enemys[i];
		tnl::Vector3 enemyPos = tnl::Vector3(enemy.posX, enemy.posY, 0);

		//************************
		// バリアと敵の判定
		//************************

		// バリアの座標とプレイヤーの座標は同じなので流用する
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
		// プレイヤーと敵の判定
		//************************
		if(enemy.isDefeat){ continue; }
		int correctType = IsIntersectRectToCorrectPosition(currentPos, prevPos, P_RECT_W, P_RECT_H, enemyPos, RECT_W, RECT_H);
		if(correctType != 0){
			info.SetIsCollision(true);
			info.SetCurrentPos(currentPos);
			info.SetCorrectType(ShareInfo::COLLISION_OBJECT_ENMEY, correctType);
		}

		//************************
		// 弾の衝突判定(Private)
		//************************
		if(PriBulletCollision(tempPos, g_enemys[i], info) && !info.GetIsPlayerHit()){
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
	for(int i = 0; i < g_enemys.size(); i++){
		S_ENEMY_INFO enemy = g_enemys[i];

		//***************************
		// 敵描画
		//***************************
		if(enemy.isDefeat){ continue; }
		int viewEnemyPosX = enemy.posX - camera->GetPosition().x + screenHalfW;
		int viewEnemyPosY = enemy.posY - camera->GetPosition().y + screenHalfH;
		DrawRotaGraph(viewEnemyPosX, viewEnemyPosY, 2.0f, 0, g_gfxHdl[enemy.imageID], true);

		//***************************
		// 弾描画
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
	if(g_enemys[enemyNum].disitance == 0){ return true; }

	// プレイヤーがどの方向にいるかを取得
	tnl::Vector3 currentPos = info.GetCurrentPos();
	tnl::Vector3 enemyPos = tnl::Vector3(g_enemys[enemyNum].posX, g_enemys[enemyNum].posY, 0);
	int direction = tnl::GetRegionPointAndRect(currentPos, enemyPos, RECT_W, RECT_H);
	if(g_enemys[enemyNum].direction == direction){
		// プレイヤーが近づいてきたら弾を発射可能
		float distance = Utility::GetDistance2D(enemyPos, currentPos);
		if(distance < g_enemys[enemyNum].disitance){ return true; }
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
				int ret = Utility::IsJudgeTrilemma(barrierColor, attribute);

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

} // namespace GameObject