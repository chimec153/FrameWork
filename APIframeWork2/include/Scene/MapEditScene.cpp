#include "MapEditScene.h"
#include "../Object/Stage.h"
#include "Layer.h"
#include "../Core/Camera.h"
#include "Layer.h"
#include "../Core/Camera.h"
#include "../Collider/ColliderRect.h"
#include "../Core.h"
#include "SceneManager.h"
#include "StartScene.h"
#include "../Core/Input.h"
#include "../Resources/ResourcesManager.h"
#include "../Resources/Texture.h"
#include "../resource.h"
#include "../Core//PathManager.h"
#include "../Sound/SoundManager.h"
#include "..//Object/UIPanel.h"
#include "..//Object/UIButton.h"
#include "..//Object/Tree.h"
#include "..//Object/Tile.h"
#include "..//Object/Building.h"
#include "..//Object/UITilePanel.h"
#include "../MapEditDlg.h"
#include "../Object/Crop.h"
#include "../Object/Rock.h"

TCHAR MapEditScene::m_strText[MAX_PATH] = {};

static wchar_t* charToWChar(const char* text);

MapEditScene::MapEditScene()	:
	m_pTileUI(nullptr),
	m_iEditTileTex(0),
	m_eEditOption((TILE_OPTION)0),
	m_pPanel(nullptr),
	m_iPage(0),
	m_iThema(0),
	m_iEditLayer(0)
{
	m_eTem = TEM_TEXTURE;
	m_pMapEditDlg = new CMapEditDlg;
}

MapEditScene::~MapEditScene()
{
	Safe_Release_VecList(m_vecTileTex);
	SAFE_RELEASE(m_pStage);
	SAFE_RELEASE(m_pTileUI);
	SAFE_RELEASE(m_pPanel);
	SAFE_DELETE(m_pMapEditDlg);

}

bool MapEditScene::Init()
{
	GET_SINGLE(Camera)->SetWorldResolution(6400,6400);

	SetEditMode(true);

	Texture* pTexture = GET_SINGLE(ResourcesManager)->LoadTexture("Tile2", TEXT("Maps\\walls_and_floors.bmp"));
	SAFE_RELEASE(pTexture);

	pTexture = GET_SINGLE(ResourcesManager)->LoadTexture("springoutdoor", TEXT("Maps\\spring_outdoorsTileSheet.bmp"));
	SAFE_RELEASE(pTexture);

	pTexture = GET_SINGLE(ResourcesManager)->LoadTexture("mine", TEXT("Maps\\TheMines.bmp"));
	SAFE_RELEASE(pTexture);

	Texture*	pNoneTex = GET_SINGLE(ResourcesManager)->LoadTexture(
		"TileNone", L"TileSheets\\Move.bmp");
	pNoneTex->SetColorKey(0, 32, 0);

	SAFE_RELEASE(pNoneTex);

	Texture*	pNoMoveTex = GET_SINGLE(ResourcesManager)->LoadTexture(
		"TileNoMove", L"TileSheets\\Nomove.bmp");
	pNoMoveTex->SetColorKey(0, 32, 0);

	SAFE_RELEASE(pNoMoveTex);

	Layer* pStageLayer = FindLayer("Stage");

	m_pStage = Obj::CreateObj<Stage>("Stage", pStageLayer);

	m_pStage->CreateTile(100, 100, 32, 32, "Tiles", L"TileSheets\\Flooring.bmp");
	m_pStage->SetTexture("Back", TEXT("AnimationUI\\BackGround.bmp"));

	Layer* pLayer = FindLayer("UI");

	m_pTileUI = Obj::CreateObj<UIPanel>("TilePanel",
		pLayer);

	m_pTileUI->SetSize(32.f, 32.f);
	m_pTileUI->SetTexture("Tiles", L"TileSheets\\Flooring.bmp");
	m_pTileUI->SetPos(GETRESOLUTION.iW * 2.f / 3.f, GETRESOLUTION.iH / 3.f - 8.f);

	LoadFile();

	Texture*	pTex = GET_SINGLE(ResourcesManager)->FindTexture("Tiles");
	m_vecTileTex.push_back(pTex);

	m_vecTileTex.push_back(pTex);
	m_iEditTileTex = 0;
	m_eEditOption = TO_NONE;

	//GET_SINGLE(SoundManager)->LoadSound("system1", false, "PPong.mp3");
	return true;
}

