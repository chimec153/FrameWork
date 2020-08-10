#include "Player.h"
#include "../Core/Camera.h"
#include "../Core/Input.h"
#include "../Resources/Texture.h"
#include "Bullet.h"
#include "../Collider/ColliderRect.h"
#include "../Core/Camera.h"
#include "../Animation/Animation.h"
#include "../Scene/Scene.h"
#include "..//Core/PathManager.h"
#include "..//Scene/SceneManager.h"
#include "..//Object/Stage.h"
#include "..//Scene/InGameScene.h"
#include "..//Object/Tile.h"
#include "UIBar.h"
#include "Weapon.h"
#include "UINum.h"
#include "..//Collider/ColliderSphere.h"
#include "Item.h"
#include "../Object/UIInventory.h"

Player::Player()	:
	m_bAttack(false),
	m_eType(PAT_NONE),
	m_bColl(false),
	m_pEnergyBar(nullptr),
	m_pHPBar(nullptr),
	m_pWeapon(nullptr),
	m_pInventory(nullptr)
{
	m_bTileEffect = true;
}

Player::Player(const Player & player)	:
	FightObj(player)
{
}

Player::~Player()
{
	SAFE_RELEASE(m_pWeapon);
}

void Player::SetWeapon(Obj* pObj)
{
	SAFE_RELEASE(m_pWeapon);

	m_pWeapon = pObj;

	if (m_pWeapon)
		m_pWeapon->AddRef();
}


