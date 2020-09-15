#pragma once
#include "MoveObj.h"
class Accessory :
	public MoveObj
{
	friend class Obj;
	friend class Scene;

private:
	Accessory();
	Accessory(const Accessory& accessory);
	virtual ~Accessory();

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
	virtual Accessory* Clone();
};

