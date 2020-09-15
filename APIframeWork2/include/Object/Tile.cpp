#include "Tile.h"
#include "../Resources/ResourcesManager.h"
#include "../Resources/Texture.h"
#include "../Core/Camera.h" 
#include "..//Core/Input.h"
#include "Tree.h"
#include "..//Scene/SceneManager.h"
#include "..//Scene/Scene.h"
#include "..//Collider/ColliderRect.h"
#include "..//Core.h"
#include "../Object/Stage.h"
#include "FarmEffect.h"
#include "Building.h"
#include "Etc.h"
#include "Crop.h"
#include "Rock.h"
#include "Grass.h"	
#include "../Animation/Animation.h"

Tile::Tile()	:
	m_eOption(TO_NONE),
	m_pOptionTex(nullptr),
	m_pObj(nullptr),
	m_pUpperTex(nullptr),
	m_tUpperImageOffset(),
	m_pSelectTex(nullptr),
	m_bSelect(false),
	m_pStage(nullptr)
{
}

Tile::Tile(const Tile & tile)	:
	StaticObj(tile)
{
	m_eOption = tile.m_eOption;
	m_pOptionTex = tile.m_pOptionTex;
	m_pSelectTex = tile.m_pSelectTex;
	m_pUpperTex = tile.m_pUpperTex;

	m_pObj = nullptr;

	if(m_pOptionTex)
		m_pOptionTex->AddRef();

	if (m_pSelectTex)
		m_pSelectTex->AddRef();

	if (m_pUpperTex)
		m_pUpperTex->AddRef();

	m_pStage = tile.m_pStage;

	m_bSelect = false;
}

Tile::~Tile()
{
	SAFE_RELEASE(m_pOptionTex);
	SAFE_RELEASE(m_pUpperTex);
	SAFE_RELEASE(m_pSelectTex);
	SAFE_RELEASE(m_pObj);
}

void Tile::SetUpperTexture(Texture* pTexture)
{
	SAFE_RELEASE(m_pUpperTex);
	m_pUpperTex = pTexture;

	if (pTexture)
		pTexture->AddRef();
}

void Tile::SetUpperTexture(const string& strKey, const wchar_t* pFileName, const string& strPathKey)
{
	SAFE_RELEASE(m_pUpperTex);
	m_pUpperTex = GET_SINGLE(ResourcesManager)->LoadTexture(
		strKey, pFileName, strPathKey);
}

void Tile::SetUpperColorKey(unsigned char r, unsigned char g, unsigned char b)
{
	m_pUpperTex->SetColorKey(r, g, b);
}

void Tile::SetTileOption(TILE_OPTION eOption)
{
	m_eOption = eOption;
	Obj* pEffect = nullptr;
	Layer* pLayer = nullptr;
	Animation* pAni = nullptr;

	switch (eOption)
	{
	case TO_HOEDIRT:
		SAFE_RELEASE(m_pUpperTex);
		m_pUpperTex = GET_SINGLE(ResourcesManager)->FindTexture("TileHoeDirt");
		SetUpperColorKey(255, 255, 255);

		pLayer = m_pScene->FindLayer("Default");

		pEffect = CreateCloneObj("HoeEffect", "HoeEffect", pLayer);

		pEffect->SetAnimationCurrentClip("HoeDirt");

		pEffect->SetPos(m_tPos);

		SAFE_RELEASE(pEffect);

		break;
	case TO_WATERDIRT:
		SAFE_RELEASE(m_pUpperTex);
		m_pUpperTex = GET_SINGLE(ResourcesManager)->FindTexture("TileHoeDirtDark");
		SetUpperColorKey(255, 255, 255);

		pLayer = m_pScene->FindLayer("Default");

		pEffect = CreateCloneObj("HoeEffect", "WaterEffect", pLayer);
		if (pEffect)
		{
			pEffect->SetAnimationCurrentClip("WaterSplashing");

			pEffect->SetPos(m_tPos);
		}
		SAFE_RELEASE(pEffect);

		break;
	case TO_NONE:
		SAFE_RELEASE(m_pOptionTex);
		m_pOptionTex = nullptr;
		break;
	case TO_NOMOVE:
		SAFE_RELEASE(m_pOptionTex);
		m_pOptionTex = GET_SINGLE(ResourcesManager)->FindTexture("TileNoMove");
		break;
	case TO_TREEANI:
		pAni = CreateAnimation("TileAni");

		pAni->LoadFromPath("Tile.sac");

		SAFE_RELEASE(pAni);

		SetImageOffset(0.f, 0.f);
		break;
	}
}