bool Player::Init()
{
	Obj::SetCollide(true);
	SetPos(256.f, 512.f);
	SetSize(32.f, 64.f);
	SetSpeed(200.f);
	SetPivot(0.5f, 1.f);
	SetTag("Player");
	m_iHP = 100;
	m_iEnergy = 100;

	SetForce(200.f);

	ColliderRect* pRC = AddCollider<ColliderRect>("attack");
	pRC->SetRect(-96.f, 32.f, 96.f, 128.f);
	pRC->SetEnable(false);
	SAFE_RELEASE(pRC);

	ColliderSphere* pSphere = AddCollider<ColliderSphere>("PlayerBody");

	pSphere->SetSphere(POSITION(0.f, -16.f), 16.f);
	pSphere->AddCollisionFunction(CS_ENTER, this,
		&Player::Hit);
	pSphere->AddCollisionFunction(CS_STAY, this,
		&Player::HitStay);
	pSphere->AddCollisionFunction(CS_LEAVE, this,
		&Player::HitLeave);

	SAFE_RELEASE(pSphere);

	Animation* pAni = CreateAnimation("PlayerAnimation");

	AddAnimationClip("IdleRight", AT_ATLAS, AO_LOOP, 0.5f, 6, 1,
		0, 0, 1, 1, 0.f, "PlayerIdleRight", L"WalkRight.bmp");
	SetAnimationClipColorKey("IdleRight", 0, 0, 0);

	AddAnimationClip("IdleLeft", AT_ATLAS, AO_LOOP, 0.5f, 6, 1,
		0, 0, 1, 1, 0.f, "PlayerIdleLeft", L"walkLeft.bmp");
	SetAnimationClipColorKey("IdleLeft", 0, 0, 0);

	AddAnimationClip("IdleUp", AT_ATLAS, AO_LOOP, 0.5f, 6, 1,
		0, 0, 1, 1, 0.f, "PlayerIdleUp", L"WalkUp.bmp");
	SetAnimationClipColorKey("IdleUp", 0, 0, 0);

	AddAnimationClip("IdleDown", AT_ATLAS, AO_LOOP, 0.5f, 6, 1,
		0, 0, 1, 1, 0.f, "PlayerIdleDown", L"aniii.bmp");
	SetAnimationClipColorKey("IdleDown", 0, 0, 0);

	AddAnimationClip("RunRight", AT_ATLAS, AO_ONCE_RETURN, 0.5f, 6, 1,
		1, 0, 2, 1, 0.f, "PlayerRunRight", L"WalkRight.bmp");
	SetAnimationClipColorKey("RunRight", 0, 0, 0);

	AddAnimationClip("RunLeft", AT_ATLAS, AO_ONCE_RETURN, 0.5f, 6, 1,
		1, 0, 2, 1, 0.f, "PlayerRunLeft", L"walkLeft.bmp");
	SetAnimationClipColorKey("RunLeft", 0, 0, 0);

	AddAnimationClip("RunUp", AT_ATLAS, AO_ONCE_RETURN, 0.5f, 6, 1,
		1, 0, 2, 1, 0.f, "PlayerRunUp", L"WalkUp.bmp");
	SetAnimationClipColorKey("RunUp", 0, 0, 0);

	AddAnimationClip("RunDown", AT_ATLAS, AO_ONCE_RETURN, 0.5f, 6, 1,
		1, 0, 2, 1, 0.f, "PlayerRunDown", L"aniii.bmp");
	SetAnimationClipColorKey("RunDown", 0, 0, 0);

	AddAnimationClip("AttackRight", AT_ATLAS, AO_ONCE_RETURN, 0.5f, 6, 1,
		0, 0, 6, 1, 0.f, "PlayerAttackRight", L"AttackRight.bmp");
	SetAnimationClipColorKey("AttackRight", 0, 0, 0);

	AddAnimationClip("AttackLeft", AT_ATLAS, AO_ONCE_RETURN, 0.5f, 6, 1,
		0, 0, 6, 1, 0.f, "PlayerAttackLeft", L"AttackLeft.bmp");
	SetAnimationClipColorKey("AttackLeft", 0, 0, 0);

	AddAnimationClip("AttackUp", AT_ATLAS, AO_ONCE_RETURN, 0.5f, 6, 1,
		0, 0, 6, 1, 0.f, "PlayerAttackUp", L"AttackUp.bmp");
	SetAnimationClipColorKey("AttackUp", 0, 0, 0);

	AddAnimationClip("AttackDown", AT_ATLAS, AO_ONCE_RETURN, 0.5f, 6, 1,
		0, 0, 6, 1, 0.f, "PlayerAttackDown", L"attackDown.bmp");
	SetAnimationClipColorKey("AttackDown", 0, 0, 0);

	AddAnimationClip("FarmRight", AT_ATLAS, AO_ONCE_RETURN, 0.5f, 6, 1,
		0, 0, 6, 1, 0.f, "PlayerFarmRight", L"farmRight.bmp");
	SetAnimationClipColorKey("FarmRight", 0, 0, 0);

	AddAnimationClip("FarmLeft", AT_ATLAS, AO_ONCE_RETURN, 0.5f, 6, 1,
		0, 0, 6, 1, 0.f, "PlayerFarmLeft", L"FarmLeft.bmp");
	SetAnimationClipColorKey("FarmLeft", 0, 0, 0);

	AddAnimationClip("FarmUp", AT_ATLAS, AO_ONCE_RETURN, 0.5f, 6, 1,
		0, 0, 6, 1, 0.f, "PlayerFarmUp", L"farmUp.bmp");
	SetAnimationClipColorKey("FarmUp", 0, 0, 0);

	AddAnimationClip("FarmDown", AT_ATLAS, AO_ONCE_RETURN, 0.5f, 6, 1,
		0, 0, 6, 1, 0.f, "PlayerFarmDown", L"farmDown.bmp");
	SetAnimationClipColorKey("FarmDown", 0, 0, 0);

	SAFE_RELEASE(pAni);	

	return true;
}

