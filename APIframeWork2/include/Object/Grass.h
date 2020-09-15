#pragma once
#include "StaticObj.h"
class Grass :
	public StaticObj
{
	friend class Obj;
	friend class Scene;

private:
	Grass();
	Grass(const Grass& grass);
	virtual ~Grass();

private:

public:
	virtual bool Init();
	virtual int Update(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(HDC hDC, float fTime);
	virtual Grass* Clone();
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

public:
	void ColEnter(class Collider* pSrc, class Collider* pDest, float fTime);
};

