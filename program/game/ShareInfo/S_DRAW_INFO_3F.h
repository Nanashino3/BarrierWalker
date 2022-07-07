#ifndef SHAREINFO_S_DRAW_INFO_3F_H_
#define SHAREINFO_S_DRAW_INFO_3F_H_

namespace ShareInfo
{
struct S_DRAW_INFO_3F
{
	float xf;		 // ��ʏ㒆�S�FX���W
	float yf;		 // ��ʏ㒆�S�FY���W
	float cxf;		 // �摜�㒆�S�FX���W
	float cyf;		 // �摜�㒆�S�FY���W
	double extRateX; // �������̊g�嗦(1.0�œ��{)
	double extRateY; // �c�����̊g�嗦(1.0�œ��{)
	double angle;	 // �`��p�x(���W�A���w��)
	int grHandle;	 // �O���t�B�b�N�n���h��
	int transFlag;	 // �����x��L���ɂ��邩
};

} // namespace ShareInfo

#endif // #ifndef SHAREINFO_S_DRAW_INFO_3F_H_