void Tile::SelectTile()
{
	if (m_bSelect)
		return;

	m_bSelect = true;

	SAFE_RELEASE(m_pSelectTex);
	
	m_pSelectTex = GET_SINGLE(ResourcesManager)->FindTexture("Mouse");
}

Texture* Tile::GetUpperTexture()	const
{
	if (m_pUpperTex)
		m_pUpperTex->AddRef();

	return m_pUpperTex;
}

bool Tile::Init()
{
	Texture* pTexture = GET_SINGLE(ResourcesManager)->LoadTexture("TileHoeDirtDark", TEXT("TileSheets\\hoeDirtDark.bmp"));
	pTexture->SetColorKey(255, 255, 255);
	SAFE_RELEASE(pTexture);

	pTexture = GET_SINGLE(ResourcesManager)->LoadTexture("TileHoeDirt", TEXT("TileSheets\\hoeDirt.bmp"));
	pTexture->SetColorKey(255, 255, 255);
	SAFE_RELEASE(pTexture);

	return true;
}

void Tile::Input(float fDeltaTime)
{
	//StaticObj::Input(fDeltaTime);	
}

int Tile::Update(float fDeltaTime)
{
	StaticObj::Update(fDeltaTime);

	return 0;
}

int Tile::LateUpdate(float fDeltaTime)
{
	//StaticObj::LateUpdate(fDeltaTime);
	return 0;
}

void Tile::Collision(float fDeltaTime)
{
	//StaticObj::Collision(fDeltaTime);
}

void Tile::Render(HDC hDC, float fDeltaTime)
{
	StaticObj::Render(hDC, fDeltaTime);

	POSITION	tPos = m_tPos - m_tSize * m_tPivot;
	tPos -= GET_SINGLE(Camera)->GetPos();

	RESOLUTION	tClientRS = GET_SINGLE(Camera)->GetClientRS();

	bool	bInClient = true;
	if (tPos.x + m_tSize.x < 0)
		bInClient = false;
	else if (tPos.x > tClientRS.iW)
		bInClient = false;
	else if (tPos.y + m_tSize.y < 0)
		bInClient = false;
	else if (tPos.y > tClientRS.iH)
		bInClient = false;

	if(m_pUpperTex)
	{
		POSITION	tImagePos;

		tImagePos += m_tUpperImageOffset;

		if (m_pUpperTex && bInClient)
		{
			if (!m_bAlphaOn)
			{
				if (m_pUpperTex->GetColorKeyEnable())
				{
					TransparentBlt(hDC, (int)tPos.x, (int)tPos.y, (int)m_tSize.x,
						(int)m_tSize.y, m_pUpperTex->GetDC(), (int)tImagePos.x, (int)tImagePos.y,
						(int)m_tSize.x, (int)m_tSize.y, m_pUpperTex->GetColorKey());
				}
				else
				{
					BitBlt(hDC, (int)tPos.x, (int)tPos.y, (int)m_tSize.x,
						(int)m_tSize.y, m_pUpperTex->GetDC(), (int)tImagePos.x, (int)tImagePos.y,
						SRCCOPY);
				}
			}

			else
			{
				BLENDFUNCTION	tBF = {};

				tBF.BlendOp = 0;
				tBF.BlendFlags = 0;
				tBF.SourceConstantAlpha = m_cAlpha;

				tBF.AlphaFormat = AC_SRC_ALPHA;

				GdiAlphaBlend(hDC, (int)tPos.x, (int)tPos.y, (int)m_tSize.x, (int)m_tSize.y,
					m_pUpperTex->GetDC(), (int)tImagePos.x, (int)tImagePos.y, (int)m_tSize.x, (int)m_tSize.y, tBF);
			}
		}
	}

	if (m_bSelect)
	{
		if (bInClient)
		{
			POSITION	tImagePos = {};

			tImagePos.y += 64.f;

			if (m_pSelectTex->GetColorKeyEnable())
			{
				TransparentBlt(hDC, (int)tPos.x, (int)tPos.y, (int)m_tSize.x,
					(int)m_tSize.y, m_pSelectTex->GetDC(), (int)tImagePos.x, (int)tImagePos.y,
					(int)m_tSize.x, (int)m_tSize.y, m_pSelectTex->GetColorKey());
			}

			else
			{
				BitBlt(hDC, (int)tPos.x, (int)tPos.y, (int)m_tSize.x,
					(int)m_tSize.y, m_pSelectTex->GetDC(), (int)tImagePos.x, (int)tImagePos.y,
					SRCCOPY);
			}
		}
		m_bSelect = false;
	}
#ifdef _DEBUG
	if (GetAsyncKeyState(VK_TAB) & 0x8000)
	{
		if (m_pOptionTex && bInClient)
		{
			POSITION	tImagePos;

			//tImagePos += m_tImageOffset;

			if (m_pOptionTex->GetColorKeyEnable())
			{
				TransparentBlt(hDC, (int)tPos.x, (int)tPos.y, (int)m_tSize.x,
					(int)m_tSize.y, m_pOptionTex->GetDC(), (int)tImagePos.x, (int)tImagePos.y,
					(int)m_tSize.x, (int)m_tSize.y, m_pOptionTex->GetColorKey());
			}
			else
			{
				BitBlt(hDC, (int)tPos.x, (int)tPos.y, (int)m_tSize.x,
					(int)m_tSize.y, m_pOptionTex->GetDC(), (int)tImagePos.x, (int)tImagePos.y,
					SRCCOPY);
			}
		}

		RECT tRC = { (int)tPos.x, (int)tPos.y, (int)(tPos.x + m_tSize.x),(int)(tPos.y + m_tSize.y) };

		switch (m_eOption)
		{
		case TO_DIRT:	
			FrameRect(hDC, &tRC, ORANGEBRUSH);
			break;
		case TO_NOMOVE:
			FrameRect(hDC, &tRC, REDBRUSH);
			break;
		case TO_WATER:
			FrameRect(hDC, &tRC, BLUEBRUSH);
			break;
		}
	}

#endif
}

