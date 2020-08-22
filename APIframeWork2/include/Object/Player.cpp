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
#include "Tool.h"
#include "../Core/Timer.h"
#include "Seed.h"
#include "Crop.h"
#include "Tool.h"
#include "UIClockHand.h"
#include "Tree.h"
#include "Etc.h"
#include "Rock.h"
#include "RockCrab.h"
#include "ObjManager.h"
#include "../Scene/SceneHome.h"
#include "../Resources/ResourcesManager.h"
#include "UIShop.h"
#include "Arm.h"
#include "Pants.h"
#include "Hair.h"
#include "Shirt.h"
#include "../Scene/SceneCrapentersShop.h"
#include "../Scene/SceneShop.h"
#include "../Scene/SceneWeaponShop.h"

Player::Player()	:
	m_bAttack(false),
	m_bColl(false),
	m_pEnergyBar(nullptr),
	m_pHPBar(nullptr),
	m_bWalk(false),
	m_eAction(PA_IDLE),
	m_iGold(0),
	m_iBuildings(0),
	m_pArm(nullptr),
	m_pPants(nullptr),
	m_pHair(nullptr),
	m_pShirt(nullptr)
{
	m_bTileEffect = true;
}

Player::Player(const Player & player)	:
	FightObj(player)
{
	m_bAttack = false;
	m_bColl = false;

	if (player.m_pEnergyBar)
		m_pEnergyBar = player.m_pEnergyBar->Clone();

	if (player.m_pHPBar)
		m_pHPBar = player.m_pHPBar->Clone();

	m_bWalk = false;
	m_eAction = player.m_eAction;
	m_iGold = player.m_iGold;
	m_iBuildings = player.m_iBuildings;

	if (player.m_pArm)
		m_pArm = player.m_pArm->Clone();

	if (player.m_pPants)
		m_pPants = player.m_pPants->Clone();

	if (player.m_pHair)
		m_pHair = player.m_pHair->Clone();

	if (player.m_pShirt)
		m_pShirt = player.m_pShirt->Clone();
}

Player::~Player()
{
}


void Player::AddGold(int iGold)
{
	m_iGold += iGold;

	UIInventory* pInventory = GET_SINGLE(ObjManager)->GetInven();

	if(pInventory)
		pInventory->SetGoldText(m_iGold);

	SAFE_RELEASE(pInventory);
}

bool Player::AddEnergy(int iEnergy)
{
	if (FightObj::AddEnergy(iEnergy))
	{
		UIClockHand* pClockHand = (UIClockHand*)GET_SINGLE(ObjManager)->GetClockHand();

		pClockHand->SetTime(360.f);
		pClockHand->AddDay();

		SetEnergy(50);
		SetHP(50);

		GET_SINGLE(SceneManager)->CreateScene<SceneHome>("Home", SC_NEXT);

		SetPos(288.f, 288.f);

		return true;
	}

	iEnergy = GetEnergy();

	m_pEnergyBar->SetValue((float)iEnergy);

	return false;
}

bool Player::AddHP(int iHP)
{
	if (FightObj::AddHP(iHP))
	{
		UIClockHand* pClockHand = (UIClockHand*)GET_SINGLE(ObjManager)->GetClockHand();

		pClockHand->SetTime(360.f);
		pClockHand->AddDay();

		SetEnergy(50);
		SetHP(50);

		GET_SINGLE(SceneManager)->CreateScene<SceneHome>("Home",SC_NEXT);

		SetPos(288.f, 288.f);

		return true;
	}

	iHP = GetHP();

	m_pHPBar->SetValue((float)iHP);

	return false;
}

void Player::SetEnergy(int iEnergy)
{
	FightObj::SetEnergy(iEnergy);

	m_pEnergyBar->SetValue((float)iEnergy);
}

void Player::SetHP(int iHP)
{
	FightObj::SetHP(iHP);

	m_pHPBar->SetValue((float)iHP);
}

void Player::SetArm(Arm* pArm)
{
	m_pArm = pArm;

	if (m_pArm)
		m_pArm->SetPlayer(this);
}

