#include "AniEditDlg.h"
#include "Core.h"
#include "Core/PathManager.h"
#include "Scene/AnimationEditScene.h"
#include "Resources/ResourcesManager.h"
#include "Resources/Texture.h"
#include "Animation/Animation.h"
#include "Core/Camera.h"

CAniEditDlg* g_pDlg = nullptr;

CAniEditDlg::CAniEditDlg()	:
	m_hWnd(0),
	m_pAnimation(nullptr),
	m_pTexture(nullptr),
	m_pClip(nullptr),
	m_hComboOption(NULL),
	m_strFileName(nullptr),
	m_strName(nullptr),
	m_hDC(0),
	m_fFrameTime(0.f),
	m_hListClip(NULL),
	m_pDefaultClip(nullptr),
	m_iSelect(0),
	m_bDestroyed(true)
{
	g_pDlg = this;
}

CAniEditDlg::~CAniEditDlg()
{
	SAFE_RELEASE(m_pTexture);
	SAFE_RELEASE(m_pAnimation);

	if (m_pDefaultClip)
	{
		SAFE_DELETE(m_pDefaultClip);
	}
	else
		SAFE_DELETE(m_pClip);

	delete[] m_strFileName;
	delete[] m_strName;
	ReleaseDC(m_hWnd, m_hDC);
}

void CAniEditDlg::SetColorKey(const COLORREF& tColor)
{
	unsigned int iBlue = tColor & 0x000000ff;
	unsigned int iGreen = (tColor & 0x0000ff00) >> 8;
	unsigned int iRed =  (tColor & 0x00ff0000) >> 16;
	
	SetDlgItemInt(m_hWnd, IDC_EDIT_RED, iRed, false);
	SetDlgItemInt(m_hWnd, IDC_EDIT_GREEN, iGreen, false);
	SetDlgItemInt(m_hWnd, IDC_EDIT_BLUE, iBlue, false);

	if(m_pTexture)
		m_pTexture->SetColorKey(tColor);
}

void CAniEditDlg::ChangeColorKey()
{
	BOOL bTrans = false;

	int iRed = GetDlgItemInt(m_hWnd, IDC_EDIT_RED, &bTrans, true);
	int iGreen = GetDlgItemInt(m_hWnd, IDC_EDIT_GREEN, &bTrans, true);
	int iBlue = GetDlgItemInt(m_hWnd, IDC_EDIT_BLUE, &bTrans, true);

	SetColorKey(RGB(iRed, iGreen, iBlue));
}

void CAniEditDlg::OnEditDlg(int iID)
{
	m_hWnd = CreateDialog(WINDOWINSTANCE, MAKEINTRESOURCE(iID), WINDOWHANDLE, CAniEditDlg::DlgProc);
	ShowWindow(m_hWnd, SW_SHOW);

	ShowCursor(true);

	m_bDestroyed = false;

	m_hComboOption = GetDlgItem(m_hWnd, IDC_COMBO_OPTION);
	m_hListClip = GetDlgItem(m_hWnd, IDC_LIST_CLIP);

	SAFE_RELEASE(m_pAnimation);
	SAFE_DELETE(m_pClip);

	m_pAnimation = new Animation;
	m_pClip = new ANIMATIONCLIP;

	TCHAR strOption[][30] = {
		TEXT("Loop"),
		TEXT("Once Return"),
		TEXT("Once Destroy"),
		TEXT("Once Frame"),
		TEXT("Time Return"),
		TEXT("Time Destroy")
	};

	TCHAR strClip[30] = TEXT("EditClip");

	SendMessage(m_hListClip, LB_ADDSTRING, 0, (LPARAM)strClip);

	for(int i=0;i<sizeof(strOption) / sizeof(strOption[0]);++i)
		SendMessage(m_hComboOption, CB_ADDSTRING, 0, (LPARAM)strOption[i]);

	m_strName = new char[_MAX_FNAME];

	memset(m_strName, 0, sizeof(_MAX_FNAME));

	m_strFileName = new TCHAR[_MAX_DIR];

	memset(m_strFileName, 0, sizeof(TCHAR) * _MAX_DIR);
}

void CAniEditDlg::Clear()
{
	DestroyWindow(m_hWnd);

	g_pDlg = 0;

	ShowCursor(false);
}