void Player::Input(float fDeltaTime)
{
	FightObj::Input(fDeltaTime);

	bool bPress = false;

	if (KEYPRESS("MoveLeft"))
	{
		bPress = true;
		m_pAnimation->ChangeClip("RunLeft");
		m_pAnimation->SetDefaultClip("IdleLeft");
		m_tMoveDir = POSITION(-1.f, 0.f);
		MoveAngle(fDeltaTime);
	}

	else if (KEYPRESS("MoveRight"))
	{
		bPress = true;
		m_pAnimation->ChangeClip("RunRight");
		m_pAnimation->SetDefaultClip("IdleRight");
		m_tMoveDir = POSITION(1.f, 0.f);
		MoveAngle(fDeltaTime);
	}

	else if (KEYPRESS("MoveFront"))
	{
		bPress = true;
		m_pAnimation->ChangeClip("RunUp");
		m_pAnimation->SetDefaultClip("IdleUp");
		m_tMoveDir = POSITION(0.f, -1.f);
		MoveAngle(fDeltaTime);
	}

	else if (KEYPRESS("MoveBack"))
	{
		bPress = true;
		m_pAnimation->ChangeClip("RunDown");
		m_pAnimation->SetDefaultClip("IdleDown");
		m_tMoveDir = POSITION(0.f, 1.f);
		MoveAngle(fDeltaTime);
	}

	if (KEYDOWN("Fire"))
	{
		Fire();

		if (m_pWeapon)
			((CWeapon*)m_pWeapon)->Attack();

		Collider* pCol = GetCollider("attack");
		pCol->SetEnable(true);
		if (m_tMoveDir.x == -1.f)
		{
			((ColliderRect*)pCol)->SetRect(-96.f, -32.f, -32.f, 32.f);
			m_pAnimation->ChangeClip("AttackLeft");
		}

		if (m_tMoveDir.y == -1.f)
		{
			((ColliderRect*)pCol)->SetRect(-48.f, -64.f, 48.f, -16.f);
			m_pAnimation->ChangeClip("AttackUp");
		}

		if (m_tMoveDir.x == 1.f)
		{
			((ColliderRect*)pCol)->SetRect(32.f, -32.f, 96.f, 32.f);
			m_pAnimation->ChangeClip("AttackRight");
		}

		if (m_tMoveDir.y == 1.f)
		{
			((ColliderRect*)pCol)->SetRect(-48.f, 16.f, 48.f, 64.f);
			m_pAnimation->ChangeClip("AttackDown");
		}
		SAFE_RELEASE(pCol);

		Tile* pTile = ((InGameScene*)m_pScene)->GetStage()->GetSelectedTile();

		if (pTile)
		{
			TILE_OPTION eOption = pTile->GetTileOption();

			if (eOption == TO_DIRT)
			{
				pTile->SetTileOption(TO_HOEDIRT);
			}

			else if (eOption == TO_HOEDIRT)
			{
				pTile->SetTileOption(TO_WATERDIRT);
			}
		}
	}

	if (KEYDOWN("TileSub"))		//	1번 키
		m_pInventory->SetCursor(0);

	if (KEYDOWN("TileAdd"))		//	2번 키
		m_pInventory->SetCursor(1);

	if (KEYDOWN("TileSetChange"))		//	3번 키
		m_pInventory->SetCursor(2);

	if (KEYDOWN("4"))
		m_pInventory->SetCursor(3);

	if (KEYDOWN("5"))
		m_pInventory->SetCursor(4);

	if (KEYDOWN("6"))
		m_pInventory->SetCursor(5);

	if (KEYDOWN("7"))
		m_pInventory->SetCursor(6);

	if (KEYDOWN("8"))
		m_pInventory->SetCursor(7);

	if (KEYDOWN("9"))
		m_pInventory->SetCursor(8);

	if (KEYDOWN("10"))
		m_pInventory->SetCursor(9);

	if (KEYDOWN("11"))
		m_pInventory->SetCursor(10);

	if (KEYDOWN("12"))
		m_pInventory->SetCursor(11);

	
}

int Player::Update(float fDeltaTime)
{
	FightObj::Update(fDeltaTime);

	if (m_bAttack && m_pAnimation->GetMotionEnd())
		m_bAttack = false;


	if (m_pAnimation->GetMotionEnd())
	{
		Collider* pCol = GetCollider("attack");
		pCol->SetEnable(false);
		SAFE_RELEASE(pCol);
	}

	if (m_pEnergyBar)
		m_pEnergyBar->SetValue((float)m_iEnergy);

	if (m_pHPBar)
		m_pHPBar->SetValue((float)m_iHP);

	if (m_pWeapon)
	{
		m_pWeapon->SetPos(m_tPos + m_tMoveDir * 16.f);
	}

	Item* pItem = m_pInventory->GetItem();

	if (pItem)
	{
		pItem->GetType();
	}

	return 0;
}

