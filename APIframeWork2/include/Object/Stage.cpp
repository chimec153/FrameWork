#include "Stage.h"
#include "../Resources/Texture.h"
#include "../Core.h"
#include "../Core/Camera.h"
#include "Tile.h"
#include "../Core/PathManager.h"
#include "../Core/Input.h"
#include "..//Scene/Scene.h"
#include "Player.h"
#include "../Core/Timer.h"
#include "UIClockHand.h"

Stage::Stage()	:
	m_iTileNumX(0),
	m_iTileNumY(0),
	m_iTileSizeX(0),
	m_iTileSizeY(0),
	m_ppBlock(nullptr),	//	타일위에 있는 오브젝트 정보를 담고 있는 블록 이중포인터 배열이다.
	m_pSelectTile(nullptr)
{
}

Stage::Stage(const Stage & stage)	:
	StaticObj(stage)
{
	m_vecTile.clear();

	for (size_t i = 0; i < stage.m_vecTile.size(); ++i)
	{
		m_vecTile.push_back(stage.m_vecTile[i]->Clone());
	}

	m_iTileNumX = stage.m_iTileNumX;
	m_iTileNumY = stage.m_iTileNumY;
	m_iTileSizeX = stage.m_iTileSizeX;
	m_iTileSizeY = stage.m_iTileSizeY;

	m_ppBlock = new char* [m_iTileNumY];

	for (int i = 0; i < m_iTileNumY; ++i)
	{
		m_ppBlock[i] = new char[m_iTileNumX];

		memcpy(m_ppBlock[i], stage.m_ppBlock[i], sizeof(char) * m_iTileNumX);
	}

	m_pSelectTile = nullptr;
}

Stage::~Stage()
{
	ClearTile();
}

void Stage::CreateTile(int iNumX, int iNumY, 
	int iSizeX, int iSizeY, const string & strKey, 
	const wchar_t * pFileName, const string & strPathKey)
{
	ClearTile();

	m_iTileNumX = iNumX;
	m_iTileNumY = iNumY;
	m_iTileSizeX = iSizeX;
	m_iTileSizeY = iSizeY;

	m_ppBlock = new char* [iNumY];

	for (int i = 0; i < iNumY; ++i)
	{
		m_ppBlock[i] = new char[iNumX];
		memset(m_ppBlock[i], 0, iNumX);

		for (int j = 0; j < iNumX; ++j)
		{
			Tile* pTile = Obj::CreateObj<Tile>("Tile");

			pTile->SetSize((float)iSizeX, (float)iSizeY);
			pTile->SetPos((float)j*iSizeX + m_tStart.x, (float)i*iSizeY + m_tStart.y);

			pTile->SetImageOffset(32.f, 224.f);

			pTile->SetTexture(strKey, pFileName, strPathKey);
			pTile->SetStage(this);
			pTile->m_pScene = m_pScene;
			pTile->m_pLayer = m_pLayer;
			pTile->SetAlpha(255);
			pTile->EnableAlpha(true);

			m_vecTile.push_back(pTile);
		}
	}

	GET_SINGLE(Camera)->SetWorldResolution(m_iTileNumX* m_iTileSizeX, m_iTileNumY* m_iTileSizeY);
}

bool Stage::Init()
{
	SetPos(0.f, 0.f);
	SetSize(1280.f, 800.f);
	SetPivot(0.f, 0.f);

	SetTexture("Stage", TEXT("AnimationUI\\BackGround.bmp"));
	return true;
}

void Stage::Input(float fDeltaTime)
{
	StaticObj::Input(fDeltaTime);
}

