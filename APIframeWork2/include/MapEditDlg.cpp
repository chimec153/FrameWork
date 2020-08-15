#include "MapEditDlg.h"
#include "Core.h"
#include "Object/Stage.h"
#include "Core/PathManager.h"
#include "Scene/SceneManager.h"
#include "Resources/ResourcesManager.h"
#include "Resources/Texture.h"
#include "Core/Input.h"
#include "Scene/MapEditScene.h"

CMapEditDlg*	g_pDlg = nullptr;

CMapEditDlg::CMapEditDlg()	:
	m_iID(0),
	m_hWnd(0),
	m_pStage(nullptr),
	m_hTypeCombo(0),
	m_hBrushCombo(0),
	m_hTextureList(0),
	m_pTexture(nullptr),
	m_iHPos(0),
	m_iVPos(0),
	m_hHScroll(0),
	m_hVScroll(0),
	m_hPicture(0),
	m_hBackDC(0),
	m_hBackBitmap(0),
	m_hPrevBitmap(0),
	m_bPictureClick(false),
	m_hTreeObj(0),
	m_pObjInfo(nullptr),
	m_eBurshMode(BM_PEN)
{
	g_pDlg = this;
	m_pBackBuffer = GET_SINGLE(ResourcesManager)->LoadTexture("BackBuffer", TEXT("Texture\\AnimationUI\\Panel.bmp"));
}

CMapEditDlg::~CMapEditDlg()
{
	SAFE_RELEASE(m_pTexture);
	SAFE_RELEASE(m_pBackBuffer);
	SAFE_DELETE_ARRAY(m_pObjInfo);
}

void CMapEditDlg::SetTexture(Texture* pTexture)
{
	SAFE_RELEASE(m_pTexture);

	m_pTexture = pTexture;

	if (m_pTexture)
		m_pTexture->AddRef();
}

