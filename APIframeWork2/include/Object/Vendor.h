#pragma once
#include "MoveObj.h"
class Vendor :
	public MoveObj
{
private:
	friend class Obj;
	friend class Scene;

private:
	Vendor();
	Vendor(const Vendor& vendor);
	~Vendor();

public:
	virtual bool Init();
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual Vendor* Clone();
};

