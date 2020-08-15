#include "AnimationEditScene.h"
#include "..//Core/Camera.h"
#include "..//Object/Obj.h"
#include "..//Object/Stage.h"
#include "..//Core/Input.h"
#include "..//Object/UIPanel.h"
#include "..//Core.h"
#include "..//Object/Stage.h"
#include "..//Core/PathManager.h"
#include "..//Object/UIPanel.h"
#include "../resource.h"
#include "..//Resources/ResourcesManager.h"
#include "..//Resources/Texture.h"
#include "../AniEditDlg.h"

CAnimationEditScene* g_pDlg = nullptr;

TCHAR CAnimationEditScene::m_strText[MAX_PATH] = {};
UIPanel* CAnimationEditScene::m_pBody = nullptr;
PAniInfo	CAnimationEditScene::m_pAniInfo = nullptr;
vector<vector<COLORREF>>		CAnimationEditScene::m_vecvecPixel[AP_END];
int		CAnimationEditScene::m_iBasePos=0;
int		CAnimationEditScene::m_iUnderwearPos=0;
int		CAnimationEditScene::m_iHairPos=0;
int		CAnimationEditScene::m_iShirtPos=0;
int		CAnimationEditScene::m_iArmPos=0;


CAnimationEditScene::CAnimationEditScene()	:
	m_pStage(nullptr),
	m_tPos(16.f, 32.f),
	m_fAnimationTime(0.f),
	m_fLimitTime(0.5f),
	m_iFrame(0),
	m_iEditMode(0),
	m_tHaitCopyPos(),
	m_iDir(0),
	m_pHair(nullptr),
	m_pShirt(nullptr),
	m_pAniEditDlg(nullptr),
	m_hWnd(0)
{
	m_vecPixel.resize(32 * 64 * 6);
	m_vecHair.resize(32 * 64 * 6);
	m_vecShirt.resize(32 * 64 * 6);
	m_vecUnderwear.resize(32 * 64 * 6);
	m_vecArm.resize(32 * 64 * 6);
	memset(&m_vecPixel[0], 0, sizeof(PIXEL) * 32 * 64 * 6);
	memset(&m_vecUnderwear[0], 0, sizeof(PIXEL) * 32 * 64 * 6);
	memset(&m_vecArm[0], 0, sizeof(PIXEL) * 32 * 64 * 6);
	memset(&m_vecHair[0], 0, sizeof(PIXEL) * 32 * 64 * 6);
	memset(&m_vecShirt[0], 0, sizeof(PIXEL) * 32 * 64 * 6);
	m_pAniInfo = new AniInfo;
	g_pDlg = this;
}

CAnimationEditScene::~CAnimationEditScene()
{
	SAFE_RELEASE(m_pStage);
	SAFE_RELEASE(m_pHair);
	SAFE_RELEASE(m_pBody);
	SAFE_RELEASE(m_pShirt);
	SAFE_DELETE(m_pAniInfo);
	SAFE_DELETE(m_pAniEditDlg);
}

bool CAnimationEditScene::Init()
{
	Layer* pLayer = FindLayer("UI");

	UIPanel* pFrontPanel = Obj::CreateObj<UIPanel>("FrontPanel",
		pLayer);

	pFrontPanel->SetPos((float)(GETRESOLUTION.iW * 0.8f), 0.f);

	pFrontPanel->SetSize((float)(GETRESOLUTION.iW * 0.2f), (float)(GETRESOLUTION.iH));
	pFrontPanel->SetTexture("AniPanel", L"AnimationUI\\Panel.bmp");

	SAFE_RELEASE(pFrontPanel);

	Layer* pBack = FindLayer("Stage");

	Layer* pDafaultLayer = FindLayer("Default");

	Stage* pStage = Obj::CreateObj<Stage>("ShirtPanel",
		pDafaultLayer);

	pStage->SetPos(1024.f, 0.f);

	pStage->SetSize(128.f, 224.f);
	pStage->SetTexture("AniShirtPanel", L"Characters\\Farmer\\shirts.bmp");

	SAFE_RELEASE(pStage);

	pStage = Obj::CreateObj<Stage>("HairPanel",
		pDafaultLayer);

	pStage->SetPos(768.f, 0.f);

	pStage->SetSize(128.f, 512.f);
	pStage->SetTexture("AniHairPanel", L"Characters\\Farmer\\hairstyles.bmp");

	SAFE_RELEASE(pStage);

	pStage = Obj::CreateObj<Stage>("BackStage",
		pBack);
	
	pStage->SetPos(0.f, 0.f);
	
	pStage->SetSize((float)GETRESOLUTION.iW, (float)GETRESOLUTION.iH);
	pStage->SetTexture("BackStage", L"AnimationUI\\BackGround.bmp");
	
	SAFE_RELEASE(pStage);

	m_pStage = Obj::CreateObj<Stage>("farmerBase", pDafaultLayer);
	m_pStage->SetSize(1.f, 1.f);
	m_pStage->SetTexture("AnimationSrc", L"Characters\\Farmer\\farmer_girl_base.bmp");


	m_pBody = Obj::CreateObj<UIPanel>("Body",
		pLayer);
	m_pBody->SetPos(GETRESOLUTION.iW * 0.9f - 16.f, GETRESOLUTION.iH * 0.25f - 32.f);

	m_pBody->SetSize(32.f, 64.f);
	m_pBody->SetTexture("Body", L"Characters\\Farmer\\farmer_girl_base.bmp");
	m_pBody->SetColorKey(255, 0, 255);


	m_pShirt = Obj::CreateObj<UIPanel>("Shirt",
		pLayer);
	m_pShirt->SetPos(GETRESOLUTION.iW * 0.9f - 16.f, GETRESOLUTION.iH * 0.25f - 32.f);
	
	m_pShirt->SetSize(16.f, 16.f);
	m_pShirt->SetTexture("Shirt", L"Characters\\Farmer\\shirts.bmp");
	m_pShirt->SetColorKey(255, 255, 255);

	m_pHair = Obj::CreateObj<UIPanel>("Hair",
		pLayer);
	m_pHair->SetPos(GETRESOLUTION.iW * 0.9f - 16.f, GETRESOLUTION.iH * 0.25f - 32.f);

	m_pHair->SetSize(32.f, 64.f);
	m_pHair->SetTexture("Hair", L"Characters\\Farmer\\hairstyles.bmp");
	m_pHair->SetColorKey(255, 255, 255);



	return true;
}

