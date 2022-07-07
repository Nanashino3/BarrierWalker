#ifndef UI_AUI_H_
#define UI_AUI_H_

namespace ShareInfo{ class CDocGameInfo; }
namespace Ui
{
class AUi
{
public:
	AUi(){}
	virtual ~AUi(){}

	virtual void Initialize(ShareInfo::CDocGameInfo&){}
	virtual void Update(ShareInfo::CDocGameInfo&){}
	virtual void Draw(ShareInfo::CDocGameInfo&){}

};

} // namespace Ui

#endif // #ifndef UI_AUI_H_
