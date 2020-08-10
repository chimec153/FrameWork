#include "Weapon.h"
#include "../Animation/Animation.h"
#include "../Scene/Scene.h"

CWeapon::CWeapon()
{
}

CWeapon::CWeapon(const CWeapon& weapon)	:
	MoveObj(weapon)
{
}

CWeapon::~CWeapon()
{
}

bool CWeapon::Init()
{
	SetSize(32.f, 64.f);
	SetPivot(0.5f, 0.5f);

	Animation* pAni = CreateAnimation("ToolAni");
	AddAnimationClip("Idle", AT_ATLAS, AO_LOOP, 0.5f, 0, 0, 0, 0, 0, 0, 1.f, "Idle", TEXT("Item\\HoeIdle.bmp"));
	SetAnimationClipColorKey("Idle", 0, 0, 0);

	AddAnimationClip("DefaultHoeDown", AT_ATLAS, AO_ONCE_RETURN, 0.5f, 6, 1, 0, 0, 2, 1, 1.f, "HoeDown", TEXT("Item\\HoeDown.bmp"));
	SetAnimationClipColorKey("DefaultHoeDown", 0, 0, 0);

	AddAnimationClip("DefaultHoeUp", AT_ATLAS, AO_ONCE_RETURN, 0.5f, 6, 1, 0, 0, 2, 1, 1.f, "HoeUp", TEXT("Item\\HoeUp.bmp"));
	SetAnimationClipColorKey("DefaultHoeUp", 0, 0, 0);

	AddAnimationClip("DefaultHoeLeft", AT_ATLAS, AO_ONCE_RETURN, 0.5f, 6, 1, 0, 0, 2, 1, 1.f, "HoeLeft", TEXT("Item\\HoeLeft.bmp"));
	SetAnimationClipColorKey("DefaultHoeLeft", 0, 0, 0);

	AddAnimationClip("DefaultHoeRight", AT_ATLAS, AO_ONCE_RETURN, 0.5f, 6, 1, 0, 0, 2, 1, 1.f, "HoeRight", TEXT("Item\\HoeRight.bmp"));
	SetAnimationClipColorKey("DefaultHoeRight", 0, 0, 0);

	SAFE_RELEASE(pAni);

	return true;
}

void CWeapon::Input(float fDeltaTime)
{
	MoveObj::Input(fDeltaTime);
}

int CWeapon::Update(float fDeltaTime)
{
	MoveObj::Update(fDeltaTime);
	return 0;
}

int CWeapon::LateUpdate(float fDeltaTime)
{
	MoveObj::LateUpdate(fDeltaTime);
	return 0;
}

void CWeapon::Collision(float fDeltaTime)
{
	MoveObj::Collision(fDeltaTime);
}

void CWeapon::Render(HDC hDC, float fDeltaTime)
{
	MoveObj::Render(hDC, fDeltaTime);
}

CWeapon* CWeapon::Clone()
{
	return new CWeapon(*this);
}

void CWeapon::Attack()
{
	Obj* pObj = m_pScene->GetPlayer();

	POSITION tDir = ((MoveObj*)pObj)->GetAngle();

	SAFE_RELEASE(pObj);

	if (m_pAnimation)
	{
		if (tDir.x > 0.f)
		{

			m_pAnimation->SetCurrentClip("DefaultHoeRight");
		}

		else if (tDir.x < 0.f)
		{

			m_pAnimation->SetCurrentClip("DefaultHoeLeft");
		}

		else if (tDir.y > 0.f)
		{

			m_pAnimation->SetCurrentClip("DefaultHoeDown");
		}

		else if (tDir.y < 0.f)
		{

			m_pAnimation->SetCurrentClip("DefaultHoeUp");
		}
			
	}
		
}
