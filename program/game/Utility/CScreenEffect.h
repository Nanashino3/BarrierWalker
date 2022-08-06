#ifndef UTILITY_CUTILITY_H_
#define UTILITY_CUTILITY_H_

namespace Utility
{
class CScreenEffect
{
public:
	static void CreateInstance();
	static void DestroyInstance();
	static CScreenEffect* GetInstance();

	bool FadeIn();
	bool FadeOut();

private:
	static CScreenEffect* m_instance;
	
	bool m_brightInit;
	int m_brightness;

private:
	CScreenEffect();
	~CScreenEffect();

	CScreenEffect(const CScreenEffect&){}
	CScreenEffect& operator=(const CScreenEffect&);
};

} // namespace Utility

#endif // #ifndef UTILITY_CUTILITY_H_