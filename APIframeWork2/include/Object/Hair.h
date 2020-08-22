#pragma once
#include "MoveObj.h"
class Hair :
	public MoveObj
{
	friend class Obj;
	friend class Scene;

private:
	Hair();
	Hair(const Hair& hair);
	virtual ~Hair();

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
	virtual Hair* Clone();
};

