#ifndef UI_CUICONTROLLER_H_
#define UI_CUICONTROLLER_H_

#include <vector>

namespace Ui{ class AUi; }
namespace Camera{ class CCamera2D; }
namespace ShareInfo{ class CDocGameInfo; }

namespace Ui
{
class CUiController
{
public:
	CUiController();
	~CUiController();

	void Initialize(ShareInfo::CDocGameInfo&);
	void Update(ShareInfo::CDocGameInfo&);
	void Draw(ShareInfo::CDocGameInfo&);

private:
	std::vector<Ui::AUi*> m_uiList;

};

} // namespace Ui

#endif // #ifndef UI_CUICONTROLLER_H_