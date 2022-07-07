#ifndef SHAREINFO_S_DRAW_INFO_3F_H_
#define SHAREINFO_S_DRAW_INFO_3F_H_

namespace ShareInfo
{
struct S_DRAW_INFO_3F
{
	float xf;		 // 画面上中心：X座標
	float yf;		 // 画面上中心：Y座標
	float cxf;		 // 画像上中心：X座標
	float cyf;		 // 画像上中心：Y座標
	double extRateX; // 横方向の拡大率(1.0で等倍)
	double extRateY; // 縦方向の拡大率(1.0で等倍)
	double angle;	 // 描画角度(ラジアン指定)
	int grHandle;	 // グラフィックハンドル
	int transFlag;	 // 透明度を有効にするか
};

} // namespace ShareInfo

#endif // #ifndef SHAREINFO_S_DRAW_INFO_3F_H_