void CMapEditDlg::OnDialog(int iID)
{
	HINSTANCE hWndInst = GET_SINGLE(Core)->GetWindowInstance();

	m_hWnd = CreateDialog(hWndInst, MAKEINTRESOURCE(iID),
		GET_SINGLE(Core)->GetWindowHandle(), CMapEditDlg::WndProc);

	ShowWindow(m_hWnd, SW_SHOW);

	TCHAR strText[][30] = {
		TEXT("보통"),
		TEXT("농사가능"),
		TEXT("경작된 땅"),
		TEXT("물준 땅"),
		TEXT("이동 불가"),
		TEXT("물")
	};

	TCHAR strBrush[][30] = {
		TEXT("텍스쳐"),
		TEXT("타입"),
		TEXT("오브젝트")
	};

	size_t iSize = ((MapEditScene*)GET_SINGLE(SceneManager)->GetScene())->GetProtoSize();

	m_pObjInfo = new ObjInfo[iSize];

	ObjInfo objInfo[] = {
		{-1, 0},
		{0, 1},
		{1, 2},
		{1, 2},
		{1, 2},
		{0, 1},
		{5, 2},
		{5, 2},
		{5, 2},
		{5, 2},
		{5, 2},
		{5, 2},
		{5, 2},
		{0, 1},
		{13, 2},
		{13, 2},
		{13, 2},
		{13, 2},
		{13, 2},
		{13, 2},
		{13, 2},
		{13, 2},
		{13, 2}
	};

	memcpy(m_pObjInfo, objInfo, sizeof(ObjInfo) * (sizeof(objInfo) / sizeof(objInfo[0])));

	m_hTypeCombo = GetDlgItem(m_hWnd, IDC_COMBO_TYPE);
	m_hBrushCombo = GetDlgItem(m_hWnd, IDC_COMBO_BRUSH);

	m_hTextureList = GetDlgItem(m_hWnd, IDC_LIST_TEXTURE);

	m_hTreeObj = GetDlgItem(m_hWnd, IDC_TREE_OBJ);

	// 트리뷰에 사용할 이미지 리스트를 만들어 트리뷰에 연결한다.
	//HIMAGELIST hImageList = ImageList_LoadBitmap(hWndInst, MAKEINTRESOURCE(IDB_BITMAP1), 16, 1, RGB(255, 255, 255));

	//TreeView_SetImageList(m_hTreeObj, hImageList, TVSIL_NORMAL);

	InsertChild((HTREEITEM)0, -1);

	m_hHScroll = GetDlgItem(m_hWnd, IDC_SCROLLBAR_H);
	m_hVScroll = GetDlgItem(m_hWnd, IDC_SCROLLBAR_V);

	m_hPicture = GetDlgItem(m_hWnd, IDC_PICTURE);

	BOOL bOK = SetScrollRange(m_hWnd, SB_HORZ, 0, 100, false);
	BOOL bOK1 = SetScrollRange(m_hWnd, SB_VERT, 0, 100, false);	

	for (int i = 0; i < sizeof(strText) / sizeof(strText[0]); ++i)
		SendMessage(m_hTypeCombo, CB_ADDSTRING, 0, (LPARAM)strText[i]);

	for (int i = 0; i < sizeof(strBrush) / sizeof(strBrush[0]); ++i)
		SendMessage(m_hBrushCombo, CB_ADDSTRING, 0, (LPARAM)strBrush[i]);

	SendMessage(m_hTypeCombo, CB_SETCURSEL, 0, 0);
	SendMessage(m_hBrushCombo, CB_SETCURSEL, 0, 0);

	SetDlgItemInt(m_hWnd, IDC_EDIT_COUNTX, 50, true);
	SetDlgItemInt(m_hWnd, IDC_EDIT_COUNTY, 50, true);
	SetDlgItemInt(m_hWnd, IDC_EDIT_SIZEX, 32, true);
	SetDlgItemInt(m_hWnd, IDC_EDIT_SIZEY, 32, true);

	Texture* pTexture = GET_SINGLE(ResourcesManager)->LoadTexture("Tile2", TEXT("Maps\\walls_and_floors.bmp"));
	SAFE_RELEASE(pTexture);

	SendMessage(m_hTextureList, LB_ADDSTRING, 0, (LPARAM)TEXT("Tile2"));

	pTexture = GET_SINGLE(ResourcesManager)->LoadTexture("springoutdoor", TEXT("Maps\\spring_outdoorsTileSheet.bmp"));
	SAFE_RELEASE(pTexture);

	SendMessage(m_hTextureList, LB_ADDSTRING, 0, (LPARAM)TEXT("springoutdoor"));

	pTexture = GET_SINGLE(ResourcesManager)->LoadTexture("mine", TEXT("Maps\\TheMines.bmp"));
	SAFE_RELEASE(pTexture);

	SendMessage(m_hTextureList, LB_ADDSTRING, 0, (LPARAM)TEXT("mine"));
}

void CMapEditDlg::CreateTileMap()
{
	BOOL bTran = false;

	int iCountX = GetDlgItemInt(m_hWnd, IDC_EDIT_COUNTX, &bTran, true);
	int iCountY = GetDlgItemInt(m_hWnd, IDC_EDIT_COUNTY, &bTran, true);
	int iSizeX = GetDlgItemInt(m_hWnd, IDC_EDIT_SIZEX, &bTran, true);
	int iSizeY = GetDlgItemInt(m_hWnd, IDC_EDIT_SIZEY, &bTran, true);

	m_pStage->CreateTile(iCountX, iCountY, iSizeX, iSizeY, "springoutdoor", TEXT("Maps\\spring_outdoorsTileSheet.bmp"));
}

void CMapEditDlg::SetStage(Stage* pStage)
{
	SAFE_RELEASE(m_pStage);

	m_pStage = pStage;
}

void CMapEditDlg::Save()
{
	OPENFILENAME tOFN = {};

	TCHAR strFileName[256] = {};

	tOFN.lStructSize = sizeof(OPENFILENAME);
	tOFN.nMaxFile = 256;
	tOFN.hwndOwner = m_hWnd;
	tOFN.lpstrFilter = (TCHAR*)TEXT("모든파일\0*.*\0맵파일\0*.tmp");
	tOFN.lpstrInitialDir = GET_SINGLE(PathManager)->FindPath(DATA_PATH);
	tOFN.lpstrFile = strFileName;

	if (GetSaveFileName(&tOFN))
	{
		Save(strFileName);
	}
}