void MapEditScene::Input(float fDeltaTime)
{
	Scene::Input(fDeltaTime);

	if (GetAsyncKeyState('W') & 0x8000)
	{
		GET_SINGLE(Camera)->Scroll(0.f, -300.f * fDeltaTime);
	}
	if (GetAsyncKeyState('S') & 0x8000)
	{
		GET_SINGLE(Camera)->Scroll(0.f, 300.f * fDeltaTime);
	}
	if (GetAsyncKeyState('A') & 0x8000)
	{
		GET_SINGLE(Camera)->Scroll(-300.f * fDeltaTime,0.f);
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		GET_SINGLE(Camera)->Scroll(300.f * fDeltaTime, 0.f);
	}

	if (GetAsyncKeyState(VK_F1) & 0x8000)
	{
		m_eTem = TEM_TEXTURE;
		m_pTileUI->SetEnable(false);
		SAFE_RELEASE(m_pTileUI);
		Layer* pLayer = FindLayer("UI");
		m_pTileUI = Obj::CreateObj<UIPanel>("TilePanel",
			pLayer);

		m_pTileUI->SetSize(32.f, 32.f);
		m_pTileUI->SetTexture("Tiles", L"TileSheets\\Flooring.bmp");
		m_pTileUI->SetPos(GETRESOLUTION.iW * 2.f / 3.f, GETRESOLUTION.iH / 3.f - 8.f);
	}

	if (GetAsyncKeyState(VK_F2) & 0x8000)
		m_eTem = TEM_OPTION;

	if (GetAsyncKeyState(VK_F3) & 0x8000)
	{
		m_eTem = TEM_OBJECT;

		size_t iSize = m_vecstrProto.size();

		if (iSize <= m_iEditTileTex)
			m_iEditTileTex = (int)iSize - 1;

		if (m_pTileUI)
		m_pTileUI->Die();

		SAFE_RELEASE(m_pTileUI);

		Layer* pLayer = FindLayer("UI");

		m_pTileUI = m_pStage->CreateCloneObj(m_vecstrProto[m_iEditTileTex], "TreeUI", pLayer);

		if(m_pTileUI)
			m_pTileUI->SetPos(GETRESOLUTION.iW * 2.f / 3.f, GETRESOLUTION.iH / 3.f - 8.f);
	}
	if (KEYDOWN("PrevScene"))
	{
		m_pMapEditDlg->Clear();

		GET_SINGLE(SceneManager)->CreateScene<StartScene>("Start", SC_NEXT);

		GET_SINGLE(Camera)->SetPos(0.f, 0.f);
	}

	if (KEYDOWN("TileSub"))	//	1번 키
	{
		Layer* pLayer = FindLayer("UI");

		switch (m_eTem)
		{
		case TEM_OBJECT:
		case TEM_TEXTURE:
			--m_iEditTileTex;
			if (m_iEditTileTex < 0)
				m_iEditTileTex = 0;
			//GET_SINGLE(SoundManager)->Play("system1");

			if (m_eTem == TEM_OBJECT)
			{
				if (m_pTileUI)
					m_pTileUI->Die();

				SAFE_RELEASE(m_pTileUI);

				m_pTileUI = m_pStage->CreateCloneObj(m_vecstrProto[m_iEditTileTex], "TreeUI", pLayer);
			}
			break;
		case TEM_OPTION:
			m_eEditOption = TO_NONE;
			//GET_SINGLE(SoundManager)->Play("system1");
			break;
		}
	}
	size_t iSize=0;
	if (KEYDOWN("TileAdd"))	//	2번 키
	{
		Layer* pLayer = FindLayer("UI");

		switch (m_eTem)
		{
		case TEM_OBJECT:
			++m_iEditTileTex;

			iSize = m_vecstrProto.size();

			if (iSize == m_iEditTileTex)
				m_iEditTileTex = (int)iSize - 1;

			if (m_pTileUI)
				m_pTileUI->Die();

			SAFE_RELEASE(m_pTileUI);

			m_pTileUI = m_pStage->CreateCloneObj(m_vecstrProto[m_iEditTileTex], "TreeUI", pLayer);

			break;
		case TEM_TEXTURE:
			++m_iEditTileTex;
			break;
		case TEM_OPTION:
			m_eEditOption = TO_NOMOVE;
			break;
		}
	}

	if (KEYDOWN("TileSetChange"))	//	3번 키
		switch (m_eTem)
		{
		case TEM_TEXTURE:
			++m_iPage;
			break;
		case TEM_OPTION:
			m_eEditOption = TO_NOMOVE;
			break;
		}

	POSITION tOffset = {};

	switch (m_eTem)
	{
	case TEM_TEXTURE:
		SetTileTexture();
		break;
	case TEM_OPTION:
		break;
	case TEM_OBJECT:
		iSize = m_vecstrProto.size();

		if (iSize > m_iEditTileTex)
		{
			if(m_pTileUI)
			m_pTileUI->SetTexture(m_vecstrProto[m_iEditTileTex]);
		}
			
		break;
	default:
		break;
	}

	HWND hActiveWnd = GetActiveWindow();
	HWND hWnd = WINDOWHANDLE;

	if (hActiveWnd == hWnd)
	{
		BRUSH_MODE eMode = (BRUSH_MODE)m_pMapEditDlg->GetBrushMode();

		if (KEYDOWN("CtrlLButton"))
		{

		}

		else if (KEYDOWN("MouseLButton"))
		{
			if (eMode == BM_RECT)
			{
				POSITION	tMouseWorldPos = MOUSEWORLDPOS;

				int iBrush = m_pMapEditDlg->GetBrush();

				if (iBrush == TEM_TEXTURE)
				{
					m_tStartPos = tMouseWorldPos;
				}
			}
		}

		if (KEYPRESS("CtrlLButton"))
		{

		}

		else if (KEYPRESS("MouseLButton"))
		{
			POSITION	tMouseWorldPos = MOUSEWORLDPOS;
			POSITION tOffset = {};
			Texture* pTex = nullptr;

			Layer* pLayer = FindLayer("Default");

			int iBrush = m_pMapEditDlg->GetBrush();
			int iType = m_pMapEditDlg->GetType();

			if (iBrush == TEM_TEXTURE)
			{
				if (eMode == BM_PEN)
				{
					m_pStage->ChangeTileTexture(tMouseWorldPos, m_pMapEditDlg->GetTexture(), m_iEditLayer);

					Tile* pTile = m_pStage->GetTile(tMouseWorldPos);

					if (pTile)
					{
						POSITION tPos = m_pMapEditDlg->GetStartPos();

						POSITION tTileStart = m_pMapEditDlg->GetTileStart();
						POSITION tTileEnd = m_pMapEditDlg->GetTileEnd();

						if (tTileStart == tTileEnd)
						{
							if (m_iEditLayer == 0)
								pTile->SetImageOffset(tPos);

							else
								pTile->SetUpperImageOffset(tPos);
						}

						else
						{
							float fTileSizeX = pTile->GetSize().x;
							float fTileSizeY = pTile->GetSize().y;

							for (float fY = tTileStart.y; fY <= tTileEnd.y; fY += fTileSizeY)
							{
								for (float fX = tTileStart.x; fX <= tTileEnd.x; fX += fTileSizeX)
								{
									POSITION tOffSet = {};

									tOffSet.x = fX - tTileStart.x;
									tOffSet.y = fY - tTileStart.y;

									m_pStage->ChangeTileTexture(tMouseWorldPos + tOffSet, m_pMapEditDlg->GetTexture(), m_iEditLayer);

									Tile* pTile = m_pStage->GetTile(tMouseWorldPos + tOffSet);

									if (pTile)
									{
										if (m_iEditLayer == 0)
											pTile->SetImageOffset(fX, fY);

										else
											pTile->SetUpperImageOffset(fX, fY);
									}
								}
							}
						}
					}
				}
			}

			else if (iBrush == TEM_OPTION)
			{
				m_pStage->ChangeTileOption(tMouseWorldPos, (TILE_OPTION)iType);
			}

			else if (iBrush == TEM_OBJECT)
			{
				Tile* pTile = m_pStage->GetTile(tMouseWorldPos);

				if (pTile)
				{
					Obj* pObj = pTile->GetObj();

					if (!pObj)	//	블록을 검사하고 블록에 아무 오브젝트도 없을 경우이다.
					{
						size_t iSize = m_vecstrProto.size();

						if (m_iEditTileTex < iSize)
						{
							Obj* pObj = m_pStage->CreateCloneObj(m_vecstrProto[m_iEditTileTex], "Tree",	pLayer);

							int iTileSizeX = m_pStage->GetTileSizeX();
							int iTileSizeY = m_pStage->GetTileSizeY();

							POSITION tPos = pTile->GetPos();

							if (pObj)
							{
								OBJ_BLOCK eBlock = pObj->GetBlock();

								m_pStage->SetBlock(pTile, pObj->GetBlock(), pObj);

								if(eBlock != OB_GRASS)
									pTile->SetTileOption(TO_NOMOVE);
								
								SAFE_RELEASE(pObj);
							}
						}
					}
				}
			}
		}

		if (KEYUP("CtrlLButton"))
		{

		}

		else if (KEYUP("MouseLButton"))
		{
			if (eMode == BM_RECT)
			{
				POSITION	tMouseWorldPos = MOUSEWORLDPOS;

				int iBrush = m_pMapEditDlg->GetBrush();

				if (iBrush == TEM_TEXTURE)
				{
						m_pStage->ChangeTileTexture(tMouseWorldPos, m_pMapEditDlg->GetTexture(), m_iEditLayer);

						POSITION tPos = m_pMapEditDlg->GetStartPos();

						int iTileSizeX = m_pStage->GetTileSizeX();
						int iTileSizeY = m_pStage->GetTileSizeY();

						int iTileNumX = m_pStage->GetTileNumX();
						int iTileNumY = m_pStage->GetTileNumY();

						int iStartIndexX = (int)(m_tStartPos.x / iTileSizeX);
						int iStartIndexY = (int)(m_tStartPos.y / iTileSizeY);

						int iEndIndexX = (int)(tMouseWorldPos.x / iTileSizeX);
						int iEndIndexY = (int)(tMouseWorldPos.y / iTileSizeY);

						if (iStartIndexX > iEndIndexX)
						{
							int iTemp = iStartIndexX;
							iStartIndexX = iEndIndexX;
							iEndIndexX = iTemp;
						}

						if (iStartIndexY > iEndIndexY)
						{
							int iTemp = iStartIndexY;
							iStartIndexY = iEndIndexY;
							iEndIndexY = iTemp;
						}

						for (int i = iStartIndexY; i <= iEndIndexY; ++i)
						{
							for (int j = iStartIndexX; j <= iEndIndexX; ++j)
							{
								if (j >= iTileNumX || j < 0 ||
									i >= iTileNumY || i < 0)
									continue;

								POSITION tOffSet = {};

								tOffSet.x = (float)(j * iTileSizeX);
								tOffSet.y = (float)(i * iTileSizeY);

								m_pStage->ChangeTileTexture(tOffSet, m_pMapEditDlg->GetTexture(), m_iEditLayer);

								Tile* pTile = m_pStage->GetTile(iTileNumX * i + j);

								if (pTile)
								{
									if (m_iEditLayer == 0)
										pTile->SetImageOffset(tPos);

									else
										pTile->SetUpperImageOffset(tPos);
								}
							}
						}
				}
			}
		}
	}

	else
	{
		if (KEYPRESS("MouseLButton"))
		{
			if (m_pMapEditDlg->IsPictureClicked())
			{
				m_pMapEditDlg->SetTileEnd();
			}
		}
	}

	if (KEYUP("MouseLButton"))
	{
		if (m_pMapEditDlg->IsPictureClicked())
		{
			m_pMapEditDlg->SetTileEnd();
			m_pMapEditDlg->SetPictureClick(false);
		}
	}

	if (KEYPRESS("MouseRButton"))
	{
		POSITION	tMouseWorldPos = MOUSEWORLDPOS;
		POSITION tOffset = {};

		switch (m_eTem)
		{
		case TEM_TEXTURE:
			break;
		case TEM_OPTION:
			break;
		case TEM_OBJECT:
			int iIndex = m_pStage->GetTileIndex(tMouseWorldPos);

			if (iIndex != -1)
			{
				if (m_pStage->GetTile(iIndex)->GetObj())	//	블록을 검사하고 블록에 오브젝트가 있을 경우이다.
					m_pStage->GetTile(iIndex)->DeleteObj();
			}
			
			break;
		}
	}

	if (KEYDOWN("Save"))
	{
		ShowCursor(TRUE);
		DialogBox(WINDOWINSTANCE, MAKEINTRESOURCE(IDD_DIALOG1),
			WINDOWHANDLE, MapEditScene::DlgProc);

		ShowCursor(FALSE);

		char	strFileName[MAX_PATH] = {};
		WideCharToMultiByte(CP_ACP, 0, m_strText, -1, strFileName,
			lstrlen(m_strText), 0, 0);

		m_pStage->SaveFromPath(strFileName);

		strcat_s(strFileName, "Objects");
		
		const char* pPath = GET_SINGLE(PathManager)->FindPathMultiByte(DATA_PATH);
		
		string	strFullPath;
		if (pPath)
			strFullPath = pPath;
		
		strFullPath += strFileName;
		const char* pFullPath = strFullPath.c_str();
		
		FILE* pFile = nullptr;
		
		fopen_s(&pFile, pFullPath, "wb");
		
		if (pFile)
		{
			Layer* pLayer = FindLayer("Default");
		
			pLayer->Save(pFile);
		
			fclose(pFile);
		}

	}
	if (KEYDOWN("Load"))
	{
		ShowCursor(TRUE);
		DialogBox(WINDOWINSTANCE, MAKEINTRESOURCE(IDD_DIALOG1),
			WINDOWHANDLE, MapEditScene::DlgProc);

		ShowCursor(FALSE);

		char	strFileName[MAX_PATH] = {};
		WideCharToMultiByte(CP_ACP, 0, m_strText, -1, strFileName,
			lstrlen(m_strText), 0, 0);
		if (!m_pStage)
		{
			Layer* pStageLayer = FindLayer("Stage");
			m_pStage = Obj::CreateObj<Stage>("Stage", pStageLayer);
		}

		m_pStage->LoadFromPath(strFileName);

		strcat_s(strFileName, "Objects");
		
		const char* pPath = GET_SINGLE(PathManager)->FindPathMultiByte(DATA_PATH);
		
		string	strFullPath;
		if (pPath)
			strFullPath = pPath;
		
		strFullPath += strFileName;
		const char* pFullPath = strFullPath.c_str();
		
		FILE* pFile = nullptr;
		
		fopen_s(&pFile, pFullPath, "rb");
		
		if (pFile)
		{
			Layer* pLayer = FindLayer("Default");
		
			pLayer->Load(pFile);
		
			fclose(pFile);
		}
	}

	if (KEYDOWN("Fire"))
	{
		m_iEditLayer = 1;
	}

	if (KEYUP("Fire"))
	{
		m_iEditLayer = 0;
	}

	if (KEYDOWN("MapEditDlg"))
	{
		MapEditDlg();
	}
}

