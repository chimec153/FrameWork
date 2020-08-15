#pragma once
#include "Item.h"
class Etc :
	public Item
{
private:
	friend class Obj;
	friend class Scene;

protected:
	Etc();
	Etc(const Etc& etc);
	virtual ~Etc();

public:
	virtual bool Init();
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual Etc* Clone();

};

