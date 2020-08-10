#include "Tree.h"
#include "..//Resources/ResourcesManager.h"
#include "..//Resources/Texture.h"
#include "..//Core/Camera.h"
#include "../Collider/ColliderRect.h"
#include "..//Core/Input.h"
#include "..//Scene/SceneManager.h"
#include "..//Scene/Scene.h"

CTree::CTree() :
	m_pTreeSprite(nullptr),
	m_bCutted(false),
	m_tTreePivot(),
	m_tTreeSize(),
	m_tTreeOffset(),
	m_bMouseOn(false),
	m_bLeavesRender(true)
{
}

CTree::CTree(const CTree& tree)	:
	StaticObj(tree)
{
	m_pTreeSprite = tree.m_pTreeSprite;
	m_bCutted = tree.m_bCutted;
	m_tTreePivot = tree.m_tTreePivot;
	m_tTreeSize = tree.m_tTreeSize;
	m_tTreeOffset = tree.m_tTreeOffset;

	if (m_pTreeSprite)
		m_pTreeSprite->AddRef();
	
	AddCollisionFunc();
}

CTree::~CTree()
{
	SAFE_RELEASE(m_pTreeSprite)
}

bool CTree::Init()
{
	return true;
}

void CTree::Input(float fDeltaTime)
{
}

int CTree::Update(float fDeltaTime)
{
	StaticObj::Input(fDeltaTime);
	return 0;
}

int CTree::LateUpdate(float fDeltaTime)
{
	StaticObj::LateUpdate(fDeltaTime);
	m_bMouseOn = false;
	m_bLeavesRender = true;
	return 0;
}

void CTree::Collision(float fDeltaTime)
{
	StaticObj::Collision(fDeltaTime);
}

void CTree::Render(HDC hDC, float fDeltaTime)
{
	StaticObj::Render(hDC, fDeltaTime);
	if (m_bLeavesRender)
	{
		POSITION	tPos = m_tPos - m_tTreeSize * m_tTreePivot;
		tPos -= GET_SINGLE(Camera)->GetPos();

		RESOLUTION	tClientRS = GET_SINGLE(Camera)->GetClientRS();

		bool	bInClient = true;

		if (tPos.x + m_tTreeSize.x < 0)
			bInClient = false;
		else if (tPos.x > tClientRS.iW)
			bInClient = false;
		else if (tPos.y + m_tTreeSize.y < 0)
			bInClient = false;
		else if (tPos.y > tClientRS.iH)
			bInClient = false;

		if (m_pTreeSprite && bInClient)
		{
			POSITION	tImagePos = {};

			tImagePos += m_tTreeOffset;

			if (m_pTreeSprite->GetColorKeyEnable())
			{
				TransparentBlt(hDC, (int)tPos.x, (int)tPos.y, (int)m_tTreeSize.x,
					(int)m_tTreeSize.y, m_pTexture->GetDC(), (int)tImagePos.x, (int)tImagePos.y,
					(int)m_tTreeSize.x, (int)m_tTreeSize.y, m_pTreeSprite->GetColorKey());
			}
			else
			{
				BitBlt(hDC, (int)tPos.x, (int)tPos.y, (int)m_tTreeSize.x,
					(int)m_tTreeSize.y, m_pTreeSprite->GetDC(), (int)tImagePos.x, (int)tImagePos.y,
					SRCCOPY);
			}
		}
	}
	if (KEYPRESS("Debug"))
	{
		Rectangle(hDC, (int)m_tPos.x - 2, (int)m_tPos.y - 2, (int)m_tPos.x + 2, (int)m_tPos.y + 2);
	}

	if(m_bMouseOn)
		Rectangle(hDC, (int)m_tPos.x - 4, (int)m_tPos.y - 4, (int)m_tPos.x + 4, (int)m_tPos.y + 4);
}

CTree* CTree::Clone()
{
	return new CTree(*this);
}

