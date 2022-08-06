#ifndef UI_CUITRILEMMA_H_
#define UI_CUITRILEMMA_H_

#include <vector>
#include "AUi.h"
#include "CONST_UI_VALUE.h"

namespace Ui
{
class CUiTrilemma : public AUi
{
public:
	CUiTrilemma();
	virtual ~CUiTrilemma();

	virtual void Draw(ShareInfo::CDocGameInfo&) final;

private:
	std::vector<int> m_attributeGfxHdl;
	int m_iconGfxHdl[MAX_ICON_IMAGE_NUM];
};

} // namespace Ui

#endif // #ifndef UI_CUITRILEMMA_H_