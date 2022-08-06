//*************************************************************
// プレイヤーキャラクタークラス
//*************************************************************
#include "CPlayerCharacter.h"
#include "CONST_GAMEOBJECT_VALUE.h"

#define NOMINMAX
#include "../Camera/CCamera2D.h"
#include "../ShareInfo/CDocGameInfo.h"
#include "../Sound/CSoundManager.h"

namespace GameObject
{
CPlayerCharacter::CPlayerCharacter()
: m_posX(0), m_posY(0)
, m_actionType(0)
, m_velocityX(0), m_velocityY(0)
, m_animType(ANIM_TYPE_RIGHT)
, m_animFrame(0)
{
	LoadDivGraph("resource/c1_anim_left.png", SIZE_4, PLAYER_X_NUM, PLAYER_Y_NUM, PLAYER_RECT_W, PLAYER_RECT_H, m_gfxHdl[ANIM_TYPE_LEFT]);
	LoadDivGraph("resource/c1_anim_right.png", SIZE_4, PLAYER_X_NUM, PLAYER_Y_NUM, PLAYER_RECT_W, PLAYER_RECT_H, m_gfxHdl[ANIM_TYPE_RIGHT]);

	m_soundManager = Sound::CSoundManager::GetInstance();
}

CPlayerCharacter::~CPlayerCharacter()
{
	for(int i = 0; i < MAX_ANIM_TYPE; i++){
		for(int j = 0; j < MAX_ANIM_TYPE; j++){
			DeleteGraph(m_gfxHdl[i][j]);
		}
	}
}

//****************************************************************************
// 関数名：Initialize
// 概　要：初期化処理
// 引　数：第1引数　ゲーム情報
// 戻り値：なし
// 詳　細：各メンバの初期化や初期座標の計算等
//****************************************************************************
void CPlayerCharacter::Initialize(ShareInfo::CDocGameInfo& info)
{
	Camera::CCamera2D* camera = info.GetCamera();
	// 初期座標設定
 	m_posX = PLAYER_FIRST_POSX - (info.GetScreenWidth() >> 1);
	m_posY = PLAYER_FIRST_POSY - (info.GetScreenHeight() >> 1);
	tnl::Vector3 pos(m_posX, m_posY, 0);
	info.SetCurrentPos(pos);

	m_actionType = 0;
	m_velocityY = 0.0f;
	m_velocityX = 0.0f;
	m_animType	 = ANIM_TYPE_RIGHT;
	m_animFrame = 0;
}

//****************************************************************************
// 関数名：Move
// 概　要：移動処理
// 引　数：第1引数　現在位置
// 　　　　第2引数　前回位置
// 　　　　第3引数　ゲーム情報
// 戻り値：なし
// 詳　細：プレイヤーの移動制御(左右移動・ダッシュ・ジャンプ)
//****************************************************************************
void CPlayerCharacter::Move(tnl::Vector3& current_pos, tnl::Vector3& prev_pos, ShareInfo::CDocGameInfo& info)
{
	// 移動前の座標を設定
	prev_pos = tnl::Vector3(m_posX, m_posY, 0);

	// 移動入力
	float dx = 0.0f;
	if(tnl::Input::IsKeyDown(eKeys::KB_D)){
		dx =  1.0f;
		m_animType = ANIM_TYPE_RIGHT;
		m_actionType |= IS_MOVING;
	}else if(tnl::Input::IsKeyDown(eKeys::KB_A)){
		dx = -1.0f;
		m_animType = ANIM_TYPE_LEFT;
		m_actionType |= IS_MOVING;
	}else{
		m_actionType &= ~IS_MOVING;
	}
	
	// ダッシュ機能
	if(dx != 0.0f){ PriFunction_Dash(dx); }

	// ジャンプ機能
	PriFunction_Jump();

	// 速度情報を設定
	//info.SetVelocity(ShareInfo::S_VELOCITY_INFO(m_velocityX, m_velocityY));

	// 移動ジャンプ中の場合、移動前座標を現在座標と合わせておく
	// (斜め右上移動した場合、下方向に当たった判定になる)
	if(((m_actionType & IS_MOVING) && (m_actionType & IS_JUMPING))){
		prev_pos.y = m_posY;
	}

	// 最終座標を設定する
	current_pos = tnl::Vector3(m_posX, m_posY, 0);
}

//****************************************************************************
// 関数名：Update
// 概　要：更新処理
// 引　数：第1引数　ゲーム情報
// 戻り値：なし
// 詳　細：移動範囲制御やオブジェクトとの衝突判定後の更新処理等
//****************************************************************************
void CPlayerCharacter::Update(ShareInfo::CDocGameInfo& info)
{
	// アニメーションのフレーム数を決定
	static float anim_time_count;
	anim_time_count += info.GetDeltaTime();
	if(anim_time_count > 0.25f){
		anim_time_count = 0;
		m_animFrame++;
		m_animFrame %= MAX_ANIM_TYPE;
	}

	// 当たり判定の結果から必要な処理を行う
	if(info.GetIsCollision()){
		// TODO：増えてきたら配列で持ちたい
		int stageCorrect = info.GetCorrectType(ShareInfo::COLLISION_OBJECT_STAGE);
		int enemyCorrect = info.GetCorrectType(ShareInfo::COLLISION_OBJECT_ENMEY);
		int floorCorrect = info.GetCorrectType(ShareInfo::COLLISION_OBJECT_MOVE_FLOOR);

		bool canInitByStage = (stageCorrect == CORRECT_UP || stageCorrect == CORRECT_DOWN) ? true : false;
		bool canInitByEnemy = (enemyCorrect == CORRECT_UP || enemyCorrect == CORRECT_DOWN) ? true : false;
		bool canInitByFloor = (floorCorrect == CORRECT_UP) ? true : false;

		if(canInitByStage || canInitByEnemy || canInitByFloor){
			m_velocityY = 0.0f;
			m_actionType &= ~IS_JUMPING;
		}

		m_posX = info.GetCurrentPos().x;
		m_posY = info.GetCurrentPos().y;
	}
	info.SetActionType(m_actionType);
	PriMoveConstrain(info);
	PriGameInfoClear(info);
}

//****************************************************************************
// 関数名：Draw
// 概　要：描画処理
// 引　数：第1引数　ゲーム情報
// 戻り値：なし
// 詳　細：プレイヤーの描画処理
//****************************************************************************
void CPlayerCharacter::Draw(ShareInfo::CDocGameInfo& info)
{
	Camera::CCamera2D* camera = info.GetCamera();
	int screen_half_w = info.GetScreenWidth() >> 1;
	int screen_half_h = info.GetScreenHeight() >> 1;

	int view_playerPos_x = m_posX - camera->GetPosition().x + screen_half_w;
	int view_playerPos_y = m_posY - camera->GetPosition().y + screen_half_h;
	DrawRotaGraph(view_playerPos_x, view_playerPos_y, 1.0f, 0, m_gfxHdl[m_animType][m_animFrame], true);
}

//****************************************************************************
// 関数名：PriFunction_Dash
// 概　要：ダッシュ機能(Private)
// 引　数：第1引数　方向を示すための符号
// 戻り値：なし
// 詳　細：ダッシュ処理
//****************************************************************************
void CPlayerCharacter::PriFunction_Dash(float dx)
{
	if(tnl::Input::IsKeyDown(eKeys::KB_LSHIFT)){
		m_velocityX += PLAYER_DASH_ACCEL;
		m_actionType |= IS_DASHING;
	}else{
		m_velocityX -= PLAYER_DASH_ACCEL;
		m_actionType &= ~IS_DASHING;
	}
	if(m_velocityX < 0.0f){ m_velocityX = 0.0f; }
	if(m_velocityX > PLAYER_DASH_SPEED){ m_velocityX = PLAYER_DASH_SPEED; }

	m_posX += (PLAYER_SPEED + m_velocityX) * dx;
}

//****************************************************************************
// 関数名：PriFunction_Jump
// 概　要：ジャンプ処理(Private)
// 引　数：なし
// 戻り値：なし
// 詳　細：ジャンプ処理
//****************************************************************************
void CPlayerCharacter::PriFunction_Jump()
{
	// ジャンプ処理
	if(tnl::Input::IsKeyDownTrigger(eKeys::KB_SPACE) && !(m_actionType & IS_JUMPING) && !(m_actionType & IS_AIR)){
		m_velocityY = PLAYER_JUMP_SPEED;
		m_actionType |= IS_JUMPING;
		m_soundManager->PlaySE(SE_ID_JUMP);
	}

	// 重力処理
	m_posY += m_velocityY;
	m_velocityY += PLAYER_JUMP_ACCEL;
	m_velocityY = std::min(30.0f, std::max(-30.0f, m_velocityY));

	// 空中判定
	if((int)m_velocityY != 0){
		m_actionType |= IS_AIR;
	}else{
		if(!(m_actionType & IS_JUMPING)){ m_actionType &= ~IS_AIR; }
	}
}

//****************************************************************************
// 関数名：PriMoveConstrain
// 概　要：移動制約(Private)
// 引　数：第1引数　ゲーム情報
// 戻り値：なし
// 詳　細：プレイヤ－の移動範囲を制御する
//****************************************************************************
void CPlayerCharacter::PriMoveConstrain(ShareInfo::CDocGameInfo& info)
{
	tnl::Vector3 camera_pos = info.GetCamera()->GetPosition();
	int screen_half_w = info.GetScreenWidth() >> 1;
	int screen_half_h = info.GetScreenHeight() >> 1;
	int rect_half_w = PLAYER_RECT_W >> 1;
	int rect_half_h = PLAYER_RECT_H >> 1;

	// X軸方向
	if(m_posX < camera_pos.x - screen_half_w + rect_half_w){
		m_posX = camera_pos.x - screen_half_w + rect_half_w;
	}
	else if(m_posX > info.GetScreenWidth() * info.GetScreenMax() - (screen_half_w + rect_half_w)){
		m_posX = info.GetScreenWidth() * info.GetScreenMax() - (screen_half_w + rect_half_w);
	}
}

//****************************************************************************
// 関数名：PriGameInfoClear
// 概　要：ゲーム情報を初期化する(Private)
// 引　数：第1引数　ゲーム情報
// 戻り値：なし
// 詳　細：プレイヤー更新後衝突判定のフラグなどを初期化するための関数
// 　　　　※ただ、プレイヤークラスの責務ではないのでどっかのタイミングで切り離す
//****************************************************************************
void CPlayerCharacter::PriGameInfoClear(ShareInfo::CDocGameInfo& info)
{
	info.SetIsCollision(false);
	for(int i = 0; i < ShareInfo::MAX_COLLISION_OBJECT_TYPE; i++){
		info.SetCorrectType(static_cast<ShareInfo::E_COLLISION_OBJECT_TYPE>(i), NO_HIT);
	}
	tnl::Vector3 init(m_posX, m_posY, 0);
	info.SetCurrentPos(init);
}

} // namespace GameObject