#ifndef GAMEOBJECT_CONST_GAMEOBJECT_VALUE_H_
#define GAMEOBJECT_CONST_GAMEOBJECT_VALUE_H_

namespace GameObject
{
//****************************************************************************
// 移動オブジェクト関連定数
const unsigned int MOVE_OBJ_RECTW = 64;
const unsigned int MOVE_OBJ_RECTH = 24;

const int MOVE_UP     = 0;
const int MOVE_RIGHT  = 1;
const int MOVE_BOTTOM = 2;
const int MOVE_LEFT   = 3;
//****************************************************************************

//****************************************************************************
// プレイヤー関連定数
const float PLAYER_SPEED = 2.0f;

// ジャンプ関連
const float PLAYER_JUMP_SPEED = -6.5f;
const float PLAYER_JUMP_ACCEL = 0.2f;

// ダッシュ関連
const float PLAYER_DASH_SPEED	= 1.0f;
const float PLAYER_DASH_ACCEL	= 0.1f;

// 初期座標(スクリーン座標)
const float PLAYER_FIRST_POSX = 75.f;
const float PLAYER_FIRST_POSY = 551.f;
//****************************************************************************

//****************************************************************************
// 固定的関連定数
// 弾：弾数
const int MAX_BULLET = 10;
// 矩形サイズ
const int FIXEDENEMY_P_RECT_W = 24;
//****************************************************************************
}

#endif // #ifndef GAMEOBJECT_CONST_GAMEOBJECT_VALUE_H_