void CAnimationEditScene::Input(float fDeltaTime)
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
		GET_SINGLE(Camera)->Scroll(-300.f * fDeltaTime, 0.f);
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		GET_SINGLE(Camera)->Scroll(300.f * fDeltaTime, 0.f);
	}

	if (GetAsyncKeyState('1') & 0x8000)
	{
		m_iEditMode = AEM_BODY;
	}

	if (GetAsyncKeyState('2') & 0x8000)
	{
		m_iEditMode = AEM_HAIR;
	}

	if (GetAsyncKeyState('3') & 0x8000)
	{
		m_iEditMode = AEM_SHIRT;
	}

	if (GetAsyncKeyState('4') & 0x8000)
	{
		ShowCursor(TRUE);
		DialogBox(WINDOWINSTANCE, MAKEINTRESOURCE(IDD_DIALOG2),
			WINDOWHANDLE, CAnimationEditScene::DlgProc2);
		ShowCursor(FALSE);
	}

	if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
	{
		memset(&m_vecPixel[0], 0, sizeof(COLORREF) * 32 * 64 * 6);
		m_iFrame = 0;
	}

	POSITION tPos;
	_SIZE tOffset;
	_SIZE tSize;

	if (KEYDOWN("FocusUp"))
	{
		switch (m_iEditMode)
		{
		case AEM_BODY:
			m_tPos.y -= 64.f;
			if (m_tPos.y < 32.f)
				m_tPos.y = 32.f;
			m_iDir = (--m_iDir) % 4;
			tOffset = m_pBody->GetImageOffset();
			tSize = m_pBody->GetSize();
			m_pBody->SetImageOffset(tOffset.x, tOffset.y - tSize.y);
			break;
		case AEM_HAIR:
			tPos = m_pHair->GetPos();
			m_pHair->SetPos(tPos.x, tPos.y-1);
			break;
		case AEM_SHIRT:
			tPos = m_pShirt->GetPos();
			m_pShirt->SetPos(tPos.x, tPos.y - 1);
			break;
		}
	}

	if (KEYDOWN("FocusDown"))
	{
		switch (m_iEditMode)
		{
		case AEM_BODY:
			m_tPos.y+=64.f;
			m_iDir = (++m_iDir) % 4;
			tOffset = m_pBody->GetImageOffset();
			tSize = m_pBody->GetSize();
			m_pBody->SetImageOffset(tOffset.x, tOffset.y + tSize.y);
			break;
		case AEM_HAIR:
			tPos = m_pHair->GetPos();
			m_pHair->SetPos(tPos.x, tPos.y + 1);
			break;
		case AEM_SHIRT:
			tPos = m_pShirt->GetPos();
			m_pShirt->SetPos(tPos.x, tPos.y + 1);
			break;
		}
	}

	if (KEYDOWN("FocusLeft"))
	{
		switch (m_iEditMode)
		{
		case AEM_BODY:
			m_tPos.x -= 32.f;
			tOffset = m_pBody->GetImageOffset();
			tSize = m_pBody->GetSize();
			m_pBody->SetImageOffset(tOffset.x - tSize.x, tOffset.y);
			break;
		case AEM_HAIR:
			tPos = m_pHair->GetPos();
			m_pHair->SetPos(tPos.x-1, tPos.y);
			break;
		case AEM_SHIRT:
			tPos = m_pShirt->GetPos();
			m_pShirt->SetPos(tPos.x-1, tPos.y);
			break;
		}
	}

	if (KEYDOWN("FocusRight"))
	{
		switch (m_iEditMode)
		{
		case AEM_BODY:
			m_tPos.x += 32.f;
			tOffset = m_pBody->GetImageOffset();
			tSize = m_pBody->GetSize();
			m_pBody->SetImageOffset(tOffset.x + tSize.x, tOffset.y);
			break;
		case AEM_HAIR:
			tPos = m_pHair->GetPos();
			m_pHair->SetPos(tPos.x+1, tPos.y);
			break;
		case AEM_SHIRT:
			tPos = m_pShirt->GetPos();
			m_pShirt->SetPos(tPos.x+1, tPos.y);
			break;
		}
	}

	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		POSITION tPos = GET_SINGLE(Input)->GetMouseWorldPos();
		switch (m_iEditMode)
		{
		case AEM_BODY:
			m_tPos = tPos;
			m_pBody->SetImageOffset(m_tPos);
			break;
		case AEM_HAIR:
			break;
		case AEM_SHIRT:
			break;
		}
	}

	if (KEYDOWN("Fire"))	//	Space
	{
		switch (m_iEditMode)
		{
		case AEM_BODY:
			break;
		case AEM_HAIR:
			tOffset = m_pHair->GetImageOffset();
			tSize = m_pHair->GetSize();
			m_pHair->SetImageOffset(tOffset.x, tOffset.y + tSize.y);
			if(tOffset.y >= 1024.f)
				m_pHair->SetImageOffset(tOffset.x, 0.f);
			break;
		case AEM_SHIRT:
			tOffset = m_pShirt->GetImageOffset();
			tSize = m_pShirt->GetSize();
			m_pShirt->SetImageOffset(tOffset.x, tOffset.y + tSize.y);
			if (tOffset.y >= 448.f)
				m_pShirt->SetImageOffset(tOffset.x, 0.f);
			//m_tShirtPos = tPos;
			break;
		}
		//m_iDir = (++m_iDir) % 4;
		
	}

	if (KEYDOWN("NextFrame"))	//	ctrl + N
	{
		if (m_iFrame < 6)
		{
			POSITION tPos = {};
			switch (m_iEditMode)
			{
			case AEM_BODY:
				m_pBody->SaveToVector(m_vecPixel, 6, m_iFrame, tPos);
				//m_pBody->SaveToVector(m_vecUnderwear, 6, m_iFrame, 96, tPos);
				for (int i = 0; i < 64; ++i)
				{
					for (int j = 0; j < 32; ++j)
					{
						m_pAniInfo->vecPixel[i * 192 + j + m_iFrame * 32] =
							m_vecPixel[i * 192 + j + m_iFrame * 32];
					}
				}
				break;

			case AEM_HAIR:
				tPos = m_pHair->GetPos();
				tPos.x -= GETRESOLUTION.iW * 0.9f - 16.f;
				tPos.y -= GETRESOLUTION.iH * 0.25f - 32.f;
				m_pHair->SaveToVector(m_vecPixel, 6, m_iFrame, tPos);
				for (int i = 0; i < 64; ++i)
				{
					for (int j = 0; j < 32; ++j)
					{
						m_pAniInfo->vecPixel[i * 192 + j + m_iFrame * 32] =
							m_vecPixel[i * 192 + j + m_iFrame * 32];
					}
				}
				break;

			case AEM_SHIRT:
				tPos = m_pShirt->GetPos();
				tPos.x -= GETRESOLUTION.iW * 0.9f - 16.f;
				tPos.y -= GETRESOLUTION.iH * 0.25f - 32.f;
				m_pShirt->SaveToVector(m_vecPixel, 6, m_iFrame, tPos);
				for (int i = 0; i < 64; ++i)
				{
					for (int j = 0; j < 32; ++j)
					{
						m_pAniInfo->vecPixel[i * 192 + j + m_iFrame * 32] =
							m_vecPixel[i * 192 + j + m_iFrame * 32];
					}
				}
				break;
				//m_pBody->SaveToVector(m_vecArm, 6, m_iFrame, 192, POSITION(0,0));
			}
		}
		++m_iFrame;
	}

	if (KEYDOWN("PixelUp"))
	{
		switch (m_iEditMode)
		{
		case AEM_BODY:
			m_tPos.y -= 1.f;
			m_pBody->SetImageOffset(m_tPos);
			break;
		case AEM_HAIR:
			tOffset = m_pHair->GetImageOffset();
			tSize = m_pHair->GetSize();
			m_pHair->SetImageOffset(tOffset.x, tOffset.y - 1.f);
			break;
		case AEM_SHIRT:
			tOffset = m_pShirt->GetImageOffset();
			tSize = m_pShirt->GetSize();
			m_pShirt->SetImageOffset(tOffset.x, tOffset.y - 1.f);
			if (tOffset.y >= 448.f)
				m_pShirt->SetImageOffset(tOffset.x, 0.f);
			break;
		}
	}

	if (KEYDOWN("PixelDown"))
	{
		switch (m_iEditMode)
		{
		case AEM_BODY:
			m_tPos.y += 1.f;
			m_pBody->SetImageOffset(m_tPos);
			break;
		case AEM_HAIR:
			tOffset = m_pHair->GetImageOffset();
			tSize = m_pHair->GetSize();
			m_pHair->SetImageOffset(tOffset.x, tOffset.y + 1.f);
			break;
		case AEM_SHIRT:
			tOffset = m_pShirt->GetImageOffset();
			tSize = m_pShirt->GetSize();
			m_pShirt->SetImageOffset(tOffset.x, tOffset.y+1.f);
			if (tOffset.y >= 448.f)
				m_pShirt->SetImageOffset(tOffset.x, 0.f);
			break;
		}
	}

	if (KEYDOWN("PixelLeft"))
	{
		switch (m_iEditMode)
		{
		case AEM_BODY:
			m_tPos.x -= 1.f;
			m_pBody->SetImageOffset(m_tPos);
			break;
		case AEM_HAIR:
			tOffset = m_pHair->GetImageOffset();
			tSize = m_pHair->GetSize();
			m_pHair->SetImageOffset(tOffset.x - 1.f, tOffset.y);
			break;
		case AEM_SHIRT:
			tOffset = m_pShirt->GetImageOffset();
			tSize = m_pShirt->GetSize();
			m_pShirt->SetImageOffset(tOffset.x - 1.f, tOffset.y);
			if (tOffset.y >= 448.f)
				m_pShirt->SetImageOffset(tOffset.x, 0.f);
			break;
		}
	}

	if (KEYDOWN("PixelRight"))
	{
		switch (m_iEditMode)
		{
		case AEM_BODY:
			m_tPos.x += 1.f;
			m_pBody->SetImageOffset(m_tPos);
			break;
		case AEM_HAIR:
			tOffset = m_pHair->GetImageOffset();
			tSize = m_pHair->GetSize();
			m_pHair->SetImageOffset(tOffset.x + 1.f, tOffset.y);
			break;
		case AEM_SHIRT:
			tOffset = m_pShirt->GetImageOffset();
			tSize = m_pShirt->GetSize();
			m_pShirt->SetImageOffset(tOffset.x + 1.f, tOffset.y);
			if (tOffset.y >= 448.f)
				m_pShirt->SetImageOffset(tOffset.x, 0.f);
			break;
		}
	}

	if (KEYDOWN("Save"))
	{
		ShowCursor(TRUE);
		DialogBox(WINDOWINSTANCE, MAKEINTRESOURCE(IDD_DIALOG1),
			WINDOWHANDLE, CAnimationEditScene::DlgProc);

		ShowCursor(FALSE);

		char	strFileName[MAX_PATH] = {};
		WideCharToMultiByte(CP_ACP, 0, m_strText, -1, strFileName,
			lstrlen(m_strText), 0, 0);
		FILE* pFile = nullptr;
		wchar_t pPath[MAX_PATH] = {};
		const wchar_t* pTexturePath = GET_SINGLE(PathManager)->FindPath(TEXTURE_PATH);
		wcscat_s((wchar_t*)pPath, MAX_PATH, pTexturePath);	//	pPath에 텍스쳐 경로를 붙인다.
		wcscat_s((wchar_t*)pPath, MAX_PATH, m_strText);	//	pPath에 파일 이름을 붙힌다.

		char strName[MAX_PATH] = {};
		WideCharToMultiByte(CP_ACP, 0, m_strText, -1, strName, MAX_PATH, nullptr, nullptr);

		switch (m_iEditMode)
		{
		case AEM_BODY:

			
			break;
		case AEM_HAIR:
			break;
		case AEM_SHIRT:
			break;
		}


		_bstr_t b(pPath);
		const char* multibytePath = b;

		fopen_s(&pFile, multibytePath, "wb");

		if (pFile)
		{
			BITMAPFILEHEADER fh = {};
			BITMAPINFOHEADER ih = {};

			ih.biSize = sizeof(BITMAPINFOHEADER);
			ih.biBitCount = 24;
			ih.biClrImportant = 0;
			ih.biClrUsed = 0;
			ih.biCompression = BI_RGB;
			ih.biHeight = 64;
			ih.biWidth = 32 * 6;
			ih.biPlanes = 1;
			ih.biSizeImage = 64 * 32 * 6 * 3;

			fh.bfType = 'B' + ('M' << 8);
			fh.bfOffBits = sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER);
			fh.bfSize = fh.bfOffBits + ih.biSizeImage;

			fwrite(&fh, sizeof(fh), 1, pFile);
			fwrite(&ih, sizeof(ih), 1, pFile);

			PIXEL tPixel[64*32*6] = {};	//	0x00bbggrr

			for (int i = 0; i < 64; ++i)
			{
				for (int j = 0; j < 32 * 6; ++j)
				{
					tPixel[(63 - i) * 32 * 6 + j].r = 
						(unsigned char)(m_vecPixel[i * 32 * 6 + j] & 0x000000ff);
					tPixel[(63 - i) * 32 * 6 + j].g = 
						(unsigned char)((m_vecPixel[i * 32 * 6 + j] & 0x0000ff00) >> 8);
					tPixel[(63 - i) * 32 * 6 + j].b = 
						(unsigned char)((m_vecPixel[i * 32 * 6 + j] & 0x00ff0000) >> 16);
				}
			}

			if (32 * 6 % 4 == 0)
			{
				for (int i = 0; i < 64; ++i)
				{
					fwrite(&tPixel[i * 32 * 6], sizeof(PIXEL), 32 * 6, pFile);
				}
			}
			else
			{
				char* empty = new char[4 - ((32 * 6) % 4)];
				for (int i = 0; i < 64; ++i)
				{
					fwrite(&tPixel[i * 32 * 6], sizeof(PIXEL), 32 * 6, pFile);
					fwrite(empty, 1, 4 - ((32 * 6) % 4), pFile);
				}
				delete[] empty;
			}


			fclose(pFile);
		}
	}

	if (KEYDOWN("Load"))
	{
		ShowCursor(TRUE);
		DialogBox(WINDOWINSTANCE, MAKEINTRESOURCE(IDD_DIALOG1),
			WINDOWHANDLE, CAnimationEditScene::DlgProc);

		ShowCursor(FALSE);

		char	strFileName[MAX_PATH] = {};
		WideCharToMultiByte(CP_ACP, 0, m_strText, -1, strFileName,
			lstrlen(m_strText), 0, 0);


		FILE* pFile = nullptr;
		wchar_t pPath[MAX_PATH] = {};
		const wchar_t* pTexturePath = GET_SINGLE(PathManager)->FindPath(TEXTURE_PATH);
		wcscat_s((wchar_t*)pPath, MAX_PATH, pTexturePath);	//	pPath에 텍스쳐 경로를 붙인다.
		wcscat_s((wchar_t*)pPath, MAX_PATH, m_strText);	//	pPath에 파일 이름을 붙힌다.

		_bstr_t b(pPath);
		const char* multibytePath = b;

		fopen_s(&pFile, multibytePath, "rb");

		if (pFile)
		{
			BITMAPFILEHEADER fh = {};
			BITMAPINFOHEADER ih = {};

			fread(&fh, sizeof(fh), 1, pFile);
			fread(&ih, sizeof(ih), 1, pFile);

			memset(&m_vecPixel[0], 0, sizeof(PIXEL) * 32 * 6 * 64);

			PIXEL tPixel[64 * 32 * 6] = {};	//	0x00bbggrr

			if (32 * 6 % 4 == 0)
			{
				for (int i = 63; i >= 0; --i)
				{
					fread(&tPixel[i * 192], sizeof(PIXEL), 192, pFile);
				}
			}
			else
			{
				char* empty = new char[4 - ((16 * 6) % 4)];
				for (int i = 63; i >= 0; --i)
				{
					fread(&tPixel[i * 32 * 6], sizeof(PIXEL), 32 * 6, pFile);
					fread(empty, 1, 4 - ((32 * 6) % 4), pFile);
				}
				delete[] empty;
			}
			COLORREF tColor = {};
			for (int i = 0; i < 64; ++i)
			{
				for (int j = 0; j < 32 * 6; ++j)
				{
					tColor = 0;
					tColor += tPixel[i * 32 * 6 + j].b;
					tColor <<= 8;
					tColor += tPixel[i * 32 * 6 + j].g;
					tColor <<= 8;
					tColor += tPixel[i * 32 * 6 + j].r;

					m_vecPixel[i * 32 * 6 + j] = tColor;
				}
			}

			fclose(pFile);
		}
	}
}

