#pragma once
#include "MoveObj.h"
class Hat :
	public MoveObj
{
	friend class Obj;
	friend class Scene;

private:
	Hat();
	Hat(const Hat& hat);
	virtual ~Hat();

private:
	class Player*	m_pPlayer;
	HAT_TYPE		m_eHatType;
	vector<char*>	m_vecHatName;

public:
	void SetPlayer(class Player* pPlayer)
	{
		m_pPlayer = pPlayer;
	}

	void SetHatType(HAT_TYPE eType);

	HAT_TYPE GetHatType()	const
	{
		return m_eHatType;
	}

public:
	virtual bool Init();
	virtual int Update(float fTime);
	virtual void Render(HDC hDC, float fTime);
	virtual Hat* Clone();
};

