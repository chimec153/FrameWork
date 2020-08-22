#pragma once
#include "MoveObj.h"
class Arm :
	public MoveObj
{
	friend class Obj;
	friend class Scene;

private:
	Arm();
	Arm(const Arm& arm);
	virtual ~Arm();

private:
	class Player*	m_pPlayer;

public:
	void SetPlayer(class Player* pPlayer)
	{
		m_pPlayer = pPlayer;
	}

public:
	virtual bool Init();
	virtual int Update(float fTime);
	virtual void Render(HDC hDC, float fTime);
	virtual Arm* Clone();

};

