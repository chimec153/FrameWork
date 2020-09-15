#pragma once
#include "StaticObj.h"
class Furniture :
	public StaticObj
{
	friend class Obj;
	friend class Scene;

private:
	Furniture();
	Furniture(const Furniture& furniture);
	virtual ~Furniture();

public:
	virtual bool Init();
	virtual int Update(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(HDC hDC, float fTime);
	virtual Furniture* Clone();
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