int CAnimationEditScene::Update(float fDeltaTime)
{
	if (m_iFrame >= 7)
	{
		memset(&m_vecPixel[0], 0, sizeof(COLORREF) * 32 * 64 * 6);
		m_iFrame %= 7;
	}

	return 0;
}

void CAnimationEditScene::Render(HDC hDC, float fDeltaTime)
{
	Scene::Render(hDC, fDeltaTime);

	POSITION tCamPos = GET_SINGLE(Camera)->GetPos();

	//Rectangle(hDC, m_tPos.x - 8.f, m_tPos.y - 16.f, m_tPos.x + 8.f, m_tPos.y + 16.f);
	for (int k = 0; k < 3; ++k)
	{
		MoveToEx(hDC, (int)(m_tPos.x - 17.f - tCamPos.x + k * 192.f), (int)(m_tPos.y - 33.f - tCamPos.y), NULL);	//	선택 영역을 그린다.
		LineTo(hDC, (int)(m_tPos.x + 17.f - tCamPos.x + k*192.f), (int)(m_tPos.y - 33.f - tCamPos.y));
		LineTo(hDC, (int)(m_tPos.x + 17.f - tCamPos.x + k*192.f), (int)(m_tPos.y + 33.f - tCamPos.y));
		LineTo(hDC, (int)(m_tPos.x - 17.f - tCamPos.x + k*192.f), (int)(m_tPos.y + 33.f - tCamPos.y));
		LineTo(hDC, (int)(m_tPos.x - 17.f - tCamPos.x + k*192.f), (int)(m_tPos.y - 33.f - tCamPos.y));
	}

	POSITION tPos = m_pHair->GetPos();
	tPos.y -= GETRESOLUTION.iH*0.25f - 32.f;
	tPos.y += m_pHair->GetImageOffset().y;

	MoveToEx(hDC, (int)(784.f - 17.f - tCamPos.x), (int)(-1.f - tCamPos.y+ tPos.y), NULL);	//	선택 영역을 그린다.
	LineTo(hDC, (int)(784.f + 17.f - tCamPos.x), (int)(-1.f - tCamPos.y+ tPos.y));
	LineTo(hDC, (int)(784.f + 17.f - tCamPos.x), (int)(65.f - tCamPos.y+tPos.y));
	LineTo(hDC, (int)(784.f - 17.f - tCamPos.x), (int)(65.f - tCamPos.y+tPos.y));
	LineTo(hDC, (int)(784.f - 17.f - tCamPos.x), (int)(-1.f - tCamPos.y+tPos.y));

	tPos = m_pShirt->GetPos();
	tPos.y -= GETRESOLUTION.iH * 0.25f - 32.f;
	tPos.y += m_pShirt->GetImageOffset().y;
	MoveToEx(hDC, (int)(1032.f - 9.f - tCamPos.x), (int)(- 1.f - tCamPos.y + tPos.y), NULL);	//	선택 영역을 그린다.
	LineTo(hDC, (int)(1032.f + 9.f - tCamPos.x), (int)(8.f - 9.f - tCamPos.y +tPos.y));
	LineTo(hDC, (int)(1032.f + 9.f - tCamPos.x), (int)(8.f + 9.f - tCamPos.y +tPos.y));
	LineTo(hDC, (int)(1032.f - 9.f - tCamPos.x), (int)(8.f + 9.f - tCamPos.y +tPos.y));
	LineTo(hDC, (int)(1032.f - 9.f - tCamPos.x), (int)(8.f - 9.f - tCamPos.y +tPos.y));

	MoveToEx(hDC, (int)(GETRESOLUTION.iW * 0.8), (int)(GETRESOLUTION.iH / 2.f), NULL);	//	틀을 그린다.
	LineTo(hDC, (int)(GETRESOLUTION.iW * 0.8), 0);
	LineTo(hDC, (int)GETRESOLUTION.iW- 1, 0);
	LineTo(hDC, (int)GETRESOLUTION.iW - 1, (int)(GETRESOLUTION.iH / 2.f));
	LineTo(hDC, (int)(GETRESOLUTION.iW * 0.8), (int)(GETRESOLUTION.iH / 2.f));

	size_t iSize = m_vecPixel.size();
	for (int i = 0; i < 64; ++i)
	{
		for (int j = 0; j < 32 * 6; ++j)
		{
			SetPixel(hDC, (int)(GETRESOLUTION.iW * 0.8f) + j,
					(int)(GETRESOLUTION.iH * 0.75f) + i, m_vecPixel[i*32*6 + j]);	//	아니면 픽셀을 복사한다.
		}
	}

#ifdef _DEBUG
	TCHAR strFrame[32] = {};
	swprintf_s(strFrame, TEXT("Frame: %d"), m_iFrame);
	TextOut(hDC, (int)GETRESOLUTION.iH - 50, 0, strFrame, lstrlen(strFrame));

	TextOut(hDC, 20, 620, TEXT("← ↑ → ↓: 한칸 이동"), lstrlen(TEXT("← ↑ → ↓: 한칸 이동")));
	TextOut(hDC, 20, 640, TEXT("w a s d: 화면 이동"), lstrlen(TEXT("w a s d: 화면 이동")));
	TextOut(hDC, 20, 660, TEXT("ctrl + N: 프레임 추가"), lstrlen(TEXT("ctrl + N: 프레임 추가")));
	TextOut(hDC, 20, 680, TEXT("1: 몸, 팔, 속옷 선택"), lstrlen(TEXT("1: 몸, 팔, 속옷 선택")));
	TextOut(hDC, 20, 700, TEXT("2: 머리 선택"), lstrlen(TEXT("2: 머리 선택")));
	TextOut(hDC, 20, 720, TEXT("3: 상의 선택"), lstrlen(TEXT("3: 상의 선택")));
	TextOut(hDC, 20, 740, TEXT("4: 편집기 열기"), lstrlen(TEXT("4: 편집기 열기")));
	TextOut(hDC, 20, 760, TEXT("space: 머리, 옷 방향바꾸기"), lstrlen(TEXT("space: 머리, 옷 방향바꾸기")));
	TextOut(hDC, 20, 780, TEXT("Esc: 프레임 모두 초기화"), lstrlen(TEXT("Esc: 프레임 모두 초기화")));
#endif
}

