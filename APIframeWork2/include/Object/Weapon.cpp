#include "Weapon.h"
#include "../Animation/Animation.h"
#include "../Scene/Scene.h"
#include "ObjManager.h"
#include "UIInventory.h"
#include "Tool.h"
#include "Player.h"
#include "../Collider/ColliderRect.h"

CWeapon::CWeapon()	:
	m_pPlayer(nullptr),
	m_bAction(false)
{
	m_cAlpha = 255;
	m_bAlphaOn = true;
}

CWeapon::CWeapon(const CWeapon& weapon)	:
	MoveObj(weapon)
{
	m_pPlayer = weapon.m_pPlayer;
	m_bAction = weapon.m_bAction;
}

CWeapon::~CWeapon()
{
}

bool CWeapon::Init()
{
	SetSize(32.f, 64.f);
	SetPivot(0.5f, 0.5f);

	Animation* pAni = CreateAnimation("ToolAni");

	pAni->LoadFromPath("Tools.sac");

	AddAnimationClip("Idle", AT_ATLAS, AO_LOOP, 0.5f, 0, 0, 0, 0, 0, 0, 1.f,0, "Idle", TEXT("Item\\HoeIdle.bmp"));
	SetAnimationClipColorKey("Idle", 0, 0, 0);
	SetAnimationDefaultClip("Idle");

	vector<wstring>	vecTexture;

	for (int i = 0; i < 12; ++i)
	{
		TCHAR strFileName[MAX_PATH] = {};

		lstrcat(strFileName, TEXT("Item\\sword\\sword"));

		TCHAR strNum[32] = {};

		swprintf_s(strNum, TEXT("%d.bmp"), i + 1);

		lstrcat(strFileName, strNum);

		vecTexture.push_back(strFileName);
	}

	vector<wstring>	vecScytheTexture;

	for (int i = 0; i < 12; ++i)
	{
		TCHAR strFileName[MAX_PATH] = {};

		lstrcat(strFileName, TEXT("Item\\scythe\\scythe"));

		TCHAR strNum[32] = {};

		swprintf_s(strNum, TEXT("%d.bmp"), i + 1);

		lstrcat(strFileName, strNum);

		vecScytheTexture.push_back(strFileName);
	}

	AddAnimationClip("swordUp", AT_FRAME, AO_ONCE_RETURN, 0.5f, 12, 1, 0, 0, 4, 1, 1.f, 0, "sword", vecTexture);
	SetAnimationClipColorKey("swordUp", 255, 255, 255);

	AddAnimationClip("swordRight", AT_FRAME, AO_ONCE_RETURN, 0.5f, 12, 1, 2, 0, 4, 1, 1.f, 0, "sword", vecTexture);
	SetAnimationClipColorKey("swordRight", 255, 255, 255);

	AddAnimationClip("swordDown", AT_FRAME, AO_ONCE_RETURN, 0.5f, 12, 1, 4, 0, 4, 1, 1.f, 0, "sword", vecTexture);
	SetAnimationClipColorKey("swordDown", 255, 255, 255);

	AddAnimationClip("swordLeft", AT_FRAME, AO_ONCE_RETURN, 0.5f, 12, 1, 6, 0, 4, 1, 1.f, 0, "sword", vecTexture);
	SetAnimationClipColorKey("swordLeft", 255, 255, 255);

	AddAnimationClip("sword2Up", AT_FRAME, AO_ONCE_RETURN, 0.5f, 12, 1, 0, 0, 4, 1, 1.f, 0, "sword2-", vecTexture);
	SetAnimationClipColorKey("sword2Up", 255, 255, 255);

	AddAnimationClip("sword2Right", AT_FRAME, AO_ONCE_RETURN, 0.5f, 12, 1, 2, 0, 4, 1, 1.f, 0, "sword2-", vecTexture);
	SetAnimationClipColorKey("sword2Right", 255, 255, 255);

	AddAnimationClip("sword2Down", AT_FRAME, AO_ONCE_RETURN, 0.5f, 12, 1, 4, 0, 4, 1, 1.f, 0, "sword2-", vecTexture);
	SetAnimationClipColorKey("sword2Down", 255, 255, 255);

	AddAnimationClip("sword2Left", AT_FRAME, AO_ONCE_RETURN, 0.5f, 12, 1, 6, 0, 4, 1, 1.f, 0, "sword2-", vecTexture);
	SetAnimationClipColorKey("sword2Left", 255, 255, 255);

	AddAnimationClip("scytheDown", AT_FRAME, AO_ONCE_RETURN, 0.5f, 8, 1, 1, 0, 4, 1, 1.f, 0, "scythe", vecScytheTexture);
	SetAnimationClipColorKey("scytheDown", 255, 255, 255);

	AddAnimationClip("scytheLeft", AT_FRAME, AO_ONCE_RETURN, 0.5f, 8, 1, 3, 0, 4, 1, 1.f, 0, "scythe", vecScytheTexture);
	SetAnimationClipColorKey("scytheLeft", 255, 255, 255);

	AddAnimationClip("scytheUp", AT_FRAME, AO_ONCE_RETURN, 0.5f, 8, 1, 5, 0, 4, 1, 1.f, 0, "scythe", vecScytheTexture);
	SetAnimationClipColorKey("scytheUp", 255, 255, 255);

	AddAnimationClip("scytheRight", AT_FRAME, AO_ONCE_RETURN, 0.5f, 8, 1, 7, 0, 4, 1, 1.f, 0, "scythe", vecScytheTexture);
	SetAnimationClipColorKey("scytheRight", 255, 255, 255);

	SAFE_RELEASE(pAni);

	ColliderRect* pRC = AddCollider<ColliderRect>("attack");

	pRC->SetRect(-20.f, -20.f, 20.f, 20.f);
	pRC->SetEnable(false);

	SAFE_RELEASE(pRC);

	return true;
}