int Player::LateUpdate(float fDeltaTime)
{
	FightObj::LateUpdate(fDeltaTime);

	return 0;
}

void Player::Collision(float fDeltaTime)
{
	FightObj::Collision(fDeltaTime);
}

void Player::Render(HDC hDC, float fDeltaTime)
{
	FightObj::Render(hDC,fDeltaTime);

	Stage* pStage = ((InGameScene*)GET_SINGLE(SceneManager)->GetScene())->GetStage();
	int iIndex = pStage->GetTileIndex(m_tPos);

	if (iIndex == -1)
		return;

	TILE_OPTION eOption = pStage->GetTile(iIndex)->GetTileOption();

	POSITION	tPos = m_tPos - m_tSize * m_tPivot;
	POSITION	tCamPos = GET_SINGLE(Camera)->GetPos();
	tPos -= tCamPos;
#ifdef _DEBUG
	if (KEYPRESS("Debug"))
	{
		wchar_t strHP[32] = {};
		wsprintf(strHP, L"PosX : %d", (int)(m_tPos.x));
		TextOut(hDC, (int)tPos.x + 100, (int)tPos.y - 40, strHP, lstrlen(strHP));
		wsprintf(strHP, L"PosY : %d", (int)(m_tPos.y));
		TextOut(hDC, (int)tPos.x + 100, (int)tPos.y - 60, strHP, lstrlen(strHP));
		if (eOption == TO_NONE)
			TextOut(hDC, (int)tPos.x, (int)tPos.y - 20, TEXT("move"), lstrlen(TEXT("move")));
		if (eOption == TO_NOMOVE)
			TextOut(hDC, (int)tPos.x, (int)tPos.y - 20, TEXT("Nomove"), lstrlen(TEXT("Nomove")));
		wsprintf(strHP, L"Coll : %d", (int)m_bColl);
		TextOut(hDC, (int)tPos.x + 180, (int)tPos.y - 80, strHP, lstrlen(strHP));
	}
#endif
}

Player * Player::Clone()
{
	return new Player(*this);
}


void Player::Hit(Collider * pSrc, Collider * pDest, float fDeltaTime)
{
	string strDest = pDest->GetTag();
	if(strDest =="MinionBullet")
		m_iHP -= 5;

	else if (strDest == "TreeBody" || strDest == "BuildingBody")
	{
		m_bColl = true;
	}

	else if (strDest == "MinionBody")
	{
		m_iHP -= 5;

		m_pHPBar->SetValue((float)m_iHP);

		POSITION tPos = pDest->GetObj()->GetPos();

		tPos -= m_tPos;

		tPos *= -1;

		SetAngle(tPos);
		MoveAngle(fDeltaTime * 40.f);

		Layer* pLayer = m_pScene->FindLayer("UI");

		Obj* pNum = CreateCloneObj("NumSm", "Num", SC_CURRENT, pLayer);

		((UINum*)pNum)->CreateNum(5);
		pNum->SetPos(m_tPos.x, m_tPos.y - m_tSize.y * m_tPivot.y);

		SAFE_RELEASE(pNum);
	}
}

void Player::HitStay(Collider * pSrc, Collider * pDest, float fDeltaTime)
{
	string strDest = pDest->GetTag();
	if (strDest == "StageColl")
	{
		ClearGravity();
		JumpEnd();
		m_tPos.y = pSrc->GetHitPoint().y - m_tPivot.y * m_tSize.y;
	}

	else if (strDest == "TreeBody" || strDest == "BuildingBody")
	{
		m_bColl = true;
	}
}

void Player::HitLeave(Collider* pSrc, Collider* pDest, float fDeltaTime)
{
	string strDest = pDest->GetTag();
	if (strDest == "TreeBody" || strDest == "BuildingBody")
	{
		m_bColl = false;
	}
}

void Player::Fire()
{
}