int MapEditScene::Update(float fDeltaTime)
{
	Scene::Update(fDeltaTime);

	if (m_eTem == TEM_OBJECT)
	{
		POSITION tCamPos = GET_SINGLE(Camera)->GetPos();

		POSITION tPos = GET_SINGLE(Input)->GetMouseClientPos();

		Layer* pLayer = FindLayer("UI");

		if (m_pTileUI)
			m_pTileUI->Die();

		SAFE_RELEASE(m_pTileUI);

		m_pTileUI = m_pStage->CreateCloneObj(m_vecstrProto[m_iEditTileTex], "TreeUI", pLayer);

		if (m_pTileUI)
		{
			m_pTileUI->SetPos(tPos.x + tCamPos.x, tPos.y + tCamPos.y);
			m_pTileUI->SetAlpha(125);
			m_pTileUI->EnableAlpha(true);
		}
	}

	return 0;
}

void MapEditScene::Render(HDC hDC, float fDeltaTime)
{
	Scene::Render(hDC, fDeltaTime);

#ifdef _DEBUG
	if (KEYPRESS("Debug"))
	{
		TCHAR strDebug[32] = {};
		wsprintf(strDebug, TEXT("m_iEditTileTex: %d"), m_iEditTileTex);
		TextOut(hDC, 0, 0, strDebug, lstrlen(strDebug));
		wsprintf(strDebug, TEXT("m_eEditOption: %d"), m_eEditOption);
		TextOut(hDC, 0, 20, strDebug, lstrlen(strDebug));
		wsprintf(strDebug, TEXT("m_eTem: %d"), m_eTem);
		TextOut(hDC, 0, 40, strDebug, lstrlen(strDebug));
		wsprintf(strDebug, TEXT("Page: %d"), m_iPage);
		TextOut(hDC, 0, 60, strDebug, lstrlen(strDebug));
		wsprintf(strDebug, TEXT("Thema: %d"), m_iThema);
		TextOut(hDC, 0, 80, strDebug, lstrlen(strDebug));
	}

	TCHAR strDebug[32] = {};
	POSITION tPos = GET_SINGLE(Input)->GetMouseWorldPos();
	wsprintf(strDebug, TEXT("Mouse: %d, %d"), (int)tPos.x, (int)tPos.y);
	TextOut(hDC, 0, 100, strDebug, lstrlen(strDebug));

#endif
}