INT_PTR CAnimationEditScene::DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			memset(m_strText, 0, sizeof(wchar_t) * MAX_PATH);	//	m_strText의 문자열을 0으로 초기화한다.
			GetDlgItemText(hWnd, IDC_EDIT1, m_strText, MAX_PATH);	//	IDC_EDIT1의 텍스트를 m_strText로 받는다.
		case IDCANCEL:
			EndDialog(hWnd, IDOK);
			return TRUE;
		}
		return FALSE;
	}
	return INT_PTR();
}

INT_PTR CAnimationEditScene::DlgProc2(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	vector<COLORREF> vecPixel = {};
	HDC hDC = nullptr;
	Texture* pTex = nullptr;
	RECT tRC = { 0, 0, 80, 90 };
	TCHAR strPos[32] = {};
	HWND hSlider = nullptr;
	TCHAR str[128] = {};
	HDC hMemDC = nullptr;
	HBITMAP hBackBitmap = nullptr;
	HBITMAP hPrevBitmap = nullptr;
	TCHAR strFileName[MAX_PATH] = {};
	HWND hCheck = nullptr;
	int a = 0;

	int i = 0;
	if (hSlider)
		SetScrollRange(hSlider, 6, 0, 6, false);
	switch (message)
	{
	case WM_PAINT:
		hDC = GetDC(hWnd);
		hMemDC = CreateCompatibleDC(hDC);
		hBackBitmap = CreateCompatibleBitmap(hDC, 32, 64);
		hPrevBitmap = (HBITMAP)SelectObject(hMemDC, hBackBitmap);

		for (int i = 0; i < 64; ++i)
		{
			for (int j = 0; j < 32; ++j)
			{
				if (!m_vecvecPixel[AP_BASE].empty())
				{
					if (m_vecvecPixel[AP_BASE][m_iBasePos][i * 192 + j + m_pAniInfo->iPos / 20 * 32] == 0)
						continue;

					SetPixel(hMemDC, j, i,	//	다이알로그 화면 상의 j, i 위치에 선택된 기초의 픽셀을 찍는다.
						m_vecvecPixel[AP_BASE][m_iBasePos][i * 192 + j + m_pAniInfo->iPos / 20 * 32]);
				}
			}
		}

		for (int i = 0; i < 64; ++i)
		{
			for (int j = 0; j < 32; ++j)
			{

				if (!m_vecvecPixel[AP_UNDERWEAR].empty())
				{
					if (m_vecvecPixel[AP_UNDERWEAR][m_iUnderwearPos][i * 192 + j + m_pAniInfo->iPos / 20 * 32] == 0)
						continue;

					SetPixel(hMemDC, j, i,	//	다이알로그 화면 상의 j, i 위치에 선택된 속옷의 픽셀을 찍는다.
						m_vecvecPixel[AP_UNDERWEAR][m_iUnderwearPos][i * 192 + j + m_pAniInfo->iPos / 20 * 32]);
				}
			}
		}

		for (int i = 0; i < 64; ++i)
		{
			for (int j = 0; j < 32; ++j)
			{

				if (!m_vecvecPixel[AP_SHIRT].empty())
				{
					if (m_vecvecPixel[AP_SHIRT][m_iShirtPos][i * 192 + j + m_pAniInfo->iPos / 20 * 32] == 0)
						continue;

					SetPixel(hMemDC, j, i,	//	다이알로그 화면 상의 j, i 위치에 선택된 셔츠의 픽셀을 찍는다.
						m_vecvecPixel[AP_SHIRT][m_iShirtPos][i * 192 + j + m_pAniInfo->iPos / 20 * 32]);
				}
			}
		}

		for (int i = 0; i < 64; ++i)
		{
			for (int j = 0; j < 32; ++j)
			{

				if (!m_vecvecPixel[AP_ARM].empty())
				{
					if (m_vecvecPixel[AP_ARM][m_iArmPos][i * 192 + j + m_pAniInfo->iPos / 20 * 32] == 0)
						continue;

					SetPixel(hMemDC, j, i,	//	다이알로그 화면 상의 j, i 위치에 선택된 팔의 픽셀을 찍는다.
						m_vecvecPixel[AP_ARM][m_iArmPos][i * 192 + j + m_pAniInfo->iPos / 20 * 32]);
				}
			}
		}

		for (int i = 0; i < 64; ++i)
		{
			for (int j = 0; j < 32; ++j)
			{

				if (!m_vecvecPixel[AP_HAIR].empty())
				{
					if (m_vecvecPixel[AP_HAIR][m_iHairPos][i * 192 + j + m_pAniInfo->iPos / 20 * 32] == 0)
						continue;	//	머리카락 픽셀이 검정색일 경우 컬러키 값이므로 픽셀을 칠하지 않는다.

					SetPixel(hMemDC, j, i,	//	다이알로그 화면 상의 j, i 위치에 선택된 머리카락의 픽셀을 찍는다.
						m_vecvecPixel[AP_HAIR][m_iHairPos][i * 192 + j + m_pAniInfo->iPos / 20 * 32]);
				}
			}
		}

		swprintf_s(strPos, TEXT("Pos: %d"), m_pAniInfo->iPos);
		TextOut(hMemDC, 100, 170, strPos, lstrlen(strPos));

		BitBlt(hDC, 64, 58, 80, 90, hMemDC, 0, 0, SRCCOPY);

		SelectObject(hMemDC, hPrevBitmap);
		DeleteObject(hBackBitmap);

		DeleteDC(hMemDC);
		ReleaseDC(hWnd, hDC);

		break;
	case WM_HSCROLL:
		switch (LOWORD(wParam))
		{
		case SB_PAGEDOWN:
		case SB_PAGEUP:
			hSlider = GetDlgItem(hWnd, IDC_SLIDER1);

			m_pAniInfo->iPos = (int)SendMessage(hSlider, TBM_GETPOS, 0, 0);

			InvalidateRect(hWnd, &tRC, false);
			break;
		case SB_THUMBTRACK:
		case SB_THUMBPOSITION:
			hSlider = GetDlgItem(hWnd, IDC_SLIDER1);

			if (hSlider)
				SetScrollRange(hSlider, 6, 0, 6, false);

			m_pAniInfo->iPos = HIWORD(wParam);
			InvalidateRect(hWnd, &tRC, false);
			break;
		}
		break;
	case WM_INITDIALOG:	//	창을 띄을때 초기화하는 부분이다.		
		g_pDlg->m_hWnd = hWnd;

			if (!m_vecvecPixel[AP_BASE].empty())
			{
				size_t iSize = m_vecvecPixel[AP_BASE].size();

				for (size_t i = 0; i < iSize; ++i)
				{
					HWND hSlider = GetDlgItem(hWnd, IDC_COMBO1);

					TCHAR str[128] = {};
					swprintf_s(str, TEXT("Body: %d"), (int)i);

					SendMessage(hSlider, CB_ADDSTRING, 0, (LPARAM)str);	//	콤보박스 1번에 문자열을 추가한다.
				}
			}

			if (!m_vecvecPixel[AP_UNDERWEAR].empty())
			{
				size_t iSize = m_vecvecPixel[AP_UNDERWEAR].size();

				for (size_t i = 0; i < iSize; ++i)
				{
					HWND hSlider = GetDlgItem(hWnd, IDC_COMBO2);

					TCHAR str[128] = {};
					swprintf_s(str, TEXT("Under: %d"), (int)i);

					SendMessage(hSlider, CB_ADDSTRING, 0, (LPARAM)str);	//	콤보박스 2번에 문자열을 추가한다.
				}
			}

			if (!m_vecvecPixel[AP_ARM].empty())
			{
				size_t iSize = m_vecvecPixel[AP_ARM].size();

				for (size_t i = 0; i < iSize; ++i)
				{
					HWND hSlider = GetDlgItem(hWnd, IDC_COMBO3);

					TCHAR str[128] = {};
					swprintf_s(str, TEXT("Arm: %d"), (int)i);

					SendMessage(hSlider, CB_ADDSTRING, 0, (LPARAM)str);	//	콤보박스 3번에 문자열을 추가한다.
				}
			}

			if (!m_vecvecPixel[AP_HAIR].empty())
			{
				size_t iSize = m_vecvecPixel[AP_HAIR].size();

				for (size_t i = 0; i < iSize; ++i)
				{
					HWND hSlider = GetDlgItem(hWnd, IDC_COMBO4);

					TCHAR str[128] = {};
					swprintf_s(str, TEXT("Hair: %d"), (int)i);

					SendMessage(hSlider, CB_ADDSTRING, 0, (LPARAM)str);	//	콤보박스 4번에 문자열을 추가한다.
				}
			}

			if (!m_vecvecPixel[AP_SHIRT].empty())
			{
				size_t iSize = m_vecvecPixel[AP_SHIRT].size();

				for (size_t i = 0; i < iSize; ++i)
				{
					HWND hSlider = GetDlgItem(hWnd, IDC_COMBO5);

					TCHAR str[128] = {};
					swprintf_s(str, TEXT("Shirt: %d"), (int)i);

					SendMessage(hSlider, CB_ADDSTRING, 0, (LPARAM)str);	//	콤보박스 4번에 문자열을 추가한다.
				}
			}
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			memset(m_strText, 0, sizeof(wchar_t) * MAX_PATH);	//	m_strText의 문자열을 0으로 초기화한다.

			GetDlgItemText(hWnd, IDC_EDIT1, m_strText, MAX_PATH);	//	IDC_EDIT1의 텍스트를 m_strText로 받는다.
		case IDCANCEL:
			EndDialog(hWnd, IDOK);
			return TRUE;
		case IDC_BUTTON1:	//	기초 추가 버튼이 눌렸을 때이다.
			vecPixel.resize(192 * 64);

			hCheck = GetDlgItem(hWnd, IDC_CHECK1);

			if (SendMessage(hCheck, BM_GETCHECK, 0, 0) == BST_CHECKED)
			{
				for (int k = 0; k < 6; ++k)
				{
					for (int j = 0; j < 64; ++j)
					{
						for (int i = 0; i < 32; ++i)
						{
							memcpy(&vecPixel[j * 192 + k * 32 + 31 - i], &m_pAniInfo->vecPixel[j * 192 + k * 32 + i], sizeof(COLORREF));
						}
					}
				}
			}

			else
				memcpy(&vecPixel[0], &m_pAniInfo->vecPixel[0], 192 * 64 * sizeof(COLORREF));

			m_vecvecPixel[AP_BASE].push_back(vecPixel);

			hSlider = GetDlgItem(hWnd, IDC_COMBO1);
			swprintf_s(str, TEXT("Body: %d"), m_iBasePos);

			SendMessage(hSlider, CB_ADDSTRING, 0, (LPARAM)str);
			break;
		case IDC_BUTTON2:	//	속옷 추가 버튼이 눌렸을 때이다.
			vecPixel.resize(192 * 64);

			hCheck = GetDlgItem(hWnd, IDC_CHECK1);

			if (SendMessage(hCheck, BM_GETCHECK, 0, 0) == BST_CHECKED)
			{
				for (int k = 0; k < 6; ++k)
				{
					for (int j = 0; j < 64; ++j)
					{
						for (int i = 0; i < 32; ++i)
						{
							memcpy(&vecPixel[j * 192 + k * 32 + 31 - i], &m_pAniInfo->vecPixel[j * 192 + k * 32 + i], sizeof(COLORREF));
						}
					}
				}
			}

			else
				memcpy(&vecPixel[0], &m_pAniInfo->vecPixel[0], 192 * 64 * sizeof(COLORREF));

			m_vecvecPixel[AP_UNDERWEAR].push_back(vecPixel);

			hSlider = GetDlgItem(hWnd, IDC_COMBO2);

			swprintf_s(str, TEXT("Under: %d"), m_iUnderwearPos);

			SendMessage(hSlider, CB_ADDSTRING, 0, (LPARAM)str);
			break;
		case IDC_BUTTON3:	//	팔 추가 버튼이 눌렸을 때이다.
			vecPixel.resize(192 * 64);

			hCheck = GetDlgItem(hWnd, IDC_CHECK1);

			if (SendMessage(hCheck, BM_GETCHECK, 0, 0) == BST_CHECKED)
			{
				for (int k = 0; k < 6; ++k)
				{
					for (int j = 0; j < 64; ++j)
					{
						for (int i = 0; i < 32; ++i)
						{
							memcpy(&vecPixel[j * 192 + k * 32 + 31 - i], &m_pAniInfo->vecPixel[j * 192 + k * 32 + i], sizeof(COLORREF));
						}
					}
				}
			}

			else
				memcpy(&vecPixel[0], &m_pAniInfo->vecPixel[0], 192 * 64 * sizeof(COLORREF));

			m_vecvecPixel[AP_ARM].push_back(vecPixel);

			hSlider = GetDlgItem(hWnd, IDC_COMBO3);

			swprintf_s(str, TEXT("Arm: %d"), m_iArmPos);

			SendMessage(hSlider, CB_ADDSTRING, 0, (LPARAM)str);
			break;
		case IDC_BUTTON4:	//	머리카락 추가 버튼이 눌렸을 때이다.
			vecPixel.resize(192 * 64);

			hCheck = GetDlgItem(hWnd, IDC_CHECK1);

			if (SendMessage(hCheck, BM_GETCHECK, 0, 0) == BST_CHECKED)
			{
				for (int k = 0; k < 6; ++k)
				{
					for (int j = 0; j < 64; ++j)
					{
						for (int i = 0; i < 32; ++i)
						{
							memcpy(&vecPixel[j * 192 + k * 32 + 31 - i], &m_pAniInfo->vecPixel[j * 192 + k * 32 + i], sizeof(COLORREF));
						}
					}
				}
			}

			else
				memcpy(&vecPixel[0], &m_pAniInfo->vecPixel[0], 192 * 64 * sizeof(COLORREF));

			m_vecvecPixel[AP_HAIR].push_back(vecPixel);

			hSlider = GetDlgItem(hWnd, IDC_COMBO4);

			swprintf_s(str, TEXT("Hair: %d"), m_iHairPos);

			SendMessage(hSlider, CB_ADDSTRING, 0, (LPARAM)str);
			break;
		case IDC_BUTTON5:	//	옷 추가 버튼이 눌렸을 때이다.
			vecPixel.resize(192 * 64);

			hCheck = GetDlgItem(hWnd, IDC_CHECK1);

			if (SendMessage(hCheck, BM_GETCHECK, 0, 0) == BST_CHECKED)
			{
				for (int k = 0; k < 6; ++k)
				{
					for (int j = 0; j < 64; ++j)
					{
						for (int i = 0; i < 32; ++i)
						{
							memcpy(&vecPixel[j * 192 + k * 32 + 31 - i], &m_pAniInfo->vecPixel[j * 192 + k * 32 + i], sizeof(COLORREF));
						}
					}
				}
			}

			else
			memcpy(&vecPixel[0], &m_pAniInfo->vecPixel[0], 192 * 64 * sizeof(COLORREF));

			m_vecvecPixel[AP_SHIRT].push_back(vecPixel);

			hSlider = GetDlgItem(hWnd, IDC_COMBO5);

			swprintf_s(str, TEXT("Shirt: %d"), m_iShirtPos);

			SendMessage(hSlider, CB_ADDSTRING, 0, (LPARAM)str);
			break;
		case IDC_BUTTON_ANI_SAVE:	//	저장 버튼이 눌렸을 때이다.
			g_pDlg->Save();
			break;			
		case IDC_COMBO1:
			switch (HIWORD(wParam))
			{
			case CBN_SELCHANGE:
				hSlider = GetDlgItem(hWnd, LOWORD(wParam));

				i = (int)SendMessage(hSlider, CB_GETCURSEL, 0, 0);

				m_iBasePos = i;

				SendMessage(hSlider, CB_GETLBTEXT, i, (LPARAM)str);
				SetWindowText(hWnd, str);
				InvalidateRect(hWnd, &tRC, false);
				break;
			case CBN_EDITCHANGE:
				hSlider = GetDlgItem(hWnd, LOWORD(wParam));

				GetWindowText(hSlider, str, 128);
				SetWindowText(hWnd, str);
				break;
			}
			break;
		case IDC_COMBO2:
			switch (HIWORD(wParam))
			{
			case CBN_SELCHANGE:
				hSlider = GetDlgItem(hWnd, LOWORD(wParam));

				i = (int)SendMessage(hSlider, CB_GETCURSEL, 0, 0);
				m_iUnderwearPos = i;

				SendMessage(hSlider, CB_GETLBTEXT, i, (LPARAM)str);
				SetWindowText(hWnd, str);

				InvalidateRect(hWnd, &tRC, false);
				break;
			case CBN_EDITCHANGE:
				hSlider = GetDlgItem(hWnd, LOWORD(wParam));

				GetWindowText(hSlider, str, 128);
				SetWindowText(hWnd, str);
				break;
			}
			break;
		case IDC_COMBO3:
			switch (HIWORD(wParam))
			{
			case CBN_SELCHANGE:
				hSlider = GetDlgItem(hWnd, LOWORD(wParam));

				i = (int)SendMessage(hSlider, CB_GETCURSEL, 0, 0);
				m_iArmPos = i;

				SendMessage(hSlider, CB_GETLBTEXT, i, (LPARAM)str);
				SetWindowText(hWnd, str);

				InvalidateRect(hWnd, &tRC, false);
				break;
			case CBN_EDITCHANGE:
				hSlider = GetDlgItem(hWnd, LOWORD(wParam));

				GetWindowText(hSlider, str, 128);
				SetWindowText(hWnd, str);
				break;
			}
			break;
		case IDC_COMBO4:
			switch (HIWORD(wParam))
			{
			case CBN_SELCHANGE:
				hSlider = GetDlgItem(hWnd, LOWORD(wParam));

				i = (int)SendMessage(hSlider, CB_GETCURSEL, 0, 0);
				m_iHairPos = i;

				SendMessage(hSlider, CB_GETLBTEXT, i, (LPARAM)str);
				SetWindowText(hWnd, str);

				InvalidateRect(hWnd, &tRC, false);
				break;
			case CBN_EDITCHANGE:
				hSlider = GetDlgItem(hWnd, LOWORD(wParam));

				GetWindowText(hSlider, str, 128);
				SetWindowText(hWnd, str);
				break;
			}
			break;
		case IDC_COMBO5:
			switch (HIWORD(wParam))
			{
			case CBN_SELCHANGE:
				hSlider = GetDlgItem(hWnd, LOWORD(wParam));
				i = (int)SendMessage(hSlider, CB_GETCURSEL, 0, 0);
				m_iShirtPos = i;
				SendMessage(hSlider, CB_GETLBTEXT, i, (LPARAM)str);
				SetWindowText(hWnd, str);
				InvalidateRect(hWnd, &tRC, false);
				break;
			case CBN_EDITCHANGE:
				hSlider = GetDlgItem(hWnd, LOWORD(wParam));
				GetWindowText(hSlider, str, 128);
				SetWindowText(hWnd, str);
				break;
			}
			break;
		}
		return FALSE;
	}
	return INT_PTR();
}

