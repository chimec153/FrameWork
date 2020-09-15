#pragma once
#include "Animal.h"

enum HORSE_OPTION
{
	HORSE_IDLE,
	HORSE_WALK,
	HORSE_END
};

class Horse :
	public Animal
{
private:
	friend class Obj;
	friend class Scene;

private:
	Horse();
	Horse(const Horse& horse);
	virtual ~Horse();

private:
	class Player*		m_pPlayer;
	HORSE_OPTION		m_eOption;

public:
	void SetPlayer(class Player* pPlayer);

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual Horse* Clone();

public:
	void ColEnter(class Collider* pSrc, class Collider* pDest, float fTime);
	void ColStay(class Collider* pSrc, class Collider* pDest, float fTime);
};

