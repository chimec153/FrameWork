#include "PrairieKingLeg.h"
#include "PrairieKing.h"
#include "../Animation/Animation.h"

PrairieKingLeg::PrairieKingLeg()	:
	m_pKing(nullptr)
{
}

PrairieKingLeg::PrairieKingLeg(const PrairieKingLeg& leg)	:
	MoveObj(leg)
{
	m_pKing = leg.m_pKing;
}

PrairieKingLeg::~PrairieKingLeg()
{
}

bool PrairieKingLeg::Init()
{
	SetSize(20.f, 6.f);

	Animation * pAni = CreateAnimation("KingLegAni");

	pAni->LoadFromPath("kingleg.sac");

	SAFE_RELEASE(pAni);

	return true;
}

int PrairieKingLeg::Update(float fTime)
{
	MoveObj::Update(fTime);

	SetPos(m_pKing->GetPos() - m_pKing->GetSize() * m_pKing->GetPivot());

	PLAYER_ACTION eAction = m_pKing->GetAction();

	bool bRevive = m_pKing->IsState(KS_REVIVE);

	if (eAction == PA_IDLE)
		SetAnimationCurrentClip("KingLegIdle");

	else if(bRevive)
		SetAnimationCurrentClip("ReviveIdle");

	else if(eAction == PA_WALK)
		SetAnimationCurrentClip("KingLegWalk");

	return 0;
}

void PrairieKingLeg::Render(HDC hDC, float fTime)
{
	MoveObj::Render(hDC, fTime);
}

PrairieKingLeg* PrairieKingLeg::Clone()
{
	return new PrairieKingLeg(*this);
}
