#pragma once
#include "MoveObj.h"
class PrairieKingLeg :
	public MoveObj
{
	friend class Obj;
	friend class Scene;

private:
	PrairieKingLeg();
	PrairieKingLeg(const PrairieKingLeg& leg);
	virtual ~PrairieKingLeg();

private:
	class PrairieKing* m_pKing;

public:
	void SetKing(class PrairieKing* pKing)
	{
		m_pKing = pKing;
	}

public:
	virtual bool Init();
	virtual int Update(float fTime);
	virtual void Render(HDC hDC, float fTime);
	virtual PrairieKingLeg* Clone();
};