void CTree::Save(FILE* pFile)
{
	StaticObj::Save(pFile);

	bool	bTexture = false;
	if (m_pTreeSprite)
	{
		bTexture = true;
		fwrite(&bTexture, 1, 1, pFile);
		m_pTreeSprite->Save(pFile);
	}
	else
	{
		fwrite(&bTexture, 1, 1, pFile);
	}

	fwrite(&m_bCutted, 1, 1, pFile);

	fwrite(&m_tTreePivot, sizeof(POSITION), 1, pFile);
	fwrite(&m_tTreeSize, sizeof(POSITION), 1, pFile);
	fwrite(&m_tTreeOffset, sizeof(POSITION), 1, pFile);
}

void CTree::Load(FILE* pFile)
{
	StaticObj::Load(pFile);

	bool	bTexture = false;
	fread(&bTexture, 1, 1, pFile);

	SAFE_RELEASE(m_pTreeSprite);
	if (bTexture)
	{
		m_pTreeSprite = GET_SINGLE(ResourcesManager)->LoadTexture(pFile);
	}

	fread(&m_bCutted, 1, 1, pFile);

	fread(&m_tTreePivot, sizeof(POSITION), 1, pFile);
	fread(&m_tTreeSize, sizeof(POSITION), 1, pFile);
	fread(&m_tTreeOffset, sizeof(POSITION), 1, pFile);

	AddCollisionFunc();
}

void CTree::AddCollisionFunc()	//	충돌 콜백함수를 등록하는 함수이다.
{
	ColliderRect* pRC = (ColliderRect*)GetCollider("TreeBody");

	if (pRC)
	{
		pRC->AddCollisionFunction(CS_ENTER, this,
			&CTree::Hit);
		pRC->AddCollisionFunction(CS_STAY, this,
			&CTree::HitStay);

		SAFE_RELEASE(pRC);
	}

	pRC = (ColliderRect*)GetCollider("TreeLeavesBody");

	if (pRC)
	{
		pRC->AddCollisionFunction(CS_ENTER, this,
			&CTree::Hit);
		pRC->AddCollisionFunction(CS_STAY, this,
			&CTree::HitStay);

		SAFE_RELEASE(pRC);
	}

	pRC = (ColliderRect*)GetCollider("BuildingBody");

	if (pRC)
	{
		pRC->AddCollisionFunction(CS_ENTER, this,
			&CTree::Hit);
		pRC->AddCollisionFunction(CS_STAY, this,
			&CTree::HitStay);

		SAFE_RELEASE(pRC);
	}

	pRC = (ColliderRect*)GetCollider("BuildingDoorBody");

	if (pRC)
	{
		pRC->AddCollisionFunction(CS_ENTER, this,
			&CTree::Hit);
		pRC->AddCollisionFunction(CS_STAY, this,
			&CTree::HitStay);

		SAFE_RELEASE(pRC);
	}
}

void CTree::Hit(Collider* pSrc, Collider* pDest, float fDeltaTime)
{
	if (pDest->GetTag() == "Mouse")
	{
		m_bMouseOn = true;
		if (KEYPRESS("MouseRButton"))
		{
			bool bEdit = GET_SINGLE(SceneManager)->GetScene()->IsEditMode();

			if(bEdit)
				Die();
		}
	}
	else if (pDest->GetTag() == "PlayerBody" && pSrc->GetTag() == "TreeLeavesBody")
	{
		m_bLeavesRender = false;
	}
}

void CTree::HitStay(Collider* pSrc, Collider* pDest, float fDeltaTime)
{
	if (pDest->GetTag() == "Mouse")
	{
		m_bMouseOn = true;
		if (KEYPRESS("MouseRButton"))
		{
			bool bEdit = GET_SINGLE(SceneManager)->GetScene()->IsEditMode();

			if (bEdit)
				Die();
		}
	}
	else if (pDest->GetTag() == "PlayerBody" && pSrc->GetTag() == "TreeLeavesBody")
	{
		m_bLeavesRender = false;
	}
}