void CMapEditDlg::Load()
{
	OPENFILENAME tOFN = {};

	TCHAR strFileName[256] = {};

	tOFN.lStructSize = sizeof(OPENFILENAME);
	tOFN.nMaxFile = 256;
	tOFN.hwndOwner = m_hWnd;
	tOFN.lpstrFilter = TEXT("모든파일\0*.*\0맵파일\0*.tmp");
	tOFN.lpstrInitialDir = GET_SINGLE(PathManager)->FindPath(DATA_PATH);
	tOFN.lpstrFile = strFileName;

	if (GetOpenFileName(&tOFN))
	{		
		Load(strFileName);
	}
}

void CMapEditDlg::Clear()
{
	DestroyWindow(m_hWnd);
	g_pDlg = 0;
	ShowCursor(false);
}

void CMapEditDlg::LoadTexture()
{
	OPENFILENAME tOFN = {};

	TCHAR strFileName[256] = {};

	tOFN.lStructSize = sizeof(OPENFILENAME);
	tOFN.nMaxFile = 256;
	tOFN.hwndOwner = m_hWnd;
	tOFN.lpstrFilter = TEXT("모든파일\0*.*\0비트맵파일\0*.bmp");
	tOFN.lpstrInitialDir = GET_SINGLE(PathManager)->FindPath(TEXTURE_PATH);
	tOFN.lpstrFile = strFileName;

	if (GetOpenFileName(&tOFN))
	{
		LoadTexture(strFileName);
	}
}

void CMapEditDlg::SelectTexture()
{
	int i = (int)SendMessage(m_hTextureList, LB_GETCURSEL, 0, 0);

	TCHAR strName[MAX_PATH] = {};

	SendMessage(m_hTextureList, LB_GETTEXT, i, (LPARAM)strName);

	char strFileName[MAX_PATH] = {};

#ifdef UNICODE
	WideCharToMultiByte(CP_ACP, NULL, strName, -1, strFileName, lstrlen(strName), NULL, NULL);
#else
	strcpy_s(strFileName, strName);
#endif

	SAFE_RELEASE(m_pTexture);

	m_pTexture = GET_SINGLE(ResourcesManager)->FindTexture(strFileName);
}