void CAniEditDlg::Save()
{
	OPENFILENAME tOFN = {};

	TCHAR strFile[MAX_PATH] = {};

	tOFN.lpstrFilter = TEXT("모든 파일\0*.*\0애니메이션 파일\0*.sac");
	tOFN.lpstrInitialDir = GET_SINGLE(PathManager)->FindPath(DATA_PATH);
	tOFN.hwndOwner = m_hWnd;
	tOFN.lStructSize = sizeof(OPENFILENAME);
	tOFN.nMaxFile = 256;
	tOFN.lpstrFile = strFile;

	if (GetSaveFileName(&tOFN))
		Save(strFile);
}

void CAniEditDlg::Load()
{
	OPENFILENAME tOFN = {};

	TCHAR strFile[MAX_PATH] = {};

	tOFN.lpstrFilter = TEXT("모든 파일\0*.*\0애니메이션 파일\0*.sac");
	tOFN.lpstrInitialDir = GET_SINGLE(PathManager)->FindPath(DATA_PATH);
	tOFN.hwndOwner = m_hWnd;
	tOFN.lStructSize = sizeof(OPENFILENAME);
	tOFN.nMaxFile = 256;
	tOFN.lpstrFile = strFile;

	if (GetOpenFileName(&tOFN))
		Load(strFile);
}

void CAniEditDlg::LoadTexture()
{
	OPENFILENAME tOFN = {};

	TCHAR strFile[MAX_PATH] = {};

	tOFN.hInstance = WINDOWINSTANCE;
	tOFN.hwndOwner = m_hWnd;
	tOFN.lpstrFile = strFile;
	tOFN.lpstrFilter = TEXT("모든파일\0*.*\0비트맵파일\0*.bmp");
	tOFN.lpstrInitialDir = GET_SINGLE(PathManager)->FindPath(TEXTURE_PATH);
	tOFN.lStructSize = sizeof(OPENFILENAME);
	tOFN.nMaxFile = 256;
	
	if (GetOpenFileName(&tOFN))
		LoadTexture(strFile);
}

void CAniEditDlg::AddFrame(POSITION tStart, POSITION tEnd)
{
	if (m_pClip)
	{
		FrameInfo tInfo = {};

		tInfo.tStart = tStart;
		tInfo.tEnd = tEnd;

		m_pClip->vecFrame.push_back(tInfo);

		POSITION tRenderPos = {};

		m_pClip->vecRenderPos.push_back(tRenderPos);

		BOOL bTrans = false;

		SetDlgItemInt(m_hWnd, IDC_EDIT_RENDER_X, 0, true);
		SetDlgItemInt(m_hWnd, IDC_EDIT_RENDER_Y, 0, true);

		++m_pClip->iLengthX;
		++m_pClip->iFrameMaxX;

		size_t iSize = m_pClip->vecFrame.size();

		if (iSize > m_iSelect)
			SetPos(m_pClip->vecFrame[m_iSelect].tStart);
	}
}

void CAniEditDlg::AddClip()
{
	int iSel = (int)SendMessage(m_hComboOption, CB_GETCURSEL, 0, 0);

	TCHAR strTime[32] = {};

	GetDlgItemText(m_hWnd, IDC_EDIT_TIME, strTime, 32);

	float fTime = (float)_wtof(strTime);

	int iFrame = (int)m_pClip->vecFrame.size();

	TCHAR strName[64] = {};

	GetDlgItemText(m_hWnd, IDC_EDIT_NAME, strName, 64);

	BOOL bTrans = false;

	int iOptionFrame = GetDlgItemInt(m_hWnd, IDC_EDIT_FRAME, &bTrans, false);

	char pName[64] = {};

#ifdef _UNICODE
	WideCharToMultiByte(CP_ACP, NULL, strName, -1, pName, lstrlen(strName), NULL, NULL);
#else
	strcpy_s(pName, strName);
#endif

	auto iter = m_pAnimation->m_mapClip.find(pName);

	if (iter != m_pAnimation->m_mapClip.end())
		return;

	SetDlgItemText(m_hWnd, IDC_EDIT_NAME, TEXT(""));

	m_pAnimation->AddClip(pName, AT_ATLAS, (ANIMATION_OPTION)iSel, iFrame * fTime, iFrame, 1, 0, 0, iFrame, 1,
		1.f, iOptionFrame, m_strName, m_strFileName);

	SendMessage(m_hListClip, LB_ADDSTRING, 0, (LPARAM)strName);

	for (int i = 0; i < iFrame; ++i)
	{
		m_pAnimation->AddFrame(pName, m_pClip->vecFrame[i]);

		m_pAnimation->AddRenderPos(pName, m_pClip->vecRenderPos[i]);
	}
}

