#ifndef FUNCTION_CSTAGECONTROL_H_
#define FUNCTION_CSTAGECONTROL_H_

#include "AFunction.h"

namespace Function
{
class CStageControl : public AFunction
{
public:
	CStageControl(unsigned int type = 0);
	virtual ~CStageControl();

	virtual void Update(ShareInfo::CDocGameInfo&) final;
};

} // namespace Function

#endif // #ifndef FUNCTION_CSTAGECONTROL_H_