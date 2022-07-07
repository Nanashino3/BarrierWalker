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

// TODO�F�Ƃ肠�������̓G�p
// ������������Ă݂�
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

// �O�����ݔ���
// �߂�l�F������(0)�A����(1)�A����(2)
int IsJudgeTrilemma(int src, int dst)
{
	int ret = JUDGE_EVEN;
	// ������
	if(src == dst){ return ret; }

	// �ԁF0�A�΁F1�A�F2
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

// �摜ID���瑮���֕ϊ�
// TODO:ID�͈�U�K��
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
	// ���X�ɖ��邭���邽�ߍŏ��P�x�ŏ�����
	if(brightInit){
		brightness = BRIGHTNESS_MIN;
		brightInit = false;
	}
	// �t�F�[�h�C��������������P�x�����ɖ߂�
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
	// ���X�ɈÂ����邽�ߍő�P�x�ŏ�����
	if(brightInit){
		brightness = BRIGHTNESS_MAX;
		brightInit = false;
	}
	// �t�F�[�h�A�E�g������������P�x�����ɖ߂�
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