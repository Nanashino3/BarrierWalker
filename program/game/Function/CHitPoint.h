#ifndef FUNCTION_CHITPOINT_H_
#define FUNCTION_CHITPOINT_H_

#include "AFunction.h"

namespace Sound{ class CSoundManager; }
namespace Function
{
class CHitPoint : public AFunction
{
public:
	CHitPoint(unsigned int type = 0);
	virtual ~CHitPoint();

	virtual void Initialize() final;
	virtual void Update(ShareInfo::CDocGameInfo&) final;
	virtual void Draw(ShareInfo::CDocGameInfo&) final;

private:
	int m_hitPoint;
	bool m_hitting;
	Sound::CSoundManager* m_soundManager;
};

} // namespace Function

#endif // #ifndef FUNCTION_CHITPOINT_H_