Tile * Tile::Clone()
{
	return new Tile(*this);
}


void Tile::Save(FILE * pFile)
{
	StaticObj::Save(pFile);
	fwrite(&m_eOption, 4, 1, pFile);

	bool bObj = true;
	if (m_pObj)
	{
		fwrite(&bObj, sizeof(bool), 1, pFile);

		OBJ_BLOCK eBlock = m_pObj->GetBlock();

		fwrite(&eBlock, sizeof(OBJ_BLOCK), 1, pFile);

		m_pObj->Save(pFile);
	}
	else
	{
		bObj = false;
		fwrite(&bObj, sizeof(bool), 1, pFile);
	}

	fwrite(&m_tUpperImageOffset, sizeof(POSITION), 1, pFile);

	bool	bTexture = false;
	if (m_pUpperTex)
	{
		bTexture = true;
		fwrite(&bTexture, 1, 1, pFile);
		m_pUpperTex->Save(pFile);
	}
	else
	{
		fwrite(&bTexture, 1, 1, pFile);
	}
}

void Tile::Load(FILE * pFile)
{
	StaticObj::Load(pFile);
	fread(&m_eOption, 4, 1, pFile);
	SetTileOption(m_eOption);

	if (m_eOption == TO_NOMOVE)
	{
		ColliderRect* pRC = AddCollider<ColliderRect>("TileBody");
		pRC->SetRect(0.f, 0.f, 32.f, 32.f);
		SAFE_RELEASE(pRC);
	}

	bool bObj = false;

	fread(&bObj, sizeof(bool), 1, pFile);

	if (bObj)
	{
		OBJ_BLOCK eBlock = OB_NONE;

		fread(&eBlock, sizeof(OBJ_BLOCK), 1, pFile);

		Obj* pObj = nullptr;

		Layer* pLayer = m_pScene->FindLayer("Default");

		switch (eBlock)
		{
		case OB_NONE:
			pObj = Obj::CreateObj<Grass>("", pLayer);
			break;
		case OB_TREE:
			pObj = Obj::CreateObj<CTree>("", pLayer);
			break;
		case OB_BUILDING:
			pObj = Obj::CreateObj<CBuilding>("", pLayer);
			break;
		case OB_CROP:
			pObj = Obj::CreateObj<Crop>("", pLayer);
			break;
		case OB_ROCK:
			pObj = Obj::CreateObj<Rock>("", pLayer);
			break;
		case OB_GRASS:
			pObj = Obj::CreateObj<Grass>("", pLayer);
			break;
		}		

		pObj->Load(pFile);

		SetObj(pObj);

		SAFE_RELEASE(pObj);
	}

	fread(&m_tUpperImageOffset, sizeof(POSITION), 1, pFile);

	bool	bTexture = false;
	fread(&bTexture, 1, 1, pFile);
	SAFE_RELEASE(m_pUpperTex);
	if (bTexture)
	{
		m_pUpperTex = GET_SINGLE(ResourcesManager)->LoadTexture(pFile);
	}
}
