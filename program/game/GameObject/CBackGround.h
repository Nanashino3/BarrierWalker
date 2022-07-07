#ifndef GAMEOBJECT_CBACKGROUND_H_
#define GAMEOBJECT_CBACKGROUND_H_

#include "AFixedObject.h"

namespace GameObject
{
class CBackGround : public AFixedObject
{
public:
	CBackGround();
	virtual ~CBackGround();

	virtual void Initialize(ShareInfo::CDocGameInfo&) final;
	virtual void Draw(ShareInfo::CDocGameInfo&) final;

private:
	int m_gfxHdl;
	float m_posX;
	float m_posY;
};

} // namespace GameObject

#endif // #ifndef GAMEOBJECT_CBACKGROUND_H_