void CAniEditDlg::DeleteClip()
{
	int iSel = (int)SendMessage(m_hListClip, LB_GETCURSEL, 0, 0);

	if (iSel == 0)
	{
		if (!m_pDefaultClip)
		{
			SAFE_DELETE(m_pClip);

			m_pClip = new ANIMATIONCLIP;
		}

		else
		{
			SAFE_DELETE(m_pDefaultClip);

			m_pDefaultClip = new ANIMATIONCLIP;
		}
	}

	else
	{
		TCHAR strClip[MAX_PATH] = {};

		SendMessage(m_hListClip, LB_GETTEXT, iSel, (LPARAM)strClip);

		char pClip[MAX_PATH] = {};

#ifdef _UNICODE
		WideCharToMultiByte(CP_ACP, NULL, strClip, -1, pClip, lstrlen(strClip), NULL, NULL);
#else
		strcpy_s(pClip, strClip);
#endif

		auto iter = m_pAnimation->m_mapClip.find(pClip);

		if (iter == m_pAnimation->m_mapClip.end())
			return;

		iter->second->vecTexture[0]->Release();

		SAFE_DELETE(iter->second);

		m_pAnimation->m_mapClip.erase(iter);

		SendMessage(m_hListClip, LB_DELETESTRING, iSel, 0);

		if (!m_pAnimation->m_mapClip.empty())
		{
			iter = m_pAnimation->m_mapClip.begin();

			m_pClip = iter->second;
		}
	}
}

void CAniEditDlg::ModifyClip()
{
	int iSel = (int)SendMessage(m_hComboOption, CB_GETCURSEL, 0, 0);

	TCHAR strTime[32] = {};

	GetDlgItemText(m_hWnd, IDC_EDIT_TIME, strTime, 32);

	float fTime = (float)_wtof(strTime);

	int iFrame = (int)m_pClip->vecFrame.size();

	TCHAR strName[64] = {};

	GetDlgItemText(m_hWnd, IDC_EDIT_NAME, strName, 64);

	BOOL bTrans = false;

	int iOptionFrame = GetDlgItemInt(m_hWnd, IDC_EDIT_FRAME, &bTrans, false);

	char pName[64] = {};

#ifdef _UNICODE
	WideCharToMultiByte(CP_ACP, NULL, strName, -1, pName, lstrlen(strName), NULL, NULL);
#else
	strcpy_s(pName, strName);
#endif

	PANIMATIONCLIP pClip = m_pAnimation->FindClip(pName);

	if (pClip) 
	{
		pClip->eType = AT_ATLAS;

		pClip->eOption = (ANIMATION_OPTION)iSel;

		pClip->fAnimationLimitTime = iFrame * fTime;

		pClip->iFrameMaxX = iFrame;

		pClip->iLengthX = iFrame;

		pClip->fAnimationFrameTime = fTime;

		pClip->iOptionFrame = iOptionFrame;

		if (pClip->iFrameMaxX != 0)
			pClip->tFrameSize.x = (float)(pClip->vecTexture[0]->GetWidth() / pClip->iFrameMaxX);

		if (pClip->iFrameMaxY != 0)
			pClip->tFrameSize.y = (float)(pClip->vecTexture[0]->GetHeight());
	}
}