void CMapEditDlg::Render(HDC hDC, float fTime)
{
	RECT tRC = {};
	POINT pt = {};

	if (m_hWnd)
	{
		HWND hPicture = GetDlgItem(m_hWnd, IDC_PICTURE);
		GetWindowRect(hPicture, &tRC);

		ScreenToClient(m_hWnd, &pt);

		BitBlt(hDC, (int)(tRC.left + pt.x), (int)(tRC.top + pt.y),
			(int)(tRC.right - tRC.left), (int)(tRC.bottom - tRC.top), m_hBackDC, 0, 0, SRCCOPY);

		if (m_pTexture)
		{
			if (m_pTexture->GetColorKeyEnable())
			{
				int fWidth = m_pTexture->GetWidth();
				int fHeight = m_pTexture->GetHeight();

				int fRenderWidth = (tRC.right - tRC.left);
				int fRenderHeight = (tRC.bottom - tRC.top);

				if (fWidth < m_tImageOffSet.x + tRC.right - tRC.left)
					fRenderWidth -= (int)(m_tImageOffSet.x + tRC.right - tRC.left) - fWidth;

				if (fHeight < m_tImageOffSet.y + tRC.bottom - tRC.top)
					fRenderHeight -= (int)(m_tImageOffSet.y + tRC.bottom - tRC.top) - fHeight;

				TransparentBlt(hDC, (int)(tRC.left + pt.x), (int)(tRC.top + pt.y), (int)(fRenderWidth), (int)(fRenderHeight), m_pTexture->GetDC(),
					(int)m_tImageOffSet.x, (int)m_tImageOffSet.y, (int)(fRenderWidth), (int)(fRenderHeight), m_pTexture->GetColorKey());
			}
			else
			{
				/*BLENDFUNCTION	tBF = {};

				tBF.BlendOp = 0;
				tBF.BlendFlags = 0;
				tBF.SourceConstantAlpha = 255;

				tBF.AlphaFormat = AC_SRC_ALPHA;

				GdiAlphaBlend(hDC, (int)(tRC.left + pt.x), (int)(tRC.top + pt.y), (int)tRC.right - tRC.left, (int)tRC.bottom - tRC.top,
					m_pTexture->GetDC(), (int)m_tImageOffSet.x, (int)m_tImageOffSet.y, (int)tRC.right - tRC.left, (int)tRC.bottom - tRC.top, tBF);*/


				BitBlt(hDC, (int)(tRC.left + pt.x), (int)(tRC.top + pt.y), (int)tRC.right - tRC.left, (int)tRC.bottom - tRC.top, m_pTexture->GetDC(),
					(int)m_tImageOffSet.x, (int)m_tImageOffSet.y, SRCCOPY);
			}
		}

		RECT tSel = { (LONG)(tRC.left + pt.x + m_tPictureOffset.x - m_tImageOffSet.x), (LONG)(tRC.top + pt.y +m_tPictureOffset.y - m_tImageOffSet.y),
			(LONG)(tRC.left + pt.x + m_tTileEnd.x - m_tImageOffSet.x + 32) , (LONG)(tRC.top + pt.y + m_tTileEnd.y - m_tImageOffSet.y + 32) };

		HPEN hPen = CreatePen(PS_DOT, 1, RGB(0, 0, 125));
		HPEN hPrevPen = (HPEN)SelectObject(hDC, hPen);

		MoveToEx(hDC, tSel.left, tSel.top, NULL);
		LineTo(hDC, tSel.right, tSel.top);
		LineTo(hDC, tSel.right, tSel.bottom);
		LineTo(hDC, tSel.left, tSel.bottom);
		LineTo(hDC, tSel.left, tSel.top);

		SelectObject(hDC, hPrevPen);
		DeleteObject(hPen);

#ifdef _DEBUG
		TCHAR strPos[32] = {};
		swprintf_s(strPos, TEXT("PosH: %d"), m_iHPos);
		TextOut(hDC, 0, 0, strPos, lstrlen(strPos));

		TCHAR strVPos[32] = {};
		swprintf_s(strVPos, TEXT("PosV: %d"), m_iVPos);
		TextOut(hDC, 0, 20, strVPos, lstrlen(strVPos));
#endif
	}
}

void CMapEditDlg::SetImagePos(int iPos)
{
	RECT tRC = {};

	HWND hPicture = GetDlgItem(m_hWnd, IDC_PICTURE);

	GetWindowRect(hPicture, &tRC);

	if (m_pTexture)
	{
		long lWidth = m_pTexture->GetWidth();

		m_tImageOffSet.x = lWidth * iPos / 100.f;

		if (m_tImageOffSet.x > lWidth - tRC.right + tRC.left)
			m_tImageOffSet.x = (float)lWidth - tRC.right + tRC.left;
	}
}

void CMapEditDlg::SetImageVPos(int iPos)
{
	RECT tRC = {};

	HWND hPicture = GetDlgItem(m_hWnd, IDC_PICTURE);

	GetWindowRect(hPicture, &tRC);

	if (m_pTexture)
	{
		long lHeight = m_pTexture->GetHeight();

		m_tImageOffSet.y = lHeight * iPos / 100.f;

		if (m_tImageOffSet.y > lHeight - tRC.bottom + tRC.top)
			m_tImageOffSet.y = (float)lHeight - tRC.bottom + tRC.top;
	}
}