INT_PTR MapEditScene::DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			memset(m_strText, 0, sizeof(wchar_t)*MAX_PATH);
			GetDlgItemText(hWnd, IDC_EDIT1, m_strText, MAX_PATH);
		case IDCANCEL:
			EndDialog(hWnd, IDOK);
			return TRUE;
		}
		return FALSE;
	}
	return INT_PTR();
}

POSITION MapEditScene::SetTileTexture()
{
	POSITION tOffset = {};

	if (m_iThema == 0)
	{
		tOffset.x = 32.f * (m_iEditTileTex % 4) + 128.f * (m_iEditTileTex / 16) - 512.f * (m_iEditTileTex / 64);
		tOffset.y = 32.f * (m_iEditTileTex / 4) - 128.f * (m_iEditTileTex / 16) + 128.f * (m_iEditTileTex / 64);
		m_pTileUI->SetColorKey(255, 255, 255);

	}
	else if (m_iThema == 1)
	{
		m_pTileUI->SetTexture("Tile2", TEXT("Maps\\walls_and_floors.bmp"));
		tOffset.x = 32.f * ((m_iEditTileTex) % 16);
		tOffset.y = 32.f * ((m_iEditTileTex) / 16);
		m_pTileUI->SetColorKey(255, 0, 255);
	}
	else if (m_iThema == 2)
	{
		m_pTileUI->SetTexture("springoutdoor");
		tOffset.x = 32.f * ((m_iEditTileTex) % 21);
		tOffset.y = 32.f * ((m_iEditTileTex) / 21);
		m_pTileUI->SetColorKey(255, 0, 255);
	}
	else if (m_iThema == 3)
	{
		m_pTileUI->SetTexture("mine");
		tOffset.x = 32.f * ((m_iEditTileTex) % 48);
		tOffset.y = 32.f * ((m_iEditTileTex) / 48);
		m_pTileUI->SetColorKey(255, 0, 255);
	}

	m_pTileUI->SetImageOffset(tOffset);
	m_pTileUI->SetSize(32.f, 32.f);

	return tOffset;
}

