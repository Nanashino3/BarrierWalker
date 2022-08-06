#ifndef UI_CUIBARRIER_H_
#define UI_CUIBARRIER_H_

#include <vector>
#include <string>

#include "AUi.h"
#include "../ShareInfo/S_DRAW_INFO.h"
#include "../ShareInfo/S_DRAW_INFO_3F.h"
#include "../ShareInfo/CONST_GAME_VALUE.h"

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
	int m_bgGfxHdl;
	int m_offsetX, m_offsetY;
	float m_ratio;

	std::vector<std::string> m_fileList;
	std::vector<ShareInfo::S_DRAW_INFO> m_bgUiList;
	std::vector<ShareInfo::S_DRAW_INFO_3F> m_gaugeList;
	std::vector<int> m_gfxHdList;
};

} // namespace Ui

#endif // #ifndef UI_CUIBARRIER_H_