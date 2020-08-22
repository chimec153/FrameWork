#pragma once
#include "MoveObj.h"
class Pants :
	public MoveObj
{
	friend class Obj;
	friend class Scene;

private:
	Pants();
	Pants(const Pants& pants);
	virtual ~Pants();

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
	virtual Pants* Clone();
};

