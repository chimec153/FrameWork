#include "Building.h"
#include "../Resources/Texture.h"
#include "..//Core/Camera.h"
#include "..//Core/Input.h"

CBuilding::CBuilding()	:
	m_pDoorTex(nullptr),
	m_tDoorPivot(),
	m_tDoorSize()
{
}

CBuilding::CBuilding(const CBuilding& building)	:
	StaticObj(building)
{
	m_pDoorTex = building.m_pDoorTex;

	if (m_pDoorTex)
		m_pDoorTex->AddRef();

	m_tDoorPivot = building.m_tDoorPivot;
	m_tDoorSize = building.m_tDoorSize;
}

CBuilding::~CBuilding()
{
	SAFE_RELEASE(m_pDoorTex);
}

bool CBuilding::Init()
{
	return true;
}

void CBuilding::Input(float fDeltaTime)
{
	StaticObj::Input(fDeltaTime);
}

int CBuilding::Update(float fDeltaTime)
{
	StaticObj::Update(fDeltaTime);
	return 0;
}

int CBuilding::LateUpdate(float fDeltaTime)
{
	StaticObj::LateUpdate(fDeltaTime);
	return 0;
}

void CBuilding::Collision(float fDeltaTime)
{
	StaticObj::Collision(fDeltaTime);
}

void CBuilding::Render(HDC hDC, float fDeltaTime)
{
	StaticObj::Render(hDC, fDeltaTime);
	POSITION	tPos = m_tPos - m_tDoorSize * m_tDoorPivot;
	tPos -= GET_SINGLE(Camera)->GetPos();

	RESOLUTION	tClientRS = GET_SINGLE(Camera)->GetClientRS();

	bool	bInClient = true;

	if (tPos.x + m_tDoorSize.x < 0)
		bInClient = false;
	else if (tPos.x > tClientRS.iW)
		bInClient = false;
	else if (tPos.y + m_tDoorSize.y < 0)
		bInClient = false;
	else if (tPos.y > tClientRS.iH)
		bInClient = false;

	if (m_pDoorTex && bInClient)
	{
		POSITION	tImagePos = {};

		//tImagePos += m_tImageOffset;

		if (m_pDoorTex->GetColorKeyEnable())
		{
			TransparentBlt(hDC, (int)tPos.x, (int)tPos.y, (int)m_tDoorSize.x,
				(int)m_tDoorSize.y, m_pTexture->GetDC(), (int)tImagePos.x, (int)tImagePos.y,
				(int)m_tDoorSize.x, (int)m_tDoorSize.y, m_pDoorTex->GetColorKey());
		}
		else
		{
			BitBlt(hDC,(int)tPos.x, (int)tPos.y, (int)m_tDoorSize.x,
				(int)m_tDoorSize.y, m_pDoorTex->GetDC(), (int)tImagePos.x, (int)tImagePos.y,
				SRCCOPY);
		}
	}

	if (KEYPRESS("Debug"))
	{
		Rectangle(hDC, (int)(m_tPos.x - 2.f), (int)(m_tPos.y - 2.f), (int)(m_tPos.x + 2.f), (int)(m_tPos.y + 2.f));
	}
}

CBuilding* CBuilding::Clone()
{
	return new CBuilding(*this);
}

void CBuilding::Save(FILE* pFile)
{
	StaticObj::Save(pFile);

	bool bTex = m_pDoorTex;
	fwrite(&bTex, sizeof(bool) * 1, 1, pFile);

	if (m_pDoorTex)
		m_pDoorTex->Save(pFile);

	fwrite(&m_tDoorPivot, sizeof(POSITION), 1, pFile);
	fwrite(&m_tDoorSize, sizeof(POSITION), 1, pFile);
}

void CBuilding::Load(FILE* pFile)
{
	StaticObj::Load(pFile);

	bool bTex = false;
	fread(&bTex, sizeof(bool) * 1, 1, pFile);

	if (bTex)
	{
		SAFE_RELEASE(m_pDoorTex);
		m_pDoorTex= new Texture;
		m_pDoorTex->Load(pFile);
	}		

	fread(&m_tDoorPivot, sizeof(POSITION), 1, pFile);
	fread(&m_tDoorSize, sizeof(POSITION), 1, pFile);
}

void CBuilding::Hit(Collider* pSrc, Collider* pDest, float fDeltaTime)
{
	if (pDest->GetTag() == "Mouse")
	{
		if (KEYPRESS("MouseRButton"))
			Die();
	}
}

void CBuilding::HitStay(Collider* pSrc, Collider* pDest, float fDeltaTime)
{
	if (pDest->GetTag() == "Mouse")
	{
		if (KEYPRESS("MouseRButton"))
			Die();
	}
}