void Player::SetPants(Pants* pPants)
{
	m_pPants = pPants;

	if (m_pPants)
		m_pPants->SetPlayer(this);
}

void Player::SetHair(Hair* pHair)
{
	m_pHair = pHair;

	if (m_pHair)
		m_pHair->SetPlayer(this);
}

void Player::SetShirt(Shirt* pShirt)
{
	m_pShirt = pShirt;

	if (m_pShirt)
		m_pShirt->SetPlayer(this);
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

	SetAttack(10);

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

	pAni->LoadFromPath("base.sac");

	SAFE_RELEASE(pAni);	

	return true;
}

void Player::Input(float fDeltaTime)
{
	FightObj::Input(fDeltaTime);

	UIInventory* pInventory = GET_SINGLE(ObjManager)->GetInven();

	if (pInventory)
	{
		UIShop* pShop = pInventory->GetShop();

		if (pInventory->GetExtended())
		{
			SAFE_RELEASE(pInventory);
			return;
		}

		if (pShop)
		{
			if (pShop->IsShopPanelOn())
			{
				SAFE_RELEASE(pInventory);
				return;
			}
		}
	}

	bool bPress = false;

	Item* pItem = nullptr;

	if (pInventory)
		pItem = pInventory->GetItem();

	SAFE_RELEASE(pInventory);

	ITEM_TYPE eType = IT_NONE;

	if(pItem)
		eType = pItem->GetType();

	if (KEYPRESS("MoveLeft"))
	{
		bPress = true;

		{
			m_pAnimation->ChangeClip("PlayerWalkLeft");
			m_pAnimation->SetDefaultClip("PlayerIdleLeft");
		}

		m_tMoveDir = POSITION(-1.f, 0.f);
		MoveAngle(fDeltaTime);

		m_eAction = PA_WALK;

		CWeapon* pWeapon = (CWeapon*)GET_SINGLE(ObjManager)->GetWeapon();

		if (pWeapon)
			pWeapon->SetPos(m_tPos.x + m_tMoveDir.x * 16.f, m_tPos.y - 0.5f * m_tSize.y + m_tMoveDir.y * 16.f);// +m_tMoveDir * 16.f);

		SAFE_RELEASE(pWeapon);

	}

	else if (KEYPRESS("MoveRight"))
	{
		bPress = true;

		{
			m_pAnimation->ChangeClip("PlayerWalkRight");
			m_pAnimation->SetDefaultClip("PlayerIdleRight");
		}

		m_tMoveDir = POSITION(1.f, 0.f);
		MoveAngle(fDeltaTime);

		m_eAction = PA_WALK;

		CWeapon* pWeapon = (CWeapon*)GET_SINGLE(ObjManager)->GetWeapon();

		if (pWeapon)
			pWeapon->SetPos(m_tPos.x + m_tMoveDir.x * 16.f, m_tPos.y - 0.5f * m_tSize.y + m_tMoveDir.y * 16.f);// +m_tMoveDir * 16.f);

		SAFE_RELEASE(pWeapon);

	}

	else if (KEYPRESS("MoveFront"))
	{
		bPress = true;

		{
			m_pAnimation->ChangeClip("PlayerWalkUp");
			m_pAnimation->SetDefaultClip("PlayerIdleUp");
		}

		m_tMoveDir = POSITION(0.f, -1.f);
		MoveAngle(fDeltaTime);

		m_eAction = PA_WALK;

		CWeapon* pWeapon = (CWeapon*)GET_SINGLE(ObjManager)->GetWeapon();

		if (pWeapon)
			pWeapon->SetPos(m_tPos.x + m_tMoveDir.x * 16.f, m_tPos.y - 0.5f * m_tSize.y );// +m_tMoveDir * 16.f);

		SAFE_RELEASE(pWeapon);

	}

	else if (KEYPRESS("MoveBack"))
	{
		bPress = true;

		{
			m_pAnimation->ChangeClip("PlayerWalkDown");
			m_pAnimation->SetDefaultClip("PlayerIdleDown");
		}

		m_tMoveDir = POSITION(0.f, 1.f);
		MoveAngle(fDeltaTime);

		m_eAction = PA_WALK;

		CWeapon* pWeapon = (CWeapon*)GET_SINGLE(ObjManager)->GetWeapon();

		if (pWeapon)
			pWeapon->SetPos(m_tPos.x + m_tMoveDir.x * 16.f, m_tPos.y - 0.5f * m_tSize.y );

		SAFE_RELEASE(pWeapon);

	}

	if ((KEYDOWN("Fire") || KEYDOWN("MouseLButton")) && m_eAction == PA_IDLE)
	{
		Stage* pStage = m_pScene->GetStage();

		OBJ_BLOCK eBlock = pStage->GetBlock();

		if (eBlock == OB_CROP)
		{
			Tile* pTile = pStage->GetSelectedTile();

			Crop* pCrop = (Crop*)pTile->GetObj();

			bool bHarvested = pCrop->IsHarvested();

			CROP_TYPE eCropType = pCrop->GetCropType();

			bool bHarvest = pCrop->CanHarvest();

			if (!bHarvested && bHarvest)
			{
				pCrop->Harvest();

				UIClockHand* pClockHand = (UIClockHand*)GET_SINGLE(ObjManager)->GetClockHand();

				if (pCrop->GetRegrowthDay() == 0)
				{
					pClockHand->DeleteCrop(pCrop);

					pStage->SetBlock(OB_NONE, nullptr);
				}
			}
		}

		if (eType == IT_TOOL && m_iEnergy >= 5.f)
		{
			TOOL_TYPE eToolType = ((Tool*)pItem)->GetToolType();

			if (eToolType == TOOL_SWORD)
			{
				CWeapon* pWeapon = (CWeapon*)GET_SINGLE(ObjManager)->GetWeapon();

				if (pWeapon)
				{
					pWeapon->Attack();
				}

				SAFE_RELEASE(pWeapon);

				Collider* pCol = GetCollider("attack");

				pCol->SetEnable(true);

				if (m_tMoveDir.x == -1.f)
				{
					((ColliderRect*)pCol)->SetRect(-96.f, -32.f, -32.f, 32.f);
					m_pAnimation->ChangeClip("PlayerAttackLeft");
				}

				if (m_tMoveDir.y == -1.f)
				{
					((ColliderRect*)pCol)->SetRect(-48.f, -64.f, 48.f, -16.f);
					m_pAnimation->ChangeClip("PlayerAttackUp");
				}

				if (m_tMoveDir.x == 1.f)
				{
					((ColliderRect*)pCol)->SetRect(32.f, -32.f, 96.f, 32.f);
					m_pAnimation->ChangeClip("PlayerAttackRight");
				}

				if (m_tMoveDir.y == 1.f)
				{
					((ColliderRect*)pCol)->SetRect(-48.f, 16.f, 48.f, 64.f);
					m_pAnimation->ChangeClip("PlayerAttackDown");
				}

				SAFE_RELEASE(pCol);

				m_eAction = PA_ATTACK;
			}

			else if (eToolType == TOOL_HOE)
			{
				CWeapon* pWeapon = (CWeapon*)GET_SINGLE(ObjManager)->GetWeapon();

				if (pWeapon)
					pWeapon->Attack();

				SAFE_RELEASE(pWeapon);

				Tile* pTile = ((InGameScene*)m_pScene)->GetStage()->GetSelectedTile();

				if (pTile)
				{
					TILE_OPTION eOption = pTile->GetTileOption();

					if (eOption == TO_DIRT)
					{
						pTile->SetTileOption(TO_HOEDIRT);
					}
				}

				if (m_tMoveDir.x == -1.f)
				{
					m_pAnimation->ChangeClip("PlayerAttackLeft");
				}

				if (m_tMoveDir.y == -1.f)
				{
					m_pAnimation->ChangeClip("PlayerAttackUp");
				}

				if (m_tMoveDir.x == 1.f)
				{
					m_pAnimation->ChangeClip("PlayerAttackRight");
				}

				if (m_tMoveDir.y == 1.f)
				{
					m_pAnimation->ChangeClip("PlayerAttackDown");
				}

				m_eAction = PA_FARM;
			}

			else if (eToolType == TOOL_EX)
			{
				CWeapon* pWeapon = (CWeapon*)GET_SINGLE(ObjManager)->GetWeapon();

				if (pWeapon)
				{
					pWeapon->Attack();
				}
				SAFE_RELEASE(pWeapon);

				Tile* pTile = ((InGameScene*)m_pScene)->GetStage()->GetSelectedTile();

				if (pTile)
				{
					Obj* pObj = pTile->GetObj();

					if (pObj)
					{
						OBJ_BLOCK eBlock = pObj->GetBlock();

						if (eBlock == OB_TREE)
						{
							((CTree*)pObj)->ExTree();
						}
					}

				}

				if (m_tMoveDir.x == -1.f)
				{
					m_pAnimation->ChangeClip("PlayerAttackLeft");
				}

				if (m_tMoveDir.y == -1.f)
				{
					m_pAnimation->ChangeClip("PlayerAttackUp");
				}

				if (m_tMoveDir.x == 1.f)
				{
					m_pAnimation->ChangeClip("PlayerAttackRight");
				}

				if (m_tMoveDir.y == 1.f)
				{
					m_pAnimation->ChangeClip("PlayerAttackDown");
				}

				m_eAction = PA_FARM;
			}

			else if (eToolType == TOOL_PIKEX)
			{
				CWeapon* pWeapon = (CWeapon*)GET_SINGLE(ObjManager)->GetWeapon();

				if (pWeapon)
					pWeapon->Attack();

				SAFE_RELEASE(pWeapon);

				Tile* pTile = ((InGameScene*)m_pScene)->GetStage()->GetSelectedTile();

				if (pTile)
				{
					Obj* pObj = pTile->GetObj();

					if (pObj)
					{
						OBJ_BLOCK eBlock = pObj->GetBlock();

						if (eBlock == OB_ROCK)
						{
							((Rock*)pObj)->Pick();
						}
					}
				}

				if (m_tMoveDir.x == -1.f)
				{
					m_pAnimation->ChangeClip("PlayerAttackLeft");
				}

				if (m_tMoveDir.y == -1.f)
				{
					m_pAnimation->ChangeClip("PlayerAttackUp");
				}

				if (m_tMoveDir.x == 1.f)
				{
					m_pAnimation->ChangeClip("PlayerAttackRight");
				}

				if (m_tMoveDir.y == 1.f)
				{
					m_pAnimation->ChangeClip("PlayerAttackDown");
				}

				m_eAction = PA_FARM;
			}

			else if (eToolType == TOOL_WATER)
			{
				CWeapon* pWeapon = (CWeapon*)GET_SINGLE(ObjManager)->GetWeapon();

				if (pWeapon)
					pWeapon->Attack();

				SAFE_RELEASE(pWeapon);

				Tile* pTile = ((InGameScene*)m_pScene)->GetStage()->GetSelectedTile();

				if (pTile)
				{
					TILE_OPTION eOption = pTile->GetTileOption();

					if (eOption == TO_HOEDIRT)
					{
						if (((Tool*)pItem)->GetWater())
						{
							pTile->SetTileOption(TO_WATERDIRT);
							((Tool*)pItem)->SetWater(false);
						}
					}

					else if (eOption == TO_WATER)
						((Tool*)pItem)->SetWater(true);
				}

				if (m_tMoveDir.x == -1.f)
				{
					m_pAnimation->ChangeClip("PlayerWaterLeft");
				}

				if (m_tMoveDir.y == -1.f)
				{
					m_pAnimation->ChangeClip("PlayerWaterUp");
				}

				if (m_tMoveDir.x == 1.f)
				{
					m_pAnimation->ChangeClip("PlayerWaterRight");
				}

				if (m_tMoveDir.y == 1.f)
				{
					m_pAnimation->ChangeClip("PlayerWaterDown");
				}

				m_eAction = PA_WATER;
			}

			AddEnergy(-5);

		}

		else if (eType == IT_SEED)
		{
			CROP_TYPE eCropType = ((Seed*)pItem)->GetCropType();
			Stage* pStage = ((InGameScene*)m_pScene)->GetStage();

			Tile* pTile = pStage->GetSelectedTile();

			if (pTile)
			{
				if (!pTile->GetObj())
				{
					TILE_OPTION eOption = pTile->GetTileOption();

					if (eOption == TO_HOEDIRT || eOption == TO_WATERDIRT)
					{
						PITEMINFO pInfo = GET_SINGLE(ResourcesManager)->FindItemInfo(eCropType);

						Obj* pCrop = CreateCloneObj(pInfo->strCropName, pInfo->strCropName, m_pLayer);

						POSITION tPos = pCrop->GetSize() * pCrop->GetPivot();

						tPos.y = 0.f;

						pCrop->SetPos(pTile->GetPos() + tPos);

						((UIClockHand*)GET_SINGLE(ObjManager)->GetClockHand())->AddCrop(pCrop);

						Collider* pCol = pCrop->GetCollider("CropBody");

						pCol->AddCollisionFunction(CS_STAY, (Crop*)pCrop, &Crop::MousePress);

						SAFE_RELEASE(pCol);

						pStage->SetBlock(OB_CROP, pCrop);

						SAFE_RELEASE(pCrop);

						UIInventory* pInventory = GET_SINGLE(ObjManager)->GetInven();

						if (pInventory)
							pInventory->DeleteItem();	//	인벤토리에서 아이템 삭제

						SAFE_RELEASE(pInventory);
					}
				}
			}
		}

		else if (eType == IT_CROP)
		{
			bool bHarvest = ((Crop*)pItem)->IsHarvested();
			CROP_TYPE eCropType = ((Crop*)pItem)->GetCropType();

			int iEnergy = ((Crop*)pItem)->GetEnergy();
			int iHP = ((Crop*)pItem)->GetHP();

			if (bHarvest &&( iEnergy != 0 || iHP != 0))
			{
				m_pAnimation->SetCurrentClip("Eat");
				m_eAction = PA_EAT;

				AddEnergy(iEnergy);
				AddHP(iHP);

				UIInventory* pInventory =GET_SINGLE(ObjManager)->GetInven();

				if(pInventory)
				pInventory->DeleteItem();

				SAFE_RELEASE(pInventory);
			}
		}
	}

	pInventory = GET_SINGLE(ObjManager)->GetInven();

	if (pInventory)
	{
		if (KEYDOWN("TileSub"))		//	1번 키
			pInventory->SetCursor(0);

		if (KEYDOWN("TileAdd"))		//	2번 키
			pInventory->SetCursor(1);

		if (KEYDOWN("TileSetChange"))		//	3번 키
			pInventory->SetCursor(2);

		if (KEYDOWN("4"))
			pInventory->SetCursor(3);

		if (KEYDOWN("5"))
			pInventory->SetCursor(4);

		if (KEYDOWN("6"))
			pInventory->SetCursor(5);

		if (KEYDOWN("7"))
			pInventory->SetCursor(6);

		if (KEYDOWN("8"))
			pInventory->SetCursor(7);

		if (KEYDOWN("9"))
			pInventory->SetCursor(8);

		if (KEYDOWN("10"))
			pInventory->SetCursor(9);

		if (KEYDOWN("11"))
			pInventory->SetCursor(10);

		if (KEYDOWN("12"))
			pInventory->SetCursor(11);

		SAFE_RELEASE(pInventory);
	}

#ifdef _DEBUG
	if (KEYPRESS("TimeFaster"))
	{
		float fScale = GET_SINGLE(Timer)->GetTimeScale();

		GET_SINGLE(Timer)->SetTimeScale(fScale + 0.1f);
	}

	else if (KEYPRESS("TimeSlower"))
	{
		float fScale = GET_SINGLE(Timer)->GetTimeScale();

		if (fScale > 0.1f)
			GET_SINGLE(Timer)->SetTimeScale(fScale - 0.1f);
	}

	else if (KEYPRESS("TimeOrigin"))
	{
		GET_SINGLE(Timer)->SetTimeScale(1.f);
	}

	else if (KEYDOWN("CarpenterShop"))
	{
		GET_SINGLE(SceneManager)->CreateScene<SceneCrapentersShop>("CarpentersShop", SC_NEXT);

		SetPos(32.f * 4.5f, 32.f * 9.5f);
	}

	else if (KEYDOWN("Shop"))
	{
		GET_SINGLE(SceneManager)->CreateScene<SceneShop>("Shop", SC_NEXT);

		SetPos(32.f * 6.5f, 32.f * 17.f);
	}

	else if (KEYDOWN("WeaponShop"))
	{
		GET_SINGLE(SceneManager)->CreateScene<SceneWeaponShop>("WeaponShop", SC_NEXT);

		GET_SINGLE(Camera)->SetPos(0.f, 0.f);

		SetPos(32.f * 6.5f, 32.f * 17.5f);
	}
#endif
}

