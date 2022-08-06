#include "CScreenEffect.h"

#include "CONST_UTILITY_VALUE.h"
#include "../../dxlib_ext/dxlib_ext.h"

namespace Utility
{
CScreenEffect* CScreenEffect::m_instance = nullptr;
CScreenEffect::CScreenEffect()
: m_brightInit(true)
, m_brightness()
{}

CScreenEffect::~CScreenEffect()
{}

void CScreenEffect::CreateInstance()
{
	m_instance = new CScreenEffect();
}

void CScreenEffect::DestroyInstance()
{
	delete m_instance;
}

CScreenEffect* CScreenEffect::GetInstance()
{
	if(m_instance == nullptr){
		CreateInstance();
	}

	return m_instance;
}

bool CScreenEffect::FadeIn()
{
	// 徐々に明るくするため最小輝度で初期化
	if(m_brightInit){
		m_brightness = BRIGHTNESS_MIN;
		m_brightInit = false;
	}
	// フェードインが完了したら輝度を元に戻す
	if(m_brightness == BRIGHTNESS_MAX){
		m_brightInit = true;
		return true;
	}

	m_brightness = (m_brightness < BRIGHTNESS_MAX + 1) ? m_brightness += 5 : BRIGHTNESS_MAX;
	SetDrawBright(m_brightness, m_brightness, m_brightness);
	return false;
}

bool CScreenEffect::FadeOut()
{
	// 徐々に暗くするため最大輝度で初期化
	if(m_brightInit){
		m_brightness = BRIGHTNESS_MAX;
		m_brightInit = false;
	}
	// フェードアウトが完了したら輝度を元に戻す
	if(m_brightness == BRIGHTNESS_MIN){
		m_brightInit = true;
		SetDrawBright(BRIGHTNESS_MAX, BRIGHTNESS_MAX, BRIGHTNESS_MAX);
		return true;
	}

	m_brightness = (m_brightness > BRIGHTNESS_MIN) ? m_brightness -= 5 : BRIGHTNESS_MIN;
	SetDrawBright(m_brightness, m_brightness, m_brightness);
	return false;
}

} // namespace Utility