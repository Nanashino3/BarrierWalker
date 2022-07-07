#include <windows.h>
#include <stdio.h>
#include <stdarg.h>
#include "DxLib.h"
#include "dxlib_ext.h"

void DrawStringEx(const int x, const int y, const int color, const char* _str, ...) {
	char buff[255] = { 0 };
	va_list argptr;
	va_start(argptr, _str);
	vsprintf(buff, _str, argptr);
	va_end(argptr);
	DrawString(x, y, buff, color);
}

void DrawLineEx(const tnl::Vector3& p, const tnl::Vector3& v, const float length, const int color) {
	tnl::Vector3 n = v;
	n.normalize();
	n *= length;
	DrawLine(p.x, p.y, p.x + n.x, p.y + n.y, color, false);
}

void DrawBoxEx(const tnl::Vector3& p, const float width, const float height, const bool fill, const int color) {
	float lx = p.x - width * 0.5f;
	float rx = p.x + width * 0.5f;
	float up = p.y - height * 0.5f;
	float down = p.y + height * 0.5f;
	DrawBox(lx, up, rx, down, color, fill);
}