int Stage::Update(float fDeltaTime)
{
	StaticObj::Update(fDeltaTime);

	POSITION tMousePos = GET_SINGLE(Input)->GetMouseWorldPos();

	Obj* pPlayer = m_pScene->GetPlayer();

	if (pPlayer)
	{
		POSITION tPos = pPlayer->GetPos();
		POSITION tAngle = ((MoveObj*)pPlayer)->GetAngle();

		SAFE_RELEASE(pPlayer);

		POSITION tTilePos = {};
		Tile* pTile = nullptr;

		tTilePos.x = (float)((int)(tPos.x / m_iTileSizeX) * m_iTileSizeX);
		tTilePos.y = (float)((int)(tPos.y / m_iTileSizeY) * m_iTileSizeY);

		if (tMousePos.x > tTilePos.x - m_iTileSizeX &&
			tMousePos.x < tTilePos.x + 2 * m_iTileSizeX &&
			tMousePos.y > tTilePos.y - m_iTileSizeY &&
			tMousePos.y < tTilePos.y + 2 * m_iTileSizeY)
			pTile = GetTile(tMousePos);			

		else
			pTile = GetTile(tPos + tAngle * 32.f);

		if (pTile)
		{
			m_pSelectTile = pTile;

			pTile->SelectTile();
		}
	}

	UIClockHand* pHand = (UIClockHand*)GET_SINGLE(Timer)->GetClockHand();

	bool bRain = false;

	if(pHand)
		bRain = pHand->IsRain();

	for (int i = 0; i < m_iTileNumY; ++i)
	{
		for (int j = 0; j < m_iTileNumX; ++j)
		{
			TILE_OPTION eOption = m_vecTile[i * m_iTileNumX + j]->GetTileOption();
			POSITION tOffset = m_vecTile[i * m_iTileNumX + j]->GetUpperImageOffset();

			if (eOption != TO_WATERDIRT &&
				eOption != TO_HOEDIRT)
				continue;

			if (eOption == TO_HOEDIRT && bRain)
				m_vecTile[i * m_iTileNumX + j]->SetTileOption(TO_WATERDIRT);

			Tile* pLeftTile = nullptr;

			if (j > 0)
				pLeftTile = m_vecTile[i * m_iTileNumX + j - 1];

			Tile* pRightTile = nullptr;

			if (j < m_iTileNumX - 1)
				pRightTile = m_vecTile[i * m_iTileNumX + j + 1];

			TILE_OPTION eLeftOption = TO_NONE;
			POSITION tLeftOffset(0.f, 0.f);

			if (pLeftTile)
			{
				eLeftOption = pLeftTile->GetTileOption();
				tLeftOffset = pLeftTile->GetUpperImageOffset();
			}				

			TILE_OPTION eRightOption = TO_NONE;
			POSITION tRightOffset(0.f, 0.f);

			if (pRightTile)
			{
				eRightOption = pRightTile->GetTileOption();
				tRightOffset = pRightTile->GetUpperImageOffset();
			}				

			if ((eLeftOption == TO_HOEDIRT || eLeftOption == TO_WATERDIRT) &&
				(eRightOption == TO_HOEDIRT || eRightOption == TO_WATERDIRT) &&
				tLeftOffset.x != 0.f && tRightOffset.x !=0.f &&
				((tOffset.x ==0.f && tOffset.y == 0.f) ||
				(tOffset.x !=0.f)))
			{
				m_vecTile[i * m_iTileNumX + j]->SetUpperImageOffset(m_iTileSizeX * 2.f, m_iTileSizeY * 3.f);
				continue;
			}

			else if ((eLeftOption == TO_HOEDIRT || eLeftOption == TO_WATERDIRT) &&
				(tLeftOffset.x != 0.f || tLeftOffset.y == 0.f) &&
				((tOffset.x == 0.f && tOffset.y == 0.f) ||
				(tOffset.x != 0.f)))
			{
				m_vecTile[i * m_iTileNumX + j]->SetUpperImageOffset(m_iTileSizeX * 3.f, m_iTileSizeY * 3.f);
				continue;
			}

			else if ((eRightOption == TO_HOEDIRT || eRightOption == TO_WATERDIRT) &&
				(tRightOffset.x != 0.f || tRightOffset.y == 0.f) &&
				((tOffset.x == 0.f && tOffset.y == 0.f) ||
				(tOffset.x != 0.f)))
			{
				m_vecTile[i * m_iTileNumX + j]->SetUpperImageOffset((float)m_iTileSizeX , m_iTileSizeY * 3.f);
				continue;
			}

			Tile* pUpTile = nullptr;

			if (i > 0)
				pUpTile = m_vecTile[(i-1) * m_iTileNumX + j];

			Tile* pDownTile = nullptr;

			if (i < m_iTileNumY - 1)
				pDownTile = m_vecTile[(i+1) * m_iTileNumX + j];

			TILE_OPTION eUpOption = TO_NONE;
			POSITION	tUpOffset(0.f, 0.f);

			if (pUpTile)
			{
				eUpOption = pUpTile->GetTileOption();
				tUpOffset = pUpTile->GetUpperImageOffset();
			}

			TILE_OPTION eDownOption = TO_NONE;
			POSITION	tDownOffset(0.f, 0.f);

			if (pDownTile)
			{
				eDownOption = pDownTile->GetTileOption();
				tDownOffset = pDownTile->GetUpperImageOffset();
			}

			if ((eUpOption == TO_HOEDIRT || eUpOption == TO_WATERDIRT) &&
				(eDownOption == TO_HOEDIRT || eDownOption == TO_WATERDIRT) &&
				tUpOffset.x == 0.f && tDownOffset.x ==0.f)
			{
				m_vecTile[i * m_iTileNumX + j]->SetUpperImageOffset(0, m_iTileSizeY * 2.f);
				continue;
			}

			else if ((eUpOption == TO_HOEDIRT || eUpOption == TO_WATERDIRT) &&
				tUpOffset.x == 0.f &&
				tOffset.x == 0.f)
			{
				m_vecTile[i * m_iTileNumX + j]->SetUpperImageOffset(0, m_iTileSizeY * 3.f);
				continue;
			}

			else if ((eDownOption == TO_HOEDIRT || eDownOption == TO_WATERDIRT) &&
				tDownOffset.x  == 0.f &&
				tOffset.x == 0.f)
			{
				m_vecTile[i * m_iTileNumX + j]->SetUpperImageOffset(0, (float)m_iTileSizeY);
				continue;
			}

			else
				m_vecTile[i * m_iTileNumX + j]->SetUpperImageOffset(0.f, 0.f);
		}
	}

	return 0;
}