void MapEditScene::TileButton0(int iButton, float fTime)
{
	m_iEditTileTex = iButton + m_iPage * 64;	//	페이지당 64개의 버튼이 있으므로 64 * 페이지 수 만큼 더해준다.
}

void MapEditScene::TilePrevButton(float fTime)
{
	if (m_iPage == 0)
		return;

	--m_iPage;

	POSITION tOffset = {};
	for (int i = 0; i < 64; ++i)
	{
		UIButton* pBtn = (UIButton*)((UITilePanel*)m_pPanel)->GetButton(i);

		if (m_iThema == 0)
		{
			tOffset.x = 32.f * ((i + m_iPage * 64) % 4) + 128.f * ((i + m_iPage * 64) / 16) - 512.f * ((i + m_iPage * 64) / 64);
			tOffset.y = 32.f * ((i + m_iPage * 64) / 4) - 128.f * ((i + m_iPage * 64) / 16) + 128.f * ((i + m_iPage * 64) / 64);
		}
		else if (m_iThema == 1)
		{
			tOffset.x = 32.f * ((i + m_iPage * 64) % 16);
			tOffset.y = 32.f * ((i + m_iPage * 64) / 16);
		}
		else if (m_iThema == 2)
		{
			tOffset.x = 32.f * ((i + m_iPage * 64) % 21);
			tOffset.y = 32.f * ((i + m_iPage * 64) / 21);
		}
		else if (m_iThema == 3)
		{
			tOffset.x = 32.f * ((i + m_iPage * 64) % 48);
			tOffset.y = 32.f * ((i + m_iPage * 64) / 48);
		}

		pBtn->SetImageOffset(tOffset);
	}
}