void CMapEditDlg::SetWindowOffSet()
{
	POINT pt;
	GetCursorPos(&pt);
	
	ScreenToClient(m_hPicture, &pt);

	m_tPictureOffset = pt;

	if (m_tPictureOffset.x < 0.f || m_tPictureOffset.y <0.f)
		return;

	POSITION tTotal = m_tImageOffSet + m_tPictureOffset;

	tTotal /= 32;

	m_tPictureOffset.x -= m_tPictureOffset.x - (int)tTotal.x*32;
	m_tPictureOffset.y -= m_tPictureOffset.y - (int)tTotal.y*32;

	SetDlgItemInt(m_hWnd, IDC_EDIT_START_X, (UINT)(m_tPictureOffset.x), true);
	SetDlgItemInt(m_hWnd, IDC_EDIT_START_Y, (UINT)( m_tPictureOffset.y), true);
	SetDlgItemInt(m_hWnd, IDC_EDIT_END_X, (UINT)(m_tPictureOffset.x + 32), true);
	SetDlgItemInt(m_hWnd, IDC_EDIT_END_Y, (UINT)( m_tPictureOffset.y + 32), true);

	m_bPictureClick = true;
	m_tTileStart = m_tPictureOffset;
	m_tTileEnd = m_tPictureOffset;
}

void CMapEditDlg::SetTextureColorKey()
{
	POINT pt;
	GetCursorPos(&pt);

	ScreenToClient(m_hPicture, &pt);

	m_tPictureOffset = pt;

	if (m_tPictureOffset.x < 0.f || m_tPictureOffset.y < 0.f)
		return;

	COLORREF tColor = GetPixel(m_pTexture->GetDC(), pt.x + (int)m_tImageOffSet.x, pt.y + (int)m_tImageOffSet.y);
	m_pTexture->SetColorKey(tColor);
}

void CMapEditDlg::SetTileEnd()
{
	POINT pt;
	GetCursorPos(&pt);

	ScreenToClient(m_hPicture, &pt);

	RECT tRC = {};
	GetWindowRect(m_hPicture, &tRC);

	POSITION tPos = pt;

	if (pt.x < 0.f || pt.y < 0.f)
		return;

	if (pt.x > tRC.right - tRC.left || pt.y > tRC.bottom - tRC.top)
		return;

	POSITION tTotal = m_tImageOffSet + tPos;

	tTotal /= 32;

	tPos.x -= tPos.x - (int)tTotal.x * 32;
	tPos.y -= tPos.y - (int)tTotal.y * 32;

	SetDlgItemInt(m_hWnd, IDC_EDIT_END_X, (UINT)(tPos.x + 32), true);
	SetDlgItemInt(m_hWnd, IDC_EDIT_END_Y, (UINT)(tPos.y + 32), true);

	m_tTileEnd = tPos;
}

void CMapEditDlg::Save(TCHAR* pFile)
{
	char strFile[MAX_PATH] = {};

#ifdef UNICODE
	WideCharToMultiByte(CP_ACP, 0, pFile, -1, strFile, lstrlen(pFile), NULL, NULL);
#else
	strcpy_s(strFile, pFile);
#endif

	FILE* pSaveFile = nullptr;

	fopen_s(&pSaveFile, strFile, "wb");

	if (!pSaveFile)
		return;

	if (m_pStage)
		m_pStage->Save(pSaveFile);
/*
	Layer* pLayer = GET_SINGLE(SceneManager)->GetScene()->FindLayer("Default");

	if (pLayer)
		pLayer->Save(pSaveFile);*/

	fclose(pSaveFile);
}

void CMapEditDlg::Load(TCHAR* pFile)
{
	char strFile[MAX_PATH] = {};

#ifdef UNICODE
	WideCharToMultiByte(CP_ACP, 0, pFile, -1, strFile, lstrlen(pFile), NULL, NULL);
#else
	strcpy_s(strFile, pFile);
#endif

	FILE* pLoadFile = nullptr;

	fopen_s(&pLoadFile, strFile, "rb");

	if (!pLoadFile)
		return;

	if (m_pStage)
		m_pStage->Load(pLoadFile);

	/*Layer* pLayer = GET_SINGLE(SceneManager)->GetScene()->FindLayer("Default");

	if (pLayer)
		pLayer->Load(pLoadFile);*/

	fclose(pLoadFile);
}