void CWeapon::Input(float fDeltaTime)
{
	MoveObj::Input(fDeltaTime);
}

int CWeapon::Update(float fDeltaTime)
{
	MoveObj::Update(fDeltaTime);

	POSITION tPos = {};

	POSITION tMoveDir = {};

	if (m_pPlayer)
	{
		tPos = m_pPlayer->GetPos();

		tMoveDir = m_pPlayer->GetAngle();

		SetPos(tPos.x + tMoveDir.x * 16.f, tPos.y - 0.5f * 64.f);
	}

	if (m_bAction)
	{
		if (m_pAnimation->GetMotionEnd())
		{
			m_bAction = false;

			Collider* pCol = GetCollider("attack");

			pCol->SetEnable(false);

			SAFE_RELEASE(pCol);
		}

		UIInventory* pInven = GET_SINGLE(ObjManager)->GetInven();

		Tool* pTool = (Tool*)pInven->GetItem();

		TOOL_TYPE eToolType = TOOL_SWORD;

		if (pTool)
			 eToolType = pTool->GetToolType();

		SAFE_RELEASE(pInven);

		if (eToolType == TOOL_SWORD || eToolType == TOOL_SCYTHE)
		{
			int iFrame = m_pAnimation->GetFrame();

			if (eToolType == TOOL_SCYTHE)
				iFrame -= 5;

			float fAngle = PI-PI / 4.f * iFrame;

			float fX = cosf(fAngle) * 40.f;
			float fY = -sinf(fAngle) * 40.f;

			SetPos(tPos.x + fX, tPos.y - 32.f + fY);
		}
	}

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
	Collider* pCol = GetCollider("attack");

	pCol->SetEnable(true);

	SAFE_RELEASE(pCol);

	Obj* pObj = GET_SINGLE(ObjManager)->GetPlayer();

	POSITION tDir = ((MoveObj*)pObj)->GetAngle();

	SAFE_RELEASE(pObj);

	UIInventory* pInven = GET_SINGLE(ObjManager)->GetInven();

	Tool* pTool = (Tool*)pInven->GetItem();

	TOOL_TYPE eToolType = pTool->GetToolType();

	SAFE_RELEASE(pInven);
	
	if (m_pAnimation)
	{
		if (tDir.x > 0.f)
		{
			switch (eToolType)
			{
			case TOOL_SWORD:
				SetSize(50.f, 50.f);
				m_pAnimation->SetCurrentClip("swordRight");
				break;
			case TOOL_HOE:
				SetSize(32.f, 64.f);
				m_pAnimation->SetCurrentClip("HoeRight");
				break;
			case TOOL_EX:
				SetSize(32.f, 64.f);
				m_pAnimation->SetCurrentClip("ExRight");
				break;
			case TOOL_PIKEX:
				SetSize(32.f, 64.f);
				m_pAnimation->SetCurrentClip("PikexRight");
				break;
			case TOOL_WATER:
				SetSize(32.f, 64.f);
				m_pAnimation->SetCurrentClip("WaterRight");
				break;
			case TOOL_SCYTHE:
				SetSize(50.f, 50.f);
				m_pAnimation->SetCurrentClip("scytheRight");
				break;
			}
		}

		else if (tDir.x < 0.f)
		{
			switch (eToolType)
			{
			case TOOL_SWORD:
				SetSize(50.f, 50.f);
				m_pAnimation->SetCurrentClip("swordLeft");
				break;
			case TOOL_HOE:
				SetSize(32.f, 64.f);
				m_pAnimation->SetCurrentClip("HoeLeft");
				break;
			case TOOL_EX:
				SetSize(32.f, 64.f);
				m_pAnimation->SetCurrentClip("ExLeft");
				break;
			case TOOL_PIKEX:
				SetSize(32.f, 64.f);
				m_pAnimation->SetCurrentClip("PikexLeft");
				break;
			case TOOL_WATER:
				SetSize(32.f, 64.f);
				m_pAnimation->SetCurrentClip("WaterLeft");
				break;
			case TOOL_SCYTHE:
				SetSize(50.f, 50.f);
				m_pAnimation->SetCurrentClip("scytheLeft");
				break;
			}
		}

		else if (tDir.y > 0.f)
		{
			switch (eToolType)
			{
			case TOOL_SWORD:
				SetSize(50.f, 50.f);
				m_pAnimation->SetCurrentClip("swordDown");
				break;
			case TOOL_HOE:
				SetSize(32.f, 64.f);
				m_pAnimation->SetCurrentClip("HoeDown");
				break;
			case TOOL_EX:
				SetSize(32.f, 64.f);
				m_pAnimation->SetCurrentClip("ExDown");
				break;
			case TOOL_PIKEX:
				SetSize(32.f, 64.f);
				m_pAnimation->SetCurrentClip("PikexDown");
				break;
			case TOOL_WATER:
				SetSize(32.f, 64.f);
				m_pAnimation->SetCurrentClip("WaterDown");
				break;
			case TOOL_SCYTHE:
				SetSize(50.f, 50.f);
				m_pAnimation->SetCurrentClip("scytheDown");
				break;
			}
		}

		else if (tDir.y < 0.f)
		{
			switch (eToolType)
			{
			case TOOL_SWORD:
				SetSize(50.f, 50.f);
				m_pAnimation->SetCurrentClip("swordUp");
				break;
			case TOOL_HOE:
				SetSize(32.f, 64.f);
				m_pAnimation->SetCurrentClip("HoeUp");
				break;
			case TOOL_EX:
				SetSize(32.f, 64.f);
				m_pAnimation->SetCurrentClip("ExUp");
				break;
			case TOOL_PIKEX:
				SetSize(32.f, 64.f);
				m_pAnimation->SetCurrentClip("PikexUp");
				break;
			case TOOL_WATER:
				SetSize(32.f, 64.f);
				m_pAnimation->SetCurrentClip("WaterUp");
				break;
			case TOOL_SCYTHE:
				SetSize(50.f, 50.f);
				m_pAnimation->SetCurrentClip("scytheUp");
				break;
			}
		}
	}

	m_bAction = true;
		
}
