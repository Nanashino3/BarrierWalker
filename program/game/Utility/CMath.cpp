#include "CMath.h"
#include "../../dxlib_ext/dxlib_ext.h"

#pragma intrinsic(sqrt, pow);

namespace Utility
{
float GetDistance2D(tnl::Vector3& p1, tnl::Vector3& p2)
{
	return (float)sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

} // namespace Utility