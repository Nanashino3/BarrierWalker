#include "CUtility.h"

#include "../ShareInfo/CONST_GAME_VALUE.h"
#include "../../dxlib_ext/dxlib_ext.h"

namespace{
const int ENEMY_IMAGE_ID_RED	= 106;
const int ENEMY_IMAGE_ID_GREEN	= 10;
const int ENEMY_IMAGE_ID_BLUE	= 34;
const int ENEMY_IMAGE_ID_BLACK	= 181;

const int BRIGHTNESS_MIN = 0;
const int BRIGHTNESS_MAX = 255;

// TODO：とりあえず黒の敵用
// 次回やり方聞いてみる
int count = 0;
int array[15] = {
	0, 1, 2, 0, 1,
	2, 0, 1, 2, 0,
	1, 2, 0, 1, 2,
};

bool brightInit = true;
int brightness = 0;
}

namespace Utility
{
void Initialize()
{
	srand((unsigned int)time(NULL));
	
	count = 0;
	int i = 14, j = 0;
	while(i > 0){
		j = rand() % (i + 1);


		int temp = array[j];
		array[j] = array[i];
		array[i] = temp;

		i--;
	}
}

// 三すくみ判定
// 戻り値：あいこ(0)、勝利(1)、負け(2)
int IsJudgeTrilemma(int src, int dst)
{
	int ret = JUDGE_EVEN;
	// あいこ
	if(src == dst){ return ret; }

	// 赤：0、緑：1、青：2
	if((src == ATTRIBUTE_TYPE_RED   && dst == ATTRIBUTE_TYPE_GREEN) ||
	   (src == ATTRIBUTE_TYPE_GREEN && dst == ATTRIBUTE_TYPE_BLUE ) ||
	   (src == ATTRIBUTE_TYPE_BLUE  && dst == ATTRIBUTE_TYPE_RED  ))
	{
		ret = JUDGE_WIN;
	}else{
		ret = JUDGE_LOSE;
	}

	return ret;
}

// 画像IDから属性へ変換
// TODO:IDは一旦適当
int ConvertImageIDToAttribute(int id)
{
	int attribute = 0;
	switch(id)
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
		attribute = array[count];
		count++;
		break;
	default:
		break;
	}
	return attribute;
}

bool FadeIn()
{
	// 徐々に明るくするため最小輝度で初期化
	if(brightInit){
		brightness = BRIGHTNESS_MIN;
		brightInit = false;
	}
	// フェードインが完了したら輝度を元に戻す
	if(brightness == BRIGHTNESS_MAX){
		brightInit = true;
		return true;
	}

	brightness = (brightness < BRIGHTNESS_MAX + 1) ? brightness += 5 : BRIGHTNESS_MAX;
	SetDrawBright(brightness, brightness, brightness);
	return false;
}

bool FadeOut()
{
	// 徐々に暗くするため最大輝度で初期化
	if(brightInit){
		brightness = BRIGHTNESS_MAX;
		brightInit = false;
	}
	// フェードアウトが完了したら輝度を元に戻す
	if(brightness == BRIGHTNESS_MIN){
		brightInit = true;
		SetDrawBright(BRIGHTNESS_MAX, BRIGHTNESS_MAX, BRIGHTNESS_MAX);
		return true;
	}

	brightness = (brightness > BRIGHTNESS_MIN) ? brightness -= 5 : BRIGHTNESS_MIN;
	SetDrawBright(brightness, brightness, brightness);
	return false;
}

} // namespace Utility