int Player::Update(float fDeltaTime)
{
	FightObj::Update(fDeltaTime);

	if (m_pArm)
	{
		m_pArm->SetPos(m_tPos.x, m_tPos.y - 0.5f * m_tSize.y);
		m_pArm->Update(fDeltaTime);
	}

	if (m_pPants)
	{
		m_pPants->SetPos(m_tPos.x, m_tPos.y - 0.5f * m_tSize.y);
		m_pPants->Update(fDeltaTime);
	}

	if (m_pShirt)
	{
		POSITION tSize = m_pShirt->GetSize();
		POSITION tPivot = m_pShirt->GetPivot();

		m_pShirt->SetPos(m_tPos-m_tPivot * m_tSize + tSize * tPivot);

		m_pShirt->Update(fDeltaTime);
	}

	if (m_pHair)
	{
		m_pHair->SetPos(m_tPos.x, m_tPos.y - 0.5f * m_tSize.y);
		m_pHair->Update(fDeltaTime);
	}

	if (m_bAttack && m_pAnimation->GetMotionEnd())
		m_bAttack = false;

	if (m_pAnimation->GetMotionEnd())
	{
		Collider* pCol = GetCollider("attack");
		pCol->SetEnable(false);
		SAFE_RELEASE(pCol);
	}

	UIInventory* pInven = GET_SINGLE(ObjManager)->GetInven();

	Item* pItem = nullptr;

	if(pInven)
	pItem = pInven->GetItem();

	SAFE_RELEASE(pInven);

	ITEM_TYPE eType = IT_NONE;

	if (pItem)
		eType = pItem->GetType();

	if (m_eAction == PA_IDLE || 
		(m_eAction == PA_ATTACK && m_pAnimation->GetMotionEnd()) ||
		(m_eAction == PA_FARM && m_pAnimation->GetMotionEnd()) ||
		(m_eAction == PA_WATER && m_pAnimation->GetMotionEnd()) ||
		(m_eAction == PA_EAT && m_pAnimation->GetMotionEnd()))
	{
		if (m_tMoveDir.x > 0.f)
		{
				m_pAnimation->SetCurrentClip("PlayerIdleRight");
		}

		else if (m_tMoveDir.x < 0.f)
		{
				m_pAnimation->SetCurrentClip("PlayerIdleLeft");
		}

		else if (m_tMoveDir.y > 0.f)
		{
				m_pAnimation->SetCurrentClip("PlayerIdleDown");
		}

		else if (m_tMoveDir.y < 0.f)
		{
				m_pAnimation->SetCurrentClip("PlayerIdleUp");
		}

		m_eAction = PA_IDLE;
	}

	if (m_eAction == PA_WALK)
		m_eAction = PA_IDLE;

	return 0;
}

