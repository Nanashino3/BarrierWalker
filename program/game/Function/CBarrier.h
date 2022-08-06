#ifndef FUNCTION_CBARRIER_H_
#define FUNCTION_CBARRIER_H_

#include <vector>
#include <string>
#include "AFunction.h"
#include "CONST_FUNCTION_VALUE.h"
#include "../ShareInfo/S_BARRIER_INFO.h"
#include "../ShareInfo/CONST_GAME_VALUE.h"

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
	int m_gfxHdl[MAX_ATTRIBUTE_TYPE][MAX_BARRIER_ANIM_FRAME];
	std::vector<ShareInfo::S_BARRIER_INFO> m_barriers;
	std::vector<std::string> m_imageList;
};

} // namespace Function

#endif // #ifndef FUNCTION_CBARRIER_H_