void CAniEditDlg::Update(float fTime)
{
	if (m_pClip)
	{
		m_fFrameTime += fTime;

		TCHAR strTime[32] = {};

		GetDlgItemText(m_hWnd, IDC_EDIT_TIME, strTime, 32);

		float fFrameTimeMax = (float)_wtof(strTime);

		if (m_fFrameTime >= fFrameTimeMax)
		{
			BOOL bTrans = false;

			int iFrame = GetDlgItemInt(m_hWnd, IDC_EDIT_CURRENT_FRAME, &bTrans, true);

			m_fFrameTime -= fFrameTimeMax;

			++iFrame;

			if (fFrameTimeMax == 0.f)
				iFrame = 0;

			size_t iSize = m_pClip->vecFrame.size();

			if (iFrame >= (int)iSize)
			{
				int iSel = (int)SendMessage(m_hComboOption, CB_GETCURSEL, 0, 0);

				iFrame = 0;
				switch (iSel)
				{
				case AO_LOOP:
					iFrame = 0;
					break;
				case AO_ONCE_FRAME:
					iFrame = m_pClip->iOptionFrame;
					break;
				default:
					break;
				}
			}

			SetDlgItemInt(m_hWnd, IDC_EDIT_CURRENT_FRAME, iFrame, true);
		}
	}

	TCHAR strSize[32] = {};

	GetDlgItemText(m_hWnd, IDC_EDIT_SIZE_X, strSize, 32 * sizeof(TCHAR));

	m_tSize.x = (float)_wtof(strSize);

	GetDlgItemText(m_hWnd, IDC_EDIT_SIZE_Y, strSize, 32 * sizeof(TCHAR));

	m_tSize.y = (float)_wtof(strSize);
}

void CAniEditDlg::Render()
{
	Texture* pBackTexture = GET_SINGLE(ResourcesManager)->LoadTexture("Back", TEXT("AnimationUI\\BackGround.bmp"));

	RECT tRC = {};

	HWND hPicture = GetDlgItem(m_hWnd, IDC_ANIMATION);

	GetWindowRect(hPicture, &tRC);

	POINT pt = {};

	ScreenToClient(m_hWnd, &pt);

	RECT tWinRC = {};

	GetWindowRect(m_hWnd, &tWinRC);

	BitBlt(m_hDC, tRC.left + pt.x, tRC.top + pt.y, tRC.right - tRC.left, tRC.bottom - tRC.top, pBackTexture->GetDC(), 0, 0, SRCCOPY);

	if (m_pTexture)
	{
		bool bColorKey = m_pTexture->GetColorKeyEnable();

		if(bColorKey)
		TransparentBlt(m_hDC, tRC.left + pt.x, tRC.top + pt.y,
			32, 64, m_pTexture->GetDC(),
			(int)m_tBasePos.x, (int)m_tBasePos.y, 32, 64, m_pTexture->GetColorKey());

		else
			BitBlt(m_hDC, tRC.left + pt.x, tRC.top + pt.y,
				32, 64, m_pTexture->GetDC(),
				(int)m_tBasePos.x, (int)m_tBasePos.y, SRCCOPY);
	}

	if (m_pAnimation)
	{
		if (m_pClip)
		{
			if (m_pTexture)
			{
				BOOL bTrans = false;

				int iFrame = GetDlgItemInt(m_hWnd, IDC_EDIT_CURRENT_FRAME, &bTrans, true);

				if (m_pClip->vecFrame.size() > iFrame)
				{
					int iWidth = (int)(m_pClip->vecFrame[iFrame].tEnd.x - m_pClip->vecFrame[iFrame].tStart.x);
					int iHeight = (int)(m_pClip->vecFrame[iFrame].tEnd.y - m_pClip->vecFrame[iFrame].tStart.y);

					bool bColorKey = m_pTexture->GetColorKeyEnable();

					if (bColorKey)
					TransparentBlt(m_hDC, tRC.left + pt.x + (int)m_pClip->vecRenderPos[iFrame].x, tRC.top + pt.y + (int)m_pClip->vecRenderPos[iFrame].y,
						iWidth, iHeight, m_pTexture->GetDC(),
						(int)m_pClip->vecFrame[iFrame].tStart.x, (int)m_pClip->vecFrame[iFrame].tStart.y, iWidth, iHeight, m_pTexture->GetColorKey());

					else
						BitBlt(m_hDC, tRC.left + pt.x + (int)m_pClip->vecRenderPos[iFrame].x, tRC.top + pt.y + (int)m_pClip->vecRenderPos[iFrame].y,
							iWidth, iHeight, m_pTexture->GetDC(),
							(int)m_pClip->vecFrame[iFrame].tStart.x, (int)m_pClip->vecFrame[iFrame].tStart.y, SRCCOPY);
				}
			}
		}
	}

	SAFE_RELEASE(pBackTexture);
}

