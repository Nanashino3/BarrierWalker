#ifndef CGAMEDIRECTOR_H_
#define CGAMEDIRECTOR_H_

#include <vector>

namespace GameObject{
	class AFixedObject;
	class AMoveObject;
	class CMoveObjectManager;
}
namespace Camera{ class CCamera2D; }
namespace Function{ class CFunctionController; }
namespace ShareInfo{ class CDocGameInfo; }
namespace Ui{ class CUiController; }

class CGameDirector
{
public:
	CGameDirector();
	~CGameDirector();

	void Initialize();
	void Update();
	void Draw();

private:
	GameObject::AMoveObject* m_playerObject;
	GameObject::CMoveObjectManager* m_moveObjManager;
	Camera::CCamera2D* m_camera;
	Function::CFunctionController* m_functionController;
	ShareInfo::CDocGameInfo* m_docGameInfo;
	Ui::CUiController* m_uiController;
	std::vector<GameObject::AFixedObject*> m_fixedObjects;
};

#endif // #ifndef CGAMEDIRECTOR_H_