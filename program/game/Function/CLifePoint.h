#ifndef FUNCTION_CLIFEPOINT_H_
#define FUNCTION_CLIFEPOINT_H_

#include "AFunction.h"
#include "CONST_FUNCTION_VALUE.h"

namespace Sound{ class CSoundManager; }
namespace Function
{
class CLifePoint : public AFunction
{
public:
	CLifePoint(unsigned int type = 0);
	virtual ~CLifePoint();

	virtual void Initialize() final;
	virtual void Update(ShareInfo::CDocGameInfo&) final;
	virtual void Draw(ShareInfo::CDocGameInfo&) final;

private:
	int m_lifePoint;
	bool m_hitting;
	Sound::CSoundManager* m_soundManager;
	int m_gfxHdl[MAX_LIFE_POINT];
};

} // namespace Function

#endif // #ifndef FUNCTION_CLIFEPOINT_H_