void CAniEditDlg::ChangeCurrentClip()
{
	TCHAR strClip[MAX_PATH] = {};

	int iSel = (int)SendMessage(m_hListClip, LB_GETCURSEL, 0, 0);

	if (iSel == 0)
	{
		if (m_pDefaultClip)
		{
			m_pClip = m_pDefaultClip;
		}

		return;
	}

	SendMessage(m_hListClip, LB_GETTEXT, iSel, (LPARAM)strClip);

	char pClip[MAX_PATH] = {};

#ifdef _UNICODE
	WideCharToMultiByte(CP_ACP, NULL, strClip, -1, pClip, lstrlen(strClip), NULL, NULL);
#else
	strcpy_s(pClip, strClip);
#endif

	auto iter = m_pAnimation->m_mapClip.find(pClip);

	if (iter == m_pAnimation->m_mapClip.end())
		return;

	if (!m_pDefaultClip)
		m_pDefaultClip = m_pClip;

	m_pClip = iter->second;

	TCHAR strTime[32] = {};

	swprintf_s(strTime, TEXT("%.4f"), m_pClip->fAnimationFrameTime);

	SetDlgItemText(m_hWnd, IDC_EDIT_TIME, strTime);

	SetDlgItemText(m_hWnd, IDC_EDIT_NAME, strClip);

	SetDlgItemInt(m_hWnd, IDC_EDIT_FRAME, m_pClip->iOptionFrame, false);

	SendMessage(m_hComboOption, CB_SETCURSEL, (WPARAM)m_pClip->eOption, 0);

	m_pClip->iFrameX = 0;
}

void CAniEditDlg::SetFramePos()
{
	if (m_pClip)
	{
		size_t iSize = m_pClip->vecFrame.size();

		if (iSize > m_iSelect)
		{
			BOOL bTrans = false;

			float fPosX = (float)GetDlgItemInt(m_hWnd, IDC_EDIT_POS_X, &bTrans, false);
			float fPosY = (float)GetDlgItemInt(m_hWnd, IDC_EDIT_POS_Y, &bTrans, false);

			m_pClip->vecFrame[m_iSelect].tStart.x = fPosX;
			m_pClip->vecFrame[m_iSelect].tStart.y = fPosY;

			float fSizeX = (float)GetDlgItemInt(m_hWnd, IDC_EDIT_SIZE_X, &bTrans, false);
			float fSizeY = (float)GetDlgItemInt(m_hWnd, IDC_EDIT_SIZE_Y, &bTrans, false);

			m_pClip->vecFrame[m_iSelect].tEnd.x = fPosX + fSizeX;
			m_pClip->vecFrame[m_iSelect].tEnd.y = fPosY + fSizeY;
		}
	}
}

void CAniEditDlg::ChangeRenderPos()
{
	if (m_pClip)
	{
		BOOL bTrans = false;

		int iX = GetDlgItemInt(m_hWnd, IDC_EDIT_RENDER_X, &bTrans, true);
		int iY = GetDlgItemInt(m_hWnd, IDC_EDIT_RENDER_Y, &bTrans, true);

		m_pClip->vecRenderPos[m_iSelect].x = (float)iX;
		m_pClip->vecRenderPos[m_iSelect].y = (float)iY;
	}
}

void CAniEditDlg::ChangeOption()
{
	int iSel = (int)SendMessage(m_hComboOption, CB_GETCURSEL, 0, 0);

	if (iSel == AO_ONCE_FRAME)
	{
		HWND hWnd = GetDlgItem(m_hWnd, IDC_EDIT_FRAME);

		SendMessage(hWnd, EM_SETREADONLY, false, 0);
	}

	else
	{
		HWND hWnd = GetDlgItem(m_hWnd, IDC_EDIT_FRAME);

		SetActiveWindow(hWnd);

		SendMessage(hWnd, EM_SETREADONLY, true, 0);
	}
}

void CAniEditDlg::ChangeOptionFrame()
{
	BOOL bTrans = false;

	int iFrame = GetDlgItemInt(m_hWnd, IDC_EDIT_FRAME, &bTrans, false);

	m_pClip->iOptionFrame = iFrame;
}

void CAniEditDlg::Save(const TCHAR* pFileName)
{
	char strFileName[MAX_PATH] = {};

#ifdef _UNICODE
	WideCharToMultiByte(CP_ACP, NULL, pFileName, -1, strFileName, lstrlen(pFileName), NULL, NULL);
#else
	strcpy_s(pFileName, strFileName);
#endif

	FILE* pFile = nullptr;

	fopen_s(&pFile, strFileName, "wb");

	if (pFile)
	{
		m_pAnimation->Save(pFile);

		fclose(pFile);
	}
}

