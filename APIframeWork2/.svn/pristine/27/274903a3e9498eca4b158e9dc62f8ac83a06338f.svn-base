#pragma once
#include "MoveObj.h"
class FarmEffect :
	public MoveObj
{
private:
	friend class Obj;
	friend class Scene;

private:
	FarmEffect();
	FarmEffect(const FarmEffect& farmeffect);
	~FarmEffect();

public:
	virtual bool Init();
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual FarmEffect* Clone();
};

