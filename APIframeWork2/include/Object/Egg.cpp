#include "Egg.h"
#include "Animal.h"
#include "Chicken.h"
#include "UIClockHand.h"
#include "ObjManager.h"

Egg::Egg()	:
	m_iDay(0),
	m_eEggType(ET_CHICK)
{
	m_eType = IT_EGG;
}

Egg::Egg(const Egg& egg)	:
	Item(egg)
{
	m_iDay = egg.m_iDay;
	m_eEggType = egg.m_eEggType;
}

Egg::~Egg()
{
}

void Egg::Birth()
{
	Die();

	if (m_eEggType == ET_CHICK)
	{
		Animal* pChick = (Animal*)CreateCloneObj("Chick", "Chick", m_pLayer);

		pChick->SetPos(m_tPos);

		int iColor = rand() % CC_END;

		((Chicken*)pChick)->SetColor((CHICKEN_COLOR)iColor);
		pChick->ActionChange(AA_IDLE);

		UIClockHand* pClockHand = (UIClockHand*)GET_SINGLE(ObjManager)->GetClockHand();

		pClockHand->AddAnimal(pChick);

		SAFE_RELEASE(pChick);
	}

	else if (m_eEggType == ET_DUCK)
	{
		Animal* pDuck = (Animal*)CreateCloneObj("Duck", "Duck", m_pLayer);

		pDuck->SetPos(m_tPos);

		pDuck->ActionChange(AA_IDLE);

		UIClockHand* pClockHand = (UIClockHand*)GET_SINGLE(ObjManager)->GetClockHand();

		pClockHand->AddAnimal(pDuck);

		SAFE_RELEASE(pDuck);
	}

	else if (m_eEggType == ET_DINO)
	{
		Animal* pDinosaur = (Animal*)CreateCloneObj("Dinosaur", "Dinosaur", m_pLayer);

		pDinosaur->SetPos(m_tPos);

		pDinosaur->ActionChange(AA_IDLE);

		UIClockHand* pClockHand = (UIClockHand*)GET_SINGLE(ObjManager)->GetClockHand();

		pClockHand->AddAnimal(pDinosaur);

		SAFE_RELEASE(pDinosaur);
	}
}

bool Egg::Init()
{
	SetTexture("items");

	SetColorKey(255, 255, 255);

	if (!Item::Init())
		return false;

	return true;
}

int Egg::Update(float fDeltaTime)
{
	Item::Update(fDeltaTime);
	return 0;
}

int Egg::LateUpdate(float fDeltaTime)
{
	Item::LateUpdate(fDeltaTime);
	return 0;
}

void Egg::Collision(float fDeltaTime)
{
	Item::Collision(fDeltaTime);
}

void Egg::Render(HDC hDC, float fDeltaTime)
{
	Item::Render(hDC, fDeltaTime);
}

Egg* Egg::Clone()
{
	return new Egg(*this);
}