void MapEditScene::TileNextButton(float fTime)
{
	++m_iPage;

	POSITION tOffset = {};

	for (int i = 0; i < 64; ++i)
	{
		UIButton* pBtn = (UIButton*)((UITilePanel*)m_pPanel)->GetButton(i);

		if (m_iThema == 0)
		{
			tOffset.x = 32.f * ((i + m_iPage * 64) % 4) + 128.f * ((i + m_iPage * 64) / 16) - 512.f * ((i + m_iPage * 64) / 64);
			tOffset.y = 32.f * ((i + m_iPage * 64) / 4) - 128.f * ((i + m_iPage * 64) / 16) + 128.f * ((i + m_iPage * 64) / 64);
		}
		else if (m_iThema == 1)
		{
			tOffset.x = 32.f * ((i + m_iPage * 64) % 16);
			tOffset.y = 32.f * ((i + m_iPage * 64) / 16);
		}
		else if (m_iThema == 2)
		{
			tOffset.x = 32.f * ((i + m_iPage * 64) % 21);
			tOffset.y = 32.f * ((i + m_iPage * 64) / 21);
		}
		else if (m_iThema == 3)
		{
			tOffset.x = 32.f * ((i + m_iPage * 64) % 48);
			tOffset.y = 32.f * ((i + m_iPage * 64) / 48);
		}

		pBtn->SetImageOffset(tOffset);
	}
}

