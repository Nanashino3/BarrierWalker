#ifndef UI_CUIBARRIER_H_
#define UI_CUIBARRIER_H_

#include "AUi.h"

namespace Ui
{
class CUiBarrier : public AUi
{
public:
	CUiBarrier();
	virtual ~CUiBarrier();

	virtual void Update(ShareInfo::CDocGameInfo&) final;
	virtual void Draw(ShareInfo::CDocGameInfo&) final;

private:
	void PriDrawAnimation(unsigned int, float[], int[]);
	void PriDrawAnimationImpl(int, int, int, int, float, int);

private:
	int m_bg_gfx_hdl;
	int m_offset_x;
	int m_offset_y;
	float m_ratio;
};

} // namespace Ui

#endif // #ifndef UI_CUIBARRIER_H_