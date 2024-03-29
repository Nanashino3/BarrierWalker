#ifndef FUNCTION_CFUNCTIONCONTROLLER_H_
#define FUNCTION_CFUNCTIONCONTROLLER_H_

#include <vector>

namespace Function{ class AFunction; }
namespace Camera{ class CCamera2D; }
namespace ShareInfo{ class CDocGameInfo; }

namespace Function
{
class CFunctionController
{
public:
	CFunctionController();
	~CFunctionController();

	void Initialize();
	void Update(ShareInfo::CDocGameInfo&);
	void Collision(ShareInfo::CDocGameInfo&);
	void Draw(ShareInfo::CDocGameInfo&);

private:
	std::vector<Function::AFunction*> m_functions;
};

} // namespace Function

#endif // #ifndef FUNCTION_CFUNCTIONCONTROLLER_H_