void CMapEditDlg::LoadTexture(TCHAR* pFileName)
{
	TCHAR strName[_MAX_FNAME] = {};
	TCHAR strDir[_MAX_DIR] = {};
	TCHAR strDrive[_MAX_DRIVE] = {};
	TCHAR strExt[_MAX_EXT] = {};

	_wsplitpath_s(pFileName, strDrive, strDir, strName, strExt);

	char pName[MAX_PATH] = {};

#ifdef UNICODE
	WideCharToMultiByte(CP_ACP, NULL, strName, -1, pName, lstrlen(strName), NULL, NULL);
#else
	strcpy_s(pName, strName);
#endif

	TCHAR strFileName[MAX_PATH] = {};

	lstrcat(strFileName, strName);
	lstrcat(strFileName, strExt);

	SAFE_RELEASE(m_pTexture);

	m_pTexture = GET_SINGLE(ResourcesManager)->LoadTextureFromFullPath(pName, pFileName);

	SendMessage(m_hTextureList, LB_ADDSTRING, 0, (LPARAM)strName);
}

LRESULT CMapEditDlg::WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hDC = GetDC(hWnd);
	int iPos;
	RECT tRC = {};
	TVITEMEX TvEx;


	if (g_pDlg)
	{
		g_pDlg->Render(hDC, 0.f);
	}

	switch (iMessage)
	{
	case WM_INITDIALOG:
		if (g_pDlg)
		{
			g_pDlg->m_hWnd = hWnd;
			g_pDlg->m_hBackDC = CreateCompatibleDC(hDC);
			g_pDlg->m_hBackBitmap = CreateCompatibleBitmap(hDC, 648, 437);
			g_pDlg->m_hPrevBitmap = (HBITMAP)SelectObject(g_pDlg->m_hBackDC, g_pDlg->m_hBackBitmap);
		}
		break;
	case WM_COMMAND:
		if (HIWORD(wParam) == BN_CLICKED)
		{
				switch (LOWORD(wParam))
				{
				case IDC_RADIO_PEN:
					if (g_pDlg)
					g_pDlg->m_eBurshMode = BM_PEN;
					break;
				case IDC_RADIO_RECT:
					if (g_pDlg)
					g_pDlg->m_eBurshMode = BM_RECT;
					break;
				}
		}

		switch (LOWORD(wParam))
		{
		case IDOK:
			g_pDlg->CreateTileMap();
			break;
		case IDCANCEL:
			DestroyWindow(hWnd);
			break;
		case IDC_BUTTON_SAVE:
			g_pDlg->Save();
			break;
		case IDC_BUTTON_LOAD:
			g_pDlg->Load();
			break;
		case IDC_LIST_TEXTURE:
			g_pDlg->SelectTexture();
			break;
		case IDC_BUTTON_TEXTURE_LOAD:
			g_pDlg->LoadTexture();
			break;
		case IDC_PICTURE:

			HWND hCheck = GetDlgItem(hWnd, IDC_CHECK_COLORKEY);

			if (g_pDlg)
			{
				if (SendMessage(hCheck, BM_GETCHECK, 0, 0) == BST_UNCHECKED)
					g_pDlg->SetWindowOffSet();

				else
					g_pDlg->SetTextureColorKey();

				SendMessage(g_pDlg->m_hBrushCombo, CB_SETCURSEL, 0, 0);
			}

			break;
		}
		break;
	case WM_NOTIFY:
		LPNMHDR hdr;
		LPNMTREEVIEW ntv;
		TCHAR strCap[32];

		hdr = (LPNMHDR)lParam;
		ntv = (LPNMTREEVIEW)lParam;

		if (g_pDlg)
		{
			if (hdr->hwndFrom == g_pDlg->m_hTreeObj)
			{
				switch (hdr->code)
				{
				case TVN_SELCHANGED:
					TvEx.mask = TVIF_PARAM | TVIF_IMAGE | TVIF_TEXT;
					TvEx.hItem = ntv->itemNew.hItem;
					TvEx.pszText = strCap;
					TvEx.cchTextMax = 30;
					TreeView_GetItem(g_pDlg->m_hTreeObj, &TvEx);
					MapEditScene* pScene = (MapEditScene*)GET_SINGLE(SceneManager)->GetScene();
					pScene->SetEditTileTex((int)TvEx.lParam);

					SendMessage(g_pDlg->m_hBrushCombo, CB_SETCURSEL, 2, 0);

					break;
				}
			}
		}
		break;
	case WM_HSCROLL:
		iPos = g_pDlg->m_iHPos;

		switch (LOWORD(wParam))
		{
		case SB_LINELEFT:
			iPos = max(0, iPos - 1);
			break;
		case SB_LINERIGHT:
			iPos = min(100, iPos + 1);
			break;
		case SB_PAGELEFT:
			iPos = max(0, iPos - 10);
			break;
		case SB_PAGERIGHT:
			iPos = min(100, iPos + 10);
			break;
		case SB_THUMBTRACK:
			iPos = HIWORD(wParam);

			g_pDlg->Render(hDC, 0.f);
			break;
		}

		g_pDlg->m_iHPos = iPos;
		g_pDlg->SetImagePos(iPos);
		iPos = SetScrollPos((HWND)lParam, SB_CTL, iPos, TRUE);

		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_VSCROLL:
		iPos = g_pDlg->m_iVPos;

		switch (LOWORD(wParam))
		{
		case SB_LINELEFT:
			iPos = max(0, iPos - 1);
			break;
		case SB_LINERIGHT:
			iPos = min(100, iPos + 1);
			break;
		case SB_PAGELEFT:
			iPos = max(0, iPos - 10);
			break;
		case SB_PAGERIGHT:
			iPos = min(100, iPos + 10);
			break;
		case SB_THUMBTRACK:
			iPos = HIWORD(wParam);
			g_pDlg->Render(hDC, 0.f);
			break;
		}

		g_pDlg->m_iVPos = iPos;
		g_pDlg->SetImageVPos(iPos);
		iPos = SetScrollPos((HWND)lParam, SB_CTL, iPos, TRUE);

		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_CLOSE:
		g_pDlg->Clear();
		break;
	case WM_DESTROY:
		SelectObject(g_pDlg->m_hBackDC, g_pDlg->m_hPrevBitmap);
		DeleteObject(g_pDlg->m_hBackBitmap);
		DeleteDC(g_pDlg->m_hBackDC);

		PostQuitMessage(0);
		g_pDlg->Clear();
		break;
	}

	ReleaseDC(hWnd, hDC);

	return LRESULT();
}

void CMapEditDlg::InsertChild(HTREEITEM pNode, int iPid)
{
	TVINSERTSTRUCT tTI;
	HTREEITEM hNode;

	MapEditScene* pScene = (MapEditScene*)GET_SINGLE(SceneManager)->GetScene();

	size_t n = pScene->GetProtoSize();

	for (size_t i = 0; i < n; ++i)
	{
		if (m_pObjInfo[i].iParent == iPid)
		{
			tTI.hParent = pNode;
			tTI.hInsertAfter = TVI_LAST;
			tTI.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
			tTI.item.iImage = m_pObjInfo[i].iImage;
			tTI.item.iSelectedImage = m_pObjInfo[i].iImage + 4;

			TCHAR strName[MAX_PATH] = {};
			char* strMultibyteName = pScene->GetProtoName((int)i);

			MultiByteToWideChar(CP_ACP, NULL, strMultibyteName, -1, strName, (int)strlen(strMultibyteName));

			tTI.item.pszText = strName;
			tTI.item.lParam = i;

			hNode = TreeView_InsertItem(m_hTreeObj, &tTI);

			InsertChild(hNode, (int)i);
		}
	}
}

void CMapEditDlg::SelectTreeItem()
{
}
