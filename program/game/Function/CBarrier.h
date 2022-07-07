#ifndef FUNCTION_CBARRIER_H_
#define FUNCTION_CBARRIER_H_

#include "AFunction.h"

namespace Sound{ class CSoundManager; }
namespace Function
{
class CBarrier : public AFunction
{
public:
	CBarrier(unsigned int);
	virtual ~CBarrier();

	virtual void Initialize() final;
	virtual void Update(ShareInfo::CDocGameInfo&) final;
	virtual void Draw(ShareInfo::CDocGameInfo&) final;

private:
	void PriSwap(unsigned int&, unsigned int&, int, int);
	void PriFluctuation(unsigned int&, int, ShareInfo::CDocGameInfo&);

private:
	int m_animType;		// アニメーション種
	int m_animFrame;	// アニメーションのフレーム数
	Sound::CSoundManager* m_soundManager;
};

} // namespace Function

#endif // #ifndef FUNCTION_CBARRIER_H_