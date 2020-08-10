#include "FightObj.h"

FightObj::FightObj()	:
	m_iHP(0),
	m_iEnergy(0)
{
}

FightObj::FightObj(const FightObj& fighter)	:
	MoveObj(fighter)
{
	m_iHP = fighter.m_iHP;
	m_iEnergy = fighter.m_iEnergy;
}

FightObj::~FightObj()
{
}

bool FightObj::Init()
{
	return true;
}

void FightObj::Input(float fDeltaTime)
{
	MoveObj::Input(fDeltaTime);
}

int FightObj::Update(float fDeltaTime)
{
	MoveObj::Update(fDeltaTime);
	return 0;
}

int FightObj::LateUpdate(float fDeltaTime)
{
	MoveObj::LateUpdate(fDeltaTime);
	return 0;
}

void FightObj::Collision(float fDeltaTime)
{
	MoveObj::Collision(fDeltaTime);
}

void FightObj::Render(HDC hDC, float fDeltaTime)
{
	MoveObj::Render(hDC, fDeltaTime);
}