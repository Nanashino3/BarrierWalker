#ifndef UI_CUITIMELIMIT_H_
#define UI_CUITIMELIMIT_H_

#include <vector>
#include "AUi.h"

namespace Ui
{
class CUiTimeLimit : public AUi
{
public:
	CUiTimeLimit();
	virtual ~CUiTimeLimit();

	virtual void Initialize(ShareInfo::CDocGameInfo&) final;
	virtual void Update(ShareInfo::CDocGameInfo&) final;
	virtual void Draw(ShareInfo::CDocGameInfo&) final;

private:
	float m_elapsed;
	int m_timeLimit;
	std::vector<int> m_timeLimits;
};

} // namespace Ui

#endif // #ifndef UI_CUITIMELIMIT_H_