void CAniEditDlg::Load(const TCHAR* pFileName)
{
	char strFileName[MAX_PATH] = {};

#ifdef _UNICODE
	WideCharToMultiByte(CP_ACP, NULL, pFileName, -1, strFileName, lstrlen(pFileName), NULL, NULL);
#else
	strcpy_s(pFileName, strFileName);
#endif

	FILE* pFile = nullptr;

	fopen_s(&pFile, strFileName, "rb");

	if (pFile)
	{
		m_pAnimation->Load(pFile);

		int iCount = (int)SendMessage(m_hListClip, LB_GETCOUNT, 0, 0);

		for(int i=1;i<iCount;++i)
			SendMessage(m_hListClip, LB_DELETESTRING, i, 0);

		auto iter = m_pAnimation->m_mapClip.begin();
		auto iterEnd = m_pAnimation->m_mapClip.end();

		for (; iter != iterEnd; ++iter)
		{
			TCHAR strClip[MAX_PATH] = {};

#ifdef _UNICODE
			MultiByteToWideChar(CP_ACP, NULL, iter->first.c_str(), -1, strClip, (int)iter->first.length());
#else
			strcpy_s(strClip, iter->first.c_str());
#endif

			SendMessage(m_hListClip, LB_ADDSTRING, 1, (LPARAM)strClip);
		}

		fclose(pFile);
	}
}

void CAniEditDlg::LoadTexture(const TCHAR* pFileName)
{
	TCHAR strDrive[_MAX_DRIVE] = {};
	TCHAR strDir[_MAX_DIR] = {};
	TCHAR strName[_MAX_FNAME] = {};
	TCHAR strExt[_MAX_EXT] = {};

	_wsplitpath_s(pFileName, strDrive, strDir, strName, strExt);

	char pName[_MAX_FNAME] = {};

#ifdef _UNICODE
	WideCharToMultiByte(CP_ACP, NULL, strName, -1, pName, lstrlen(strName), NULL , NULL);
#else
	strcpy_s(pName, strName);
#endif

	SAFE_RELEASE(m_pTexture);

	m_pTexture = GET_SINGLE(ResourcesManager)->LoadTextureFromFullPath(pName, pFileName);

	float fWidth = (float)m_pTexture->GetWidth();
	float fHeight = (float)m_pTexture->GetHeight();

	RESOLUTION tCamResolution = GET_SINGLE(Camera)->GetWorldRS();

	if (tCamResolution.iH < fHeight || tCamResolution.iW < fWidth)
		GET_SINGLE(Camera)->SetWorldResolution((int)fWidth, (int)fHeight);

	strcpy_s(m_strName,strlen(pName) + 1,pName);

	lstrcpy(m_strFileName, pFileName);
}


INT_PTR __stdcall CAniEditDlg::DlgProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_INITDIALOG:
		g_pDlg->m_hWnd = hWnd;
		g_pDlg->m_hDC = GetDC(hWnd);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			DestroyWindow(hWnd);
			break;
		case ID_TEXTURE_LOAD:
			g_pDlg->LoadTexture();
			break;
		case IDC_BUTTON_ANI_SAVE:
			g_pDlg->Save();
			break;
		case ID_CLIP_ADD:
			g_pDlg->AddClip();
			break;
		case ID_ANI_LOAD:
			g_pDlg->Load();
			break;
		case IDC_BUTTON_COLORKEY:
			g_pDlg->ChangeColorKey();
			break;
		case IDC_LIST_CLIP:
			switch (HIWORD(wParam))
			{
			case LBN_SELCHANGE:
				g_pDlg->ChangeCurrentClip();
				break;
			}
			break;
		case ID_CLIP_DELETE:
			g_pDlg->DeleteClip();
			break;
		case IDC_BUTTON_POS:
			g_pDlg->SetFramePos();
			break;
		case ID_CLIP_MODIFY:
			g_pDlg->ModifyClip();
			break;
		case IDC_BUTTON_RENDER:
			g_pDlg->ChangeRenderPos();
			break;
		case IDC_COMBO_OPTION:
			g_pDlg->ChangeOption();
			break;
		case IDC_EDIT_FRAME:
			g_pDlg->ChangeOptionFrame();
			break;
		}
		break;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		g_pDlg->m_bDestroyed = true;
		ShowCursor(false);
		break;
	}


	return INT_PTR();
}