void CAnimationEditScene::AnimationSave(const TCHAR* pFilePath, const vector<COLORREF>& vecCOLOR)
{
	char	strFileName[MAX_PATH] = {};

	WideCharToMultiByte(CP_ACP, 0, pFilePath, -1, strFileName,
		lstrlen(pFilePath), 0, 0);

	FILE* pFile = nullptr;

	fopen_s(&pFile, strFileName, "wb");

	if (pFile)
	{
		BITMAPFILEHEADER fh = {};
		BITMAPINFOHEADER ih = {};

		ih.biSize = sizeof(BITMAPINFOHEADER);
		ih.biBitCount = 24;
		ih.biClrImportant = 0;
		ih.biClrUsed = 0;
		ih.biCompression = BI_RGB;
		ih.biHeight = 64;
		ih.biWidth = 32 * 6;
		ih.biPlanes = 1;
		ih.biSizeImage = 64 * 32 * 6 * 3;

		fh.bfType = 'B' + ('M' << 8);
		fh.bfOffBits = sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER);
		fh.bfSize = fh.bfOffBits + ih.biSizeImage;

		fwrite(&fh, sizeof(fh), 1, pFile);
		fwrite(&ih, sizeof(ih), 1, pFile);

		PIXEL tPixel[64 * 32 * 6] = {};	//	0x00bbggrr

		for (int i = 0; i < 64; ++i)
		{
			for (int j = 0; j < 32 * 6; ++j)
			{
				tPixel[(63 - i) * 32 * 6 + j].r =
					(unsigned int)vecCOLOR[i * 32 * 6 + j] & 0x000000ff;
				tPixel[(63 - i) * 32 * 6 + j].g =
					(unsigned int)(vecCOLOR[i * 32 * 6 + j] & 0x0000ff00) >> 8;
				tPixel[(63 - i) * 32 * 6 + j].b =
					(unsigned int)(vecCOLOR[i * 32 * 6 + j] & 0x00ff0000) >> 16;
			}
		}

		if ((32 * 6) % 4 == 0)
		{
			for (int i = 0; i < 64; ++i)
			{
				fwrite(&tPixel[i * 32 * 6], sizeof(PIXEL), 32 * 6, pFile);
			}
		}
		else
		{
			char* empty = new char[4 - ((32 * 6) % 4)];
			for (int i = 0; i < 64; ++i)
			{
				fwrite(&tPixel[i * 32 * 6], sizeof(PIXEL), 32 * 6, pFile);
				fwrite(empty, 1, 4 - ((32 * 6) % 4), pFile);
			}
			delete[] empty;
		}


		fclose(pFile);
	}
}

