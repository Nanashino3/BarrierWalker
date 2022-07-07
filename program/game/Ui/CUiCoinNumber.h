#ifndef UI_CUICOINNUMBER_H_
#define UI_CUICOINNUMBER_H_

#include "AUi.h"

namespace Ui
{
class CUiCoinNumber : public AUi
{
public:
	CUiCoinNumber();
	virtual ~CUiCoinNumber();

	virtual void Initialize(ShareInfo::CDocGameInfo&) final;
	virtual void Update(ShareInfo::CDocGameInfo&) final;
	virtual void Draw(ShareInfo::CDocGameInfo&) final;

private:
	unsigned int m_total_num;
	unsigned int m_coin_num;
};

} // namespace Ui

#endif // #ifndef UI_CUICOINNUMBER_H_