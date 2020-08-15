#pragma once
#include "MoveObj.h"
class SlimeHead :
	public MoveObj
{
private:
	friend class Obj;
	friend class Scene;

private:
	SlimeHead();
	SlimeHead(const SlimeHead& head);
	virtual ~SlimeHead();

public:
	virtual bool Init();
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual SlimeHead* Clone();
};

