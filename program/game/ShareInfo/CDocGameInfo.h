#ifndef SHAREINFO_CDOCGAMEINFO_H_
#define SHAREINFO_CDOCGAMEINFO_H_

#include "CONST_GAME_VALUE.h"
#include "E_COLLISION_OBJECT_TYPE.h"
#include "S_VELOCITY_INFO.h"
#include "../../dxlib_ext/dxlib_ext.h"

namespace tnl{ class Vector3; }
namespace Camera{ class CCamera2D; }

namespace ShareInfo
{
class CDocGameInfo
{
public:
	~CDocGameInfo();

	static CDocGameInfo* GetInstance();
	static void DestroyInstance();
	
	void Initialize();
	void SetDeltaTime(float);
	float GetDeltaTime() const;
	void SetIsCollision(bool);
	bool GetIsCollision() const;
	void SetCurrentPos(tnl::Vector3&);
	tnl::Vector3 GetCurrentPos() const;
	void SetCorrectType(E_COLLISION_OBJECT_TYPE, int);
	int GetCorrectType(E_COLLISION_OBJECT_TYPE) const;
	void SetScreenMax(int);
	int GetScreenMax() const;
	void SetFunctionType(unsigned int);
	unsigned int GetFunctionType() const;
	void SetBarrierEnergy(int, float);
	float GetBarrierEnergy(int) const;
	void SetBarrierColor(int, int);
	int GetBarrierColor(int) const;
	void SetIsPlayerHit(bool);
	bool GetIsPlayerHit() const;
	void SetIsGameOver(bool);
	bool GetIsGameOver() const;
	void SetStageIndex(int);
	int GetStageIndex() const;
	void SetScreenWidth(int);
	int GetScreenWidth() const;
	void SetScreenHeight(int);
	int GetScreenHeight() const;
	void SetMaxStage(int);
	int GetMaxStage() const;
	void SetIsClear(bool);
	bool GetIsClear() const;
	void SetIsFinalStage(bool);
	bool GetIsFinalStage() const;
	void SetActionType(unsigned int);
	unsigned int GetActionType() const;
	void SetCamera(Camera::CCamera2D*);
	Camera::CCamera2D* GetCamera() const;
	void SetAcquiredItem(unsigned int);
	unsigned int GetAcquiredItem() const;
	void SetChangeType(unsigned int);
	unsigned int GetChangeType() const;
	void SetVelocity(const S_VELOCITY_INFO&);
	S_VELOCITY_INFO GetVelocity() const;
	void SetPrevScene(unsigned int);
	unsigned int GetPrevScene() const;
	void SetScoreCalcSrcInfo(unsigned int, unsigned int);
	unsigned int GetScoreCalcSrcInfo(unsigned int);

private:
	float m_deltaTime;
	bool m_isCollision;
	tnl::Vector3 m_currentPos;
	int m_correctType[MAX_COLLISION_OBJECT_TYPE];
	int m_screenMax;
	unsigned int m_functionType;
	float m_barrierEnergy[MAX_ATTRIBUTE_TYPE];
	int m_barrierColor[MAX_ATTRIBUTE_TYPE];
	bool m_isPlayerHit;
	bool m_isGmaeOver;
	int m_stageIndex;
	int m_screenWidth;
	int m_screenHeight;
	int m_maxStage;
	bool m_isClear;
	bool m_isFinalStage;
	unsigned int m_actionType;
	Camera::CCamera2D* m_camera;
	unsigned int m_acquiredItem;
	unsigned int m_changeType;
	S_VELOCITY_INFO m_velocity;
	unsigned int m_prevScene;
	unsigned int m_calcSource[MAX_ACCESS_KEY];

	// ÉRÉsÅ[Ç≥ÇÍÇ»Ç¢ÇÊÇ§Ç…ëŒâû
	CDocGameInfo();
	CDocGameInfo(const CDocGameInfo&){}
	CDocGameInfo& operator=(const CDocGameInfo&){}

	static CDocGameInfo* s_myInstance;
};

} // namespace ShareInfo

#endif // #ifndef CDOCGAMEINFO_H_