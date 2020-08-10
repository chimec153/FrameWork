#pragma once
#include "MoveObj.h"
class FightObj :
	public MoveObj
{
protected:
	FightObj();
	FightObj(const FightObj& fighter);
	virtual ~FightObj() = 0;

protected:
	int							m_iHP;
	int							m_iEnergy;

public:
	virtual bool Init() = 0;
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual FightObj* Clone() = 0;
};