int Player::LateUpdate(float fDeltaTime)
{
	FightObj::LateUpdate(fDeltaTime);

	m_bWalk = false;

	return 0;
}

void Player::Collision(float fDeltaTime)
{
	FightObj::Collision(fDeltaTime);
}

void Player::Render(HDC hDC, float fDeltaTime)
{
	FightObj::Render(hDC,fDeltaTime);

	if (m_pPants)
		m_pPants->Render(hDC, fDeltaTime);

	if (m_pShirt)
		m_pShirt->Render(hDC, fDeltaTime);

	if (m_tMoveDir.y < 0.f)
	{
		if (m_pArm)
			m_pArm->Render(hDC, fDeltaTime);
	}

	if (m_pHair)
		m_pHair->Render(hDC, fDeltaTime);

	if (m_tMoveDir.y >= 0.f)
	{
		if (m_pArm)
			m_pArm->Render(hDC, fDeltaTime);

		CWeapon* pWeapon = (CWeapon*)GET_SINGLE(ObjManager)->GetWeapon();

		if (pWeapon)
			pWeapon->Render(hDC, fDeltaTime);

		SAFE_RELEASE(pWeapon);
	}


	POSITION	tCamPos = GET_SINGLE(Camera)->GetPos();

	UIInventory* pInven = GET_SINGLE(ObjManager)->GetInven();

	Item* pItem = nullptr;

	if(pInven)
		pItem = pInven->GetItem();

	SAFE_RELEASE(pInven);

	if (pItem)
	{
		ITEM_TYPE eType = pItem->GetType();

		if (eType == IT_SEED || eType == IT_CROP || eType == IT_ETC)
		{
			Texture* pTexture = pItem->GetTexture();

			if (pTexture)
			{
				POSITION tSize = pItem->GetSize();
				POSITION tOffSet = pItem->GetImageOffset();
				POSITION tPos = m_tPos - m_tSize * m_tPivot - tCamPos;

				tPos.y -= tSize.y *0.5f;

				TransparentBlt(hDC, (int)tPos.x, (int)tPos.y, (int)tSize.x, (int)tSize.y, pTexture->GetDC(),
					(int)tOffSet.x, (int)tOffSet.y, (int)tSize.x, (int)tSize.y, pTexture->GetColorKey());

				SAFE_RELEASE(pTexture);
			}
		}
	}

	Stage* pStage = ((InGameScene*)GET_SINGLE(SceneManager)->GetScene())->GetStage();

	int iIndex = pStage->GetTileIndex(m_tPos);

	if (iIndex == -1)
		return;

	Tile* pTile = pStage->GetTile(iIndex);

	TILE_OPTION eOption = TO_NONE;

	if(pTile)
		eOption = pTile->GetTileOption();

	POSITION	tPos = m_tPos - m_tSize * m_tPivot;
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
		wsprintf(strHP, L"HP : %d", m_iHP);
		TextOut(hDC, (int)tPos.x + 100, (int)tPos.y - 100, strHP, lstrlen(strHP));
		wsprintf(strHP, L"E : %d", m_iEnergy);
		TextOut(hDC, (int)tPos.x + 100, (int)tPos.y - 120, strHP, lstrlen(strHP));
		wsprintf(strHP, TEXT("Layer: %d"), m_pLayer);
		TextOut(hDC, (int)tPos.x, (int)tPos.y, strHP, lstrlen(strHP));
		wsprintf(strHP, TEXT("Scene: %d"), m_pScene);
		TextOut(hDC, (int)tPos.x, (int)tPos.y - 20, strHP, lstrlen(strHP));
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

	if (strDest == "TreeBody" || strDest == "BuildingBody")
	{
		m_bColl = true;
	}

	else if (strDest == "MinionBody" || strDest == "SlimeBody" ||
		strDest == "BugBody" )
	{
		Obj* pObj = pDest->GetObj();

		POSITION tPos = pObj->GetPos();

		Hitted(((FightObj*)pObj)->GetAttack(), tPos);
	}

	if (strDest == "RockCrabBody")
	{
		Obj* pObj = pDest->GetObj();

		bool bAwake = ((RockCrab*)pObj)->GetAwake();

		if (bAwake)
		{
			POSITION tPos = pObj->GetPos();

			Hitted(((FightObj*)pObj)->GetAttack(), tPos);
		}
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

void Player::SetBarLayer(Layer* pLayer)
{
	m_pHPBar->SetScene(pLayer->GetScene());
	m_pHPBar->SetLayer(pLayer);

	m_pEnergyBar->SetScene(pLayer->GetScene());
	m_pEnergyBar->SetLayer(pLayer);
}

void Player::AddObjectToLayer(Layer* pLayer)
{
	if (m_pHPBar)
		pLayer->AddObject(m_pHPBar);

	if (m_pEnergyBar)
		pLayer->AddObject(m_pEnergyBar);
}
