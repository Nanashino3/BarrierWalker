#ifndef UI_CUITRILEMMA_H_
#define UI_CUITRILEMMA_H_

#include "AUi.h"

namespace Ui
{
class CUiTrilemma : public AUi
{
public:
	CUiTrilemma();
	virtual ~CUiTrilemma();

	virtual void Draw(ShareInfo::CDocGameInfo&) final;
};

} // namespace Ui

#endif // #ifndef UI_CUITRILEMMA_H_