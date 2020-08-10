#pragma once
#include "Obj.h"
class StaticObj :
	public Obj
{
protected:
	StaticObj();
	StaticObj(const StaticObj& obj);
	virtual ~StaticObj();

public:
	virtual bool Init() = 0;
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual StaticObj* Clone() = 0;
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

