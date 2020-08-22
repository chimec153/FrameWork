#pragma once
#include "MoveObj.h"
class Shirt :
	public MoveObj
{
	friend class Obj;
	friend class Scene;

private:
	Shirt();
	Shirt(const Shirt& shirt);
	virtual ~Shirt();

private:
	class Player* m_pPlayer;

public:
	void SetPlayer(class Player* pPlayer)
	{
		m_pPlayer = pPlayer;
	}

public:
	virtual bool Init();
	virtual int Update(float fTime);
	virtual void Render(HDC hDC, float fTime);
	virtual Shirt* Clone();
};