void MapEditScene::TileUpButton(float fTime)
{
	++m_iThema;
	m_iPage = 0;

	Texture* pTex = nullptr;
	POSITION tOffset = {};

	if (m_iThema == 0)
	{
		pTex = GET_SINGLE(ResourcesManager)->FindTexture("Tiles");
	}

	else if (m_iThema == 1)
	{
		pTex = GET_SINGLE(ResourcesManager)->FindTexture("Tile2");
	}

	else if (m_iThema == 2)
	{
		pTex = GET_SINGLE(ResourcesManager)->FindTexture("springoutdoor");
	}
	else if (m_iThema == 3)
	{
		pTex = GET_SINGLE(ResourcesManager)->FindTexture("mine");
	}

	for (int i = 0; i < 64; ++i)
	{
		UIButton* pBtn = (UIButton*)((UITilePanel*)m_pPanel)->GetButton(i);

		if (m_iThema == 0)
		{
			tOffset.x = 32.f * (i % 4) + 128.f * (i / 16) - 512.f * (i / 64);
			tOffset.y = 32.f * (i / 4) - 128.f * (i / 16) + 128.f * (i / 64);
		}
		else if (m_iThema == 1)
		{
			tOffset.x = 32.f * (i % 16);
			tOffset.y = 32.f * (i / 16);
		}
		else if (m_iThema == 2)
		{
			tOffset.x = 32.f * (i % 21);
			tOffset.y = 32.f * (i / 21);
		}
		else if (m_iThema == 3)
		{
			tOffset.x = 32.f * (i % 48);
			tOffset.y = 32.f * (i / 48);
		}

		pBtn->SetImageOffset(tOffset);
		pBtn->SetTexture(pTex);
	}
	m_pTileUI->SetTexture(pTex);

	SAFE_RELEASE(pTex);
}

