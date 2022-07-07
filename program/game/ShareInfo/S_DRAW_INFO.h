#ifndef SHAREINFO_S_DRAW_INFO_H_
#define SHAREINFO_S_DRAW_INFO_H_

namespace ShareInfo
{
struct S_DRAW_INFO
{
	int x;			// ��ʏ㒆�S�FX���W
	int y;			// ��ʏ㒆�S�FY���W
	double extRate;	// �������̊g�嗦(1.0�œ��{)
	double angle;	// �`��p�x(���W�A���w��)
	int grHandle;	// �O���t�B�b�N�n���h��
	int transFlag;	// �����x��L���ɂ��邩
};

} // namespace ShareInfo

#endif // #ifndef SHAREINFO_S_DRAW_INFO_H_