void CAnimationEditScene::OnEditDlg()
{
	if (m_pAniEditDlg)
		m_pAniEditDlg->OnEditDlg(IDD_DIALOG2);

	m_pAniEditDlg = new CAniEditDlg;
	m_pAniEditDlg->OnEditDlg(IDD_DIALOG2);
}


void CAnimationEditScene::Save()
{
	TCHAR strFileName[MAX_PATH] = {};

	OPENFILENAME tOFN = {};

	tOFN.lStructSize = sizeof(OPENFILENAME);
	tOFN.hwndOwner = m_hWnd;
	tOFN.lpstrFile = strFileName;
	tOFN.lpstrFilter = TEXT("모든파일\0*.*\0비트맵파일\0*.bmp");
	tOFN.lpstrInitialDir = GET_SINGLE(PathManager)->FindPath(TEXTURE_PATH);
	tOFN.nMaxFile = 256;

	if (GetSaveFileName(&tOFN))
		Save(strFileName);
}

void CAnimationEditScene::Save(TCHAR* pFileName)
{
	vector<COLORREF> vecPixel = {};

	vecPixel.resize(192 * 64);
	memset(&vecPixel[0], 0, sizeof(COLORREF) * 192 * 64);

	for (int i = 0; i < 64; ++i)
	{
		for (int j = 0; j < 192; ++j)
		{
			if (!m_vecvecPixel[AP_BASE].empty())
			{
				if (m_vecvecPixel[AP_BASE][m_iBasePos][i * 192 + j] != 0)
					vecPixel[i * 192 + j] =
					m_vecvecPixel[AP_BASE][m_iBasePos][i * 192 + j];
			}

			if (!m_vecvecPixel[AP_UNDERWEAR].empty())
			{
				if (m_vecvecPixel[AP_UNDERWEAR][m_iUnderwearPos][i * 192 + j] != 0)
					vecPixel[i * 192 + j] =
					m_vecvecPixel[AP_UNDERWEAR][m_iUnderwearPos][i * 192 + j];
			}

			if (!m_vecvecPixel[AP_SHIRT].empty())
			{
				if (m_vecvecPixel[AP_SHIRT][m_iShirtPos][i * 192 + j] != 0)
					vecPixel[i * 192 + j] =
					m_vecvecPixel[AP_SHIRT][m_iShirtPos][i * 192 + j];
			}

			if (!m_vecvecPixel[AP_ARM].empty())
			{
				if (m_vecvecPixel[AP_ARM][m_iArmPos][i * 192 + j] != 0)
					vecPixel[i * 192 + j] =
					m_vecvecPixel[AP_ARM][m_iArmPos][i * 192 + j];
			}

			if (!m_vecvecPixel[AP_HAIR].empty())
			{
				if (m_vecvecPixel[AP_HAIR][m_iHairPos][i * 192 + j] != 0)
					vecPixel[i * 192 + j] =
					m_vecvecPixel[AP_HAIR][m_iHairPos][i * 192 + j];
			}
		}
	}

	AnimationSave(pFileName, vecPixel);
}