void MapEditScene::TileDownButton(float fTime)
{
	if (m_iThema == 0)
		return;

	--m_iThema;

	m_iPage = 0;

	Texture* pTex = nullptr;
	POSITION tOffset = {};

	if (m_iThema == 0)
		pTex = GET_SINGLE(ResourcesManager)->FindTexture("Tiles");

	else if (m_iThema == 1)
		pTex = GET_SINGLE(ResourcesManager)->FindTexture("Tile2");

	else if (m_iThema == 2)
		pTex = GET_SINGLE(ResourcesManager)->FindTexture("springoutdoor");

	else if (m_iThema == 3)
		pTex = GET_SINGLE(ResourcesManager)->FindTexture("mine");

	for (int i = 0; i < 64; ++i)
	{
		UIButton* pBtn = (UIButton*)((UITilePanel*)m_pPanel)->GetButton(i);

		if (m_iThema == 0)
		{
			tOffset.x = 32.f * (i % 4) + 128.f * (i / 16) - 512.f * (i / 64);
			tOffset.y = 32.f * (i / 4) - 128.f * (i / 16) + 128.f * (i / 64);
		}
		else if (m_iThema == 1)
		{
			tOffset.x = 32.f * (i % 16);
			tOffset.y = 32.f * (i / 16);
		}
		else if (m_iThema == 2)
		{
			tOffset.x = 32.f * (i % 21);
			tOffset.y = 32.f * (i / 21);
		}
		else if (m_iThema == 3)
		{
			tOffset.x = 32.f * (i % 48);
			tOffset.y = 32.f * (i / 48);
		}

		pBtn->SetImageOffset(tOffset);
		pBtn->SetTexture(pTex);
	}
	m_pTileUI->SetTexture(pTex);

	SAFE_RELEASE(pTex);
}

void MapEditScene::MapEditDlg()
{
	m_pMapEditDlg->SetStage(m_pStage);
	m_pMapEditDlg->OnDialog(IDD_DIALOG3);

	ShowCursor(true);
}
