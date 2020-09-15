#pragma once
#include "FightObj.h"

enum ANIMAL_ACTION
{
	AA_IDLE,
	AA_WALK,
	AA_EAT,
	AA_SIT,
	AA_END
};

class Animal :
	public FightObj
{
private:
	friend class Obj;
	friend class Scene;

protected:
	Animal();
	Animal(const Animal& animal);
	virtual ~Animal();

protected:
	int				m_iDay;
	int				m_iFeedCount;
	ANIMAL_ACTION	m_eAction;
	bool			m_bFeed;
	bool			m_bBaby;
	ANIMAL_TYPE		m_eAnimalType;
	int				m_iGrowDay;
	float			m_fTime;
	float			m_fLimitTime;
	float			m_fEatLimitTime;
	int				m_iFriendShip;

public:
	virtual bool AddDay(int iDay);

	void SetFeed(bool bFeed)
	{
		m_bFeed = bFeed;
	}

	bool IsFeed()	const
	{
		return m_bFeed;
	}

public:
	virtual bool Init();
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual Animal* Clone();

public:
	void ColEnter(class Collider* pSrc, class Collider* pDest, float fTime);
	void ColStay(class Collider* pSrc, class Collider* pDest, float fTime);
	void ColLeave(class Collider* pSrc, class Collider* pDest, float fTime);

public:
	virtual void ActionChange(ANIMAL_ACTION eAction);
};