int Stage::LateUpdate(float fDeltaTime)
{
	StaticObj::LateUpdate(fDeltaTime);
	return 0;
}

void Stage::Collision(float fDeltaTime)
{
	StaticObj::Collision(fDeltaTime);
}

void Stage::Render(HDC hDC, float fDeltaTime)
{
	//StaticObj::Render(hDC, fDeltaTime);
	if (m_pTexture)
	{
		POSITION	tPos = m_tPos - m_tSize * m_tPivot;
		POSITION	tCamPos = GET_SINGLE(Camera)->GetPos();
		BitBlt(hDC, (int)tPos.x, (int)tPos.y, (int)GETRESOLUTION.iW,
			(int)GETRESOLUTION.iH, m_pTexture->GetDC(), (int)tCamPos.x, (int)tCamPos.y,
			SRCCOPY);
	}

	for (size_t i = 0; i < m_vecTile.size(); ++i)
	{
		m_vecTile[i]->Render(hDC, fDeltaTime);
	}
	
	if (m_pScene->IsEditMode())
	{
		POSITION	tCamPos = GET_SINGLE(Camera)->GetPos();
		for (int i = 1; i < m_iTileNumY; ++i)
		{
			MoveToEx(hDC, 0, (int)(i * m_iTileSizeY - tCamPos.y), NULL);
			LineTo(hDC, (int)(m_iTileNumX * m_iTileSizeX), (int)(i * m_iTileSizeY - tCamPos.y));
		}
		for (int i = 1; i < m_iTileNumX; ++i)
		{
			MoveToEx(hDC, (int)(i * m_iTileSizeX - tCamPos.x), 0, NULL);
			LineTo(hDC, (int)(i * m_iTileSizeX - tCamPos.x), (int)(m_iTileNumY * m_iTileSizeY));
		}
	}

#ifdef _DEBUG
		list<Collider*>::iterator	iter;
		list<Collider*>::iterator	iterEnd = m_ColliderList.end();

		for (iter = m_ColliderList.begin(); iter != iterEnd;)
		{
			if (!(*iter)->GetEnable())
			{
				++iter;
				continue;
			}
			(*iter)->Render(hDC, fDeltaTime);
			if (!(*iter)->GetLife())
			{
				SAFE_RELEASE((*iter));
				iter = m_ColliderList.erase(iter);
				iterEnd = m_ColliderList.end();
			}
			else
				++iter;
		}
#endif
}

Stage * Stage::Clone()
{
	return new Stage(*this);
}

void Stage::Save(FILE * pFile)
{
	StaticObj::Save(pFile);
	fwrite(&m_iTileNumX,	4, 1, pFile);
	fwrite(&m_iTileNumY,	4, 1, pFile);
	fwrite(&m_iTileSizeX,	4, 1, pFile);
	fwrite(&m_iTileSizeY,	4, 1, pFile);

	for (size_t i = 0; i < m_vecTile.size(); ++i)
	{
		m_vecTile[i]->Save(pFile);
	}

}

void Stage::Load(FILE * pFile)
{
	ClearTile();

	StaticObj::Load(pFile);
	fread(&m_iTileNumX, 4, 1, pFile);
	fread(&m_iTileNumY, 4, 1, pFile);
	fread(&m_iTileSizeX, 4, 1, pFile);
	fread(&m_iTileSizeY, 4, 1, pFile);

	m_ppBlock = new char* [m_iTileNumY];

	for (int i = 0; i < m_iTileNumY; ++i)
	{
		m_ppBlock[i] = new char[m_iTileNumX];
		memset(m_ppBlock[i], 0, m_iTileNumX);
	}

	for (int i = 0; i < m_iTileNumX*m_iTileNumY; ++i)
	{
		Tile* pTile = Obj::CreateObj<Tile>("Tile");

		pTile->m_pScene = m_pScene;
		pTile->m_pLayer = m_pLayer;

		pTile->Load(pFile);

		pTile->SetAlpha(255);
		pTile->EnableAlpha(true);

		m_vecTile.push_back(pTile);
	}

	GET_SINGLE(Camera)->SetWorldResolution(m_iTileNumX * m_iTileSizeX, m_iTileNumY * m_iTileSizeY);
}

