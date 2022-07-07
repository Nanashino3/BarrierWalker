#ifndef CAMERA_CCAMERA2D_H_
#define CAMERA_CCAMERA2D_H_
#include "../../dxlib_ext/dxlib_ext.h"

namespace ShareInfo{ class CDocGameInfo; }

namespace Camera
{
class CCamera2D
{
public:
	CCamera2D(int, int, const tnl::Vector3& pos = tnl::Vector3(0, 0, 0));
	~CCamera2D();

	void Initialize();
	void Update(ShareInfo::CDocGameInfo&);

	tnl::Vector3 GetPosition() const { return m_pos; }

private:
	int m_screen_w;
	int m_screen_h;
	tnl::Vector3 m_pos;
};

} // namespace Camera

#endif // #ifndef CAMERA_CCAMERA2D_H_