#ifndef SHAREINFO_S_BARRIER_INFO_H_
#define SHAREINFO_S_BARRIER_INFO_H_

namespace ShareInfo
{
struct S_BARRIER_INFO
{
	int color;		// バリアの色
	float energy;	// エネルギー
	bool isBarrier;	// バリアを張るか
	bool isEmpty;	// バリアが空か
};

} // namespace ShareInfo

#endif // #ifndef SHAREINFO_S_BARRIER_INFO_H_