void Stage::ChangeTileImageOffset(const POSITION& tPos,
	const POSITION& tOffSet, int iLayer)
{
	int iIndex = GetTileIndex(tPos);

	if (iIndex == -1)
		return;

	if(iLayer == 0)
		m_vecTile[iIndex]->SetImageOffset(tOffSet);

	else if(iLayer == 1)
		m_vecTile[iIndex]->SetUpperImageOffset(tOffSet);
}

void Stage::ChangeTileTexture(const POSITION& tPos, Texture* pTexture, int iLayer)
{
	size_t iSize = m_vecTile.size();

	int iIndex = GetTileIndex(tPos);

	if (iIndex <= -1 || iIndex >= (int)iSize)
		return;

	if(iLayer == 0)
		m_vecTile[iIndex]->SetTexture(pTexture);

	else if(iLayer == 1)
		m_vecTile[iIndex]->SetUpperTexture(pTexture);
}

void Stage::ChangeTileOption(const POSITION & tPos,
	TILE_OPTION eOption)
{
	int iIndex = GetTileIndex(tPos);

	size_t iSize = m_vecTile.size();

	if (iIndex == -1 || iSize <= iIndex)
		return;

	m_vecTile[iIndex]->SetTileOption(eOption);
}

int Stage::GetTileIndex(const POSITION & tPos)	const
{
	POSITION tNewPos = tPos - m_tStart;

	return GetTileIndex(tNewPos.x, tNewPos.y);
}

int Stage::GetTileIndex(float x, float y)	const
{
	int idxX = (int)x/ m_iTileSizeX;
	int idxY = (int)y / m_iTileSizeY;

	/*if (idxX < 0 || idxX >= m_iTileNumX)
		return -1;

	else if (idxY < 0 || idxY >= m_iTileNumY)
		return -1;*/

	return idxY * m_iTileNumX +idxX;
}

Tile* Stage::GetTile(const POSITION& tPos) const
{
	int iIndex = GetTileIndex(tPos);

	size_t iSize = m_vecTile.size();

	if (iIndex < 0 || iIndex >= iSize)
		return nullptr;

	return m_vecTile[iIndex];
}

void Stage::SetBlock(OBJ_BLOCK eBlock, Obj* pObj)
{
	POSITION tPos = m_pSelectTile->GetPos() - m_tStart;

	int iIndexX = (int)(tPos.x / m_iTileSizeX);
	int iIndexY = (int)(tPos.y / m_iTileSizeY);

	tPos.x += m_iTileSizeX / 2.f;

	m_pSelectTile->SetObj(pObj);

	if(pObj)
		pObj->SetPos(tPos);

	m_ppBlock[iIndexY][iIndexX] = eBlock;
}

void Stage::SetBlock(int iTileIndex, OBJ_BLOCK eBlock, class Obj* pObj)
{
	m_vecTile[iTileIndex]->SetObj(pObj);

	pObj->SetPos(iTileIndex % m_iTileNumX * m_iTileSizeX + m_iTileSizeX / 2.f,
		iTileIndex / m_iTileNumX * m_iTileSizeY + m_iTileSizeY / 2.f);

	m_ppBlock[iTileIndex / m_iTileNumX][iTileIndex % m_iTileNumX] = eBlock;
}

void Stage::SetBlock(const POSITION& tPos, OBJ_BLOCK eBlock, Obj* pObj)
{
	int iIndex = GetTileIndex(tPos);

	SetBlock(iIndex, eBlock, pObj);
}

void Stage::SetBlock(Tile* pTile, OBJ_BLOCK eBlock, Obj* pObj)
{
	size_t iSize = m_vecTile.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		if (m_vecTile[i] == pTile)
			SetBlock((int)i, eBlock, pObj);
	}
}

OBJ_BLOCK Stage::GetBlock(int iTileIndex)	const
{
	return OBJ_BLOCK(m_ppBlock[iTileIndex / m_iTileNumX][iTileIndex % m_iTileNumX]);
}

OBJ_BLOCK Stage::GetBlock() const
{
	if (!m_pSelectTile)
		return OB_NONE;

	POSITION tPos = m_pSelectTile->GetPos() - m_tStart;

	int iIndexX = (int)(tPos.x / m_iTileSizeX);
	int iIndexY = (int)(tPos.y / m_iTileSizeY);

	return (OBJ_BLOCK)m_ppBlock[iIndexY][iIndexX];
}

void Stage::ClearTile()
{
	for (int i = 0; i < m_iTileNumY; ++i)
		SAFE_DELETE_ARRAY(m_ppBlock[i]);
	SAFE_DELETE_ARRAY(m_ppBlock);

	for (int i = 0; i < m_vecTile.size(); ++i)
	{
		Obj::EraseObj(m_vecTile[i]);
	}

	Safe_Release_VecList(m_vecTile);
}