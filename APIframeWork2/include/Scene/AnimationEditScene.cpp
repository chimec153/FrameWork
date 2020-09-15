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
#include "../Scene/SceneManager.h"
#include "../Scene/StartScene.h"

CAnimationEditScene::CAnimationEditScene()	:
	m_pAniEditDlg(nullptr),
	m_iSelect(0)
{
}

CAnimationEditScene::~CAnimationEditScene()
{
	SAFE_DELETE(m_pAniEditDlg);
}

bool CAnimationEditScene::Init()
{
	Layer* pBack = FindLayer("Stage");

	Stage* pStage = Obj::CreateObj<Stage>("BackStage",
		pBack);
	
	pStage->SetPos(0.f, 0.f);
	
	pStage->SetSize((float)GETRESOLUTION.iW, (float)GETRESOLUTION.iH);
	pStage->SetTexture("BackStage", L"AnimationUI\\BackGround.bmp");
	
	SAFE_RELEASE(pStage);

	return true;
}

void CAnimationEditScene::Input(float fDeltaTime)
{
	Scene::Input(fDeltaTime);

	if (GetAsyncKeyState('W') & 0x8000)
	{
		HWND hWnd = GetActiveWindow();

		if(hWnd == WINDOWHANDLE)
			GET_SINGLE(Camera)->Scroll(0.f, -300.f * fDeltaTime);
	}
	if (GetAsyncKeyState('S') & 0x8000)
	{
		HWND hWnd = GetActiveWindow();

		if (hWnd == WINDOWHANDLE)
		GET_SINGLE(Camera)->Scroll(0.f, 300.f * fDeltaTime);
	}
	if (GetAsyncKeyState('A') & 0x8000)
	{
		HWND hWnd = GetActiveWindow();

		if (hWnd == WINDOWHANDLE)
		GET_SINGLE(Camera)->Scroll(-300.f * fDeltaTime, 0.f);
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		HWND hWnd = GetActiveWindow();

		if (hWnd == WINDOWHANDLE)
		GET_SINGLE(Camera)->Scroll(300.f * fDeltaTime, 0.f);
	}

	if (KEYDOWN("5"))
	{
		if (m_pAniEditDlg)
		{
			if (m_pAniEditDlg->IsDestroyed())
			{
				SAFE_DELETE(m_pAniEditDlg);

				m_pAniEditDlg = new CAniEditDlg;

				m_pAniEditDlg->OnEditDlg(IDD_DIALOG2);
			}
		}

		else
		{
			m_pAniEditDlg = new CAniEditDlg;

			m_pAniEditDlg->OnEditDlg(IDD_DIALOG2);
		}

	}

	if (KEYDOWN("Key_C"))
	{
		HWND hWnd = GetActiveWindow();

		if (hWnd == WINDOWHANDLE)
		{
			POSITION tMousePos = GET_SINGLE(Input)->GetMouseWorldPos();

			Texture* pTexture = m_pAniEditDlg->GetTexture();

			COLORREF tColor = GetPixel(pTexture->GetDC(), (int)tMousePos.x, (int)tMousePos.y);

			m_pAniEditDlg->SetColorKey(tColor);
		}
	}

	if (KEYDOWN("Key_E_CTRL"))
	{
		if(m_pAniEditDlg)
		{
			PANIMATIONCLIP pClip = m_pAniEditDlg->GetClip();

			pClip->vecFrame.clear();
		}
	}

	else if (KEYDOWN("Inventory"))
	{
		EraseFrameAll();
	}

	POSITION tPos;
	_SIZE tOffset;
	_SIZE tSize;

	if (KEYDOWN("CtrlFocusUp"))
	{
		if (m_pAniEditDlg)
		{
			POSITION tPos = m_pAniEditDlg->GetBasePos();

			m_pAniEditDlg->SetBasePos(POSITION(tPos.x, tPos.y - 1));
		}
	}

	else if (KEYDOWN("FocusUp"))	//	VK_UP
	{
		HWND hWnd = GetActiveWindow();

		if (hWnd == WINDOWHANDLE)
		{
			PANIMATIONCLIP pClip = nullptr;

			if (m_pAniEditDlg)
				pClip = m_pAniEditDlg->GetClip();

			if (pClip)
			{
				size_t iSize = pClip->vecFrame.size();

				if (iSize > m_iSelect)
				{
					--pClip->vecFrame[m_iSelect].tStart.y;
					--pClip->vecFrame[m_iSelect].tEnd.y;

					m_pAniEditDlg->SetPos(pClip->vecFrame[m_iSelect].tStart);
				}
			}
		}
	}

	if (KEYDOWN("CtrlFocusDown"))
	{
		if (m_pAniEditDlg)
		{
			POSITION tPos = m_pAniEditDlg->GetBasePos();

			m_pAniEditDlg->SetBasePos(POSITION(tPos.x, tPos.y + 1));
		}
	}

	else if (KEYDOWN("FocusDown"))	//	VK_DOWN
	{
		HWND hWnd = GetActiveWindow();

		if (hWnd == WINDOWHANDLE)
		{
			PANIMATIONCLIP pClip = nullptr;

			if (m_pAniEditDlg)
				pClip = m_pAniEditDlg->GetClip();

			if (pClip)
			{
				size_t iSize = pClip->vecFrame.size();

				if (iSize > m_iSelect)
				{
					++pClip->vecFrame[m_iSelect].tStart.y;
					++pClip->vecFrame[m_iSelect].tEnd.y;

					m_pAniEditDlg->SetPos(pClip->vecFrame[m_iSelect].tStart);
				}
			}
		}
	}

	if (KEYDOWN("CtrlFocusLeft"))
	{
		if (m_pAniEditDlg)
		{
			POSITION tPos = m_pAniEditDlg->GetBasePos();

			m_pAniEditDlg->SetBasePos(POSITION(tPos.x - 1, tPos.y));
		}
	}

	else if (KEYDOWN("FocusLeft"))
	{
		HWND hWnd = GetActiveWindow();

		if (hWnd == WINDOWHANDLE)
		{
			PANIMATIONCLIP pClip = nullptr;

			if (m_pAniEditDlg)
				pClip = m_pAniEditDlg->GetClip();

			if (pClip)
			{
				size_t iSize = pClip->vecFrame.size();

				if (iSize > m_iSelect)
				{
					--pClip->vecFrame[m_iSelect].tStart.x;
					--pClip->vecFrame[m_iSelect].tEnd.x;

					m_pAniEditDlg->SetPos(pClip->vecFrame[m_iSelect].tStart);
				}
			}
		}
	}

		if (KEYDOWN("CtrlFocusRight"))
		{
			if (m_pAniEditDlg)
			{
				POSITION tPos = m_pAniEditDlg->GetBasePos();

				m_pAniEditDlg->SetBasePos(POSITION(tPos.x + 1, tPos.y));
			}
		}

		else if (KEYDOWN("FocusRight"))
		{

		HWND hWnd = GetActiveWindow();

		if (hWnd == WINDOWHANDLE)
		{
			PANIMATIONCLIP pClip = nullptr;

			if (m_pAniEditDlg)
				pClip = m_pAniEditDlg->GetClip();

			if (pClip)
			{
				size_t iSize = pClip->vecFrame.size();

				if (iSize > m_iSelect)
				{
					++pClip->vecFrame[m_iSelect].tStart.x;
					++pClip->vecFrame[m_iSelect].tEnd.x;

					m_pAniEditDlg->SetPos(pClip->vecFrame[m_iSelect].tStart);
				}
			}
		}
	}

	if (KEYDOWN("CtrlPixelUp"))
	{
		if (m_pAniEditDlg)
		{
			POSITION tBasePos = m_pAniEditDlg->GetBasePos();

			float fSizeY = m_pAniEditDlg->GetSize().y;

			tBasePos.y -= 64;

			m_pAniEditDlg->SetBasePos(tBasePos);
		}
	}

	else if (KEYDOWN("PixelUp"))
	{
		HWND hWnd = GetActiveWindow();

		if (hWnd == WINDOWHANDLE)
		{
			PANIMATIONCLIP pClip = nullptr;

			if (m_pAniEditDlg)
				pClip = m_pAniEditDlg->GetClip();

			if (pClip)
			{
				size_t iSize = pClip->vecFrame.size();

				if (iSize > m_iSelect)
				{
					float fSizeY = m_pAniEditDlg->GetSize().y;

					pClip->vecFrame[m_iSelect].tStart.y-= fSizeY;
					pClip->vecFrame[m_iSelect].tEnd.y-= fSizeY;

					m_pAniEditDlg->SetPos(pClip->vecFrame[m_iSelect].tStart);
				}
			}
		}
	}

	if (KEYDOWN("CtrlPixelDown"))
	{
		if (m_pAniEditDlg)
		{
			POSITION tBasePos = m_pAniEditDlg->GetBasePos();

			float fSizeY = m_pAniEditDlg->GetSize().y;

			tBasePos.y += 64;

			m_pAniEditDlg->SetBasePos(tBasePos);
		}
	}

	else if (KEYDOWN("PixelDown"))
	{
		HWND hWnd = GetActiveWindow();

		if (hWnd == WINDOWHANDLE)
		{
			PANIMATIONCLIP pClip = nullptr;

			if (m_pAniEditDlg)
				pClip = m_pAniEditDlg->GetClip();

			if (pClip)
			{
				size_t iSize = pClip->vecFrame.size();

				if (iSize > m_iSelect)
				{
					float fSizeY = m_pAniEditDlg->GetSize().y;

					pClip->vecFrame[m_iSelect].tStart.y += fSizeY;
					pClip->vecFrame[m_iSelect].tEnd.y += fSizeY;

					m_pAniEditDlg->SetPos(pClip->vecFrame[m_iSelect].tStart);
				}
			}
		}

	}

	if (KEYDOWN("CtrlPixelLeft"))
	{
		if (m_pAniEditDlg)
		{
			POSITION tBasePos = m_pAniEditDlg->GetBasePos();

			float fSizeX = m_pAniEditDlg->GetSize().x;

			tBasePos.x -= 32;

			m_pAniEditDlg->SetBasePos(tBasePos);
		}
	}

	else if (KEYDOWN("PixelLeft"))
	{
		HWND hWnd = GetActiveWindow();

		if (hWnd == WINDOWHANDLE)
		{
			PANIMATIONCLIP pClip = nullptr;

			if (m_pAniEditDlg)
				pClip = m_pAniEditDlg->GetClip();

			if (pClip)
			{
				size_t iSize = pClip->vecFrame.size();

				if (iSize > m_iSelect)
				{
					float fSizeX = m_pAniEditDlg->GetSize().x;

					pClip->vecFrame[m_iSelect].tStart.x -= fSizeX;
					pClip->vecFrame[m_iSelect].tEnd.x -= fSizeX;

					m_pAniEditDlg->SetPos(pClip->vecFrame[m_iSelect].tStart);
				}
			}
		}

	}

	if (KEYDOWN("CtrlPixelRight"))
	{
		if (m_pAniEditDlg)
		{
			POSITION tBasePos = m_pAniEditDlg->GetBasePos();

			float fSizeX = m_pAniEditDlg->GetSize().x;

			tBasePos.x += 32;

			m_pAniEditDlg->SetBasePos(tBasePos);
		}
	}

	else if (KEYDOWN("PixelRight"))
	{
		HWND hWnd = GetActiveWindow();

		if (hWnd == WINDOWHANDLE)
		{
			PANIMATIONCLIP pClip = nullptr;

			if (m_pAniEditDlg)
				pClip = m_pAniEditDlg->GetClip();

			if (pClip)
			{
				size_t iSize = pClip->vecFrame.size();

				if (iSize > m_iSelect)
				{
					float fSizeX = m_pAniEditDlg->GetSize().x;

					pClip->vecFrame[m_iSelect].tStart.x += fSizeX;
					pClip->vecFrame[m_iSelect].tEnd.x += fSizeX;

					m_pAniEditDlg->SetPos(pClip->vecFrame[m_iSelect].tStart);
				}
			}
		}

	}


	if (KEYDOWN("PrevScene"))
	{
		GET_SINGLE(SceneManager)->CreateScene<StartScene>("Start", SC_NEXT);

		GET_SINGLE(Camera)->SetPos(0.f, 0.f);
/*
		if(m_pAniEditDlg)
			m_pAniEditDlg->Clear();*/
	}

	if (KEYPRESS("CtrlLButton"))
	{
		POSITION tPos = GET_SINGLE(Input)->GetMouseWorldPos();

		m_pAniEditDlg->SetBasePos(tPos);
	}

	else if (KEYDOWN("MouseLButton"))
	{
		HWND hActive = GetActiveWindow();

		if (hActive == WINDOWHANDLE)
		{
			m_tStart = GET_SINGLE(Input)->GetMouseWorldPos();

			RESOLUTION tRS = GET_SINGLE(Camera)->GetClientRS();

			POSITION tMouseClientPos = GET_SINGLE(Input)->GetMouseClientPos();

			if (tRS.iW >= tMouseClientPos.x && tRS.iH >= tMouseClientPos.y &&
				tMouseClientPos.x >=0.f && tMouseClientPos.y >=0.f)
			{
				if (SelectFrame(m_tStart))
				{
					PANIMATIONCLIP pClip = m_pAniEditDlg->GetClip();

					POSITION tPos = pClip->vecFrame[m_iSelect].tStart;

					m_tMouseClickPos = m_tStart - tPos;
				}

				else
				{
					m_tMouseClickPos = 0.f;

					m_iSelect = -1;
				}
			}
		}
	}

	else if (KEYPRESS("MouseLButton"))
	{
		POSITION tMousePos = GET_SINGLE(Input)->GetMouseWorldPos();

		RESOLUTION tRS = GET_SINGLE(Camera)->GetClientRS();

		POSITION tMouseClientPos = GET_SINGLE(Input)->GetMouseClientPos();

		if (tRS.iW >= tMouseClientPos.x && tRS.iH >= tMouseClientPos.y &&
			tMouseClientPos.x >= 0.f && tMouseClientPos.y >= 0.f)
		{
			PANIMATIONCLIP pClip = m_pAniEditDlg->GetClip();

			if (pClip)
			{
				size_t iSize = pClip->vecFrame.size();

				if (iSize > m_iSelect && m_iSelect != -1)
				{
					float fWidth = pClip->vecFrame[m_iSelect].tEnd.x - pClip->vecFrame[m_iSelect].tStart.x;
					float fHeight = pClip->vecFrame[m_iSelect].tEnd.y - pClip->vecFrame[m_iSelect].tStart.y;

					pClip->vecFrame[m_iSelect].tStart = tMousePos - m_tMouseClickPos;
					pClip->vecFrame[m_iSelect].tEnd = pClip->vecFrame[m_iSelect].tStart + POSITION(fWidth, fHeight);

					m_pAniEditDlg->SetPos(tMousePos - m_tMouseClickPos);
				}
			}
		}
	}

	else if (KEYUP("MouseLButton"))	//	마우스 왼쪽 버튼을 땔 때다.
	{
		HWND hActive = GetActiveWindow();

		if (hActive == WINDOWHANDLE)	//	현재 윈도우가 활성화된 경우다.
		{
			RESOLUTION tRS = GET_SINGLE(Camera)->GetClientRS();

			POSITION tMouseClientPos = GET_SINGLE(Input)->GetMouseClientPos();

			if (tRS.iW >=tMouseClientPos.x && tRS.iH >= tMouseClientPos.y)
			{
				POSITION tSize = m_pAniEditDlg->GetSize();

				m_tEnd = GET_SINGLE(Input)->GetMouseWorldPos();

				if (!IsInFrame(m_tEnd))	//	프레임이 있는 위치가 아닐 경우다.
				{
					if (tSize.x != 0.f && tSize.y != 0.f)	//	사이즈 설정에 값이 하나라도 있을 경우다.
					{
						m_tStart = m_tEnd;	//	현재 위치를 시작위치로 하고

						m_tEnd = m_tStart + tSize;	//	끝 위치를 시작위치 + 크기 만큼으로 정한다.

						m_pAniEditDlg->AddFrame(m_tStart, m_tEnd);	//	현재 클립에 프레임을 추가한다.
					}

					else
					{

						if (m_tStart.x > m_tEnd.x)
						{
							float fTemp = m_tStart.x;

							m_tStart.x = m_tEnd.x;

							m_tEnd.x = fTemp;
						}

						if (m_tStart.y > m_tEnd.y)
						{
							float fTemp = m_tStart.y;

							m_tStart.y = m_tEnd.y;

							m_tEnd.y = fTemp;
						}

						m_pAniEditDlg->AddFrame(m_tStart, m_tEnd);
					}
				}
			}
		}
	}

	else if (KEYDOWN("MouseRButton"))
	{
		HWND hActive = GetActiveWindow();

		if (hActive == WINDOWHANDLE)
			EraseFrame();
	}
}

int CAnimationEditScene::Update(float fDeltaTime)
{
	if (m_pAniEditDlg)
		m_pAniEditDlg->Update(fDeltaTime);

	return 0;
}

void CAnimationEditScene::Render(HDC hDC, float fDeltaTime)
{
	if (m_pAniEditDlg)
		m_pAniEditDlg->Render();

	Scene::Render(hDC, fDeltaTime);

	POSITION tCamPos = GET_SINGLE(Camera)->GetPos();

	Texture* pTexture = nullptr;

	if(m_pAniEditDlg)
		pTexture = m_pAniEditDlg->GetTexture();

	if (pTexture)
	{
		long lWidth = pTexture->GetWidth();
		long lHeight = pTexture->GetHeight();

		TransparentBlt(hDC, 0, 0, lWidth - (int)tCamPos.x, lHeight - (int)tCamPos.y,
			pTexture->GetDC(), (int)tCamPos.x, (int)tCamPos.y, lWidth - (int)tCamPos.x, lHeight - (int)tCamPos.y, SRCCOPY);
	}

	PANIMATIONCLIP pClip = nullptr;

	if (m_pAniEditDlg)
		pClip = m_pAniEditDlg->GetClip();

	size_t iFrameSize = 0;

	if(pClip)
		iFrameSize = pClip->vecFrame.size();

	POSITION tMousePos = GET_SINGLE(Input)->GetMouseWorldPos();

	for (size_t i = 0; i < iFrameSize; ++i)
	{
		RECT tRC = { (int)(pClip->vecFrame[iFrameSize - i - 1].tStart.x  - tCamPos.x),
			(int)(pClip->vecFrame[iFrameSize - i - 1].tStart.y - tCamPos.y),
			(int)(pClip->vecFrame[iFrameSize - i - 1].tEnd.x - tCamPos.x),
			(int)(pClip->vecFrame[iFrameSize - i - 1].tEnd.y - tCamPos.y) };

		if (m_iSelect == iFrameSize - i - 1)
			FrameRect(hDC, &tRC, GREENBRUSH);

		else if (tRC.left > tMousePos.x - tCamPos.x ||
			tRC.top > tMousePos.y - tCamPos.y ||
			tRC.right < tMousePos.x - tCamPos.x ||
			tRC.bottom < tMousePos.y - tCamPos.y)
			FrameRect(hDC, &tRC, BLACKBRUSH);

		else
			FrameRect(hDC, &tRC, BLUEBRUSH);
	}

#ifdef _DEBUG
	if (m_pAniEditDlg)
	{
		POSITION tBasePos = m_pAniEditDlg->GetBasePos();

		RECT tBaseRC = { (int)(tBasePos.x - tCamPos.x), (int)(tBasePos.y - tCamPos.y), 
			(int)(tBasePos.x + 32 - tCamPos.x), (int)(tBasePos.y + 64 - tCamPos.y)};

		FrameRect(hDC, &tBaseRC, BLACKBRUSH);
	}
#endif

	TCHAR strSize[32] = {};

	swprintf_s(strSize, TEXT("Size: %d"), (int)iFrameSize);
	TextOut(hDC, 0, GETRESOLUTION.iW/2, strSize, lstrlen(strSize));
}

void CAnimationEditScene::OnEditDlg()
{
	if (m_pAniEditDlg)
		m_pAniEditDlg->OnEditDlg(IDD_DIALOG2);

	m_pAniEditDlg = new CAniEditDlg;
	m_pAniEditDlg->OnEditDlg(IDD_DIALOG2);
}

void CAnimationEditScene::EraseFrame()
{
	POSITION tMousePos = GET_SINGLE(Input)->GetMouseWorldPos();

	PANIMATIONCLIP pClip = m_pAniEditDlg->GetClip();

	size_t iSize = pClip->vecFrame.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		if (pClip->vecFrame[iSize - i - 1].tStart.x > tMousePos.x ||
			pClip->vecFrame[iSize - i - 1].tStart.y > tMousePos.y ||
			pClip->vecFrame[iSize - i - 1].tEnd.x < tMousePos.x ||
			pClip->vecFrame[iSize - i - 1].tEnd.y < tMousePos.y)
			continue;

		auto iter = pClip->vecFrame.begin() + iSize - i - 1;

		pClip->vecFrame.erase(iter);

		--pClip->iFrameMaxX;
		--pClip->iLengthX;

		return;
	}
}

void CAnimationEditScene::EraseFrameAll()
{
	POSITION tMousePos = GET_SINGLE(Input)->GetMouseWorldPos();

	if (m_pAniEditDlg)
	{
		PANIMATIONCLIP pClip = m_pAniEditDlg->GetClip();

		if (pClip)
		{
			vector<FrameInfo>::iterator iter = pClip->vecFrame.begin();
			vector<FrameInfo>::iterator iterEnd = pClip->vecFrame.end();

			for (; iter != iterEnd;)
			{
				if ((*iter).tStart.x > tMousePos.x ||
					(*iter).tStart.y > tMousePos.y ||
					(*iter).tEnd.x < tMousePos.x ||
					(*iter).tEnd.y < tMousePos.y)
				{
					++iter;
					continue;
				}

				iter = pClip->vecFrame.erase(iter);
				iterEnd = pClip->vecFrame.end();

				pClip->iFrameMaxX = 0;
				pClip->iLengthX = 0;
			}
		}
	}
}

bool CAnimationEditScene::SelectFrame(const POSITION& tPos)
{
	PANIMATIONCLIP pClip = m_pAniEditDlg->GetClip();

	size_t iSize = pClip->vecFrame.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		if (pClip->vecFrame[iSize - i - 1].tStart.x > tPos.x ||
			pClip->vecFrame[iSize - i - 1].tStart.y > tPos.y ||
			pClip->vecFrame[iSize - i - 1].tEnd.x < tPos.x ||
			pClip->vecFrame[iSize - i - 1].tEnd.y < tPos.y)
			continue;

		m_iSelect = (int)(iSize - i - 1);

		if (m_pAniEditDlg)
			m_pAniEditDlg->SetSelect(m_iSelect);

		PANIMATIONCLIP pClip = m_pAniEditDlg->GetClip();

		size_t iSize = pClip->vecFrame.size();

		if (iSize > m_iSelect)
			m_pAniEditDlg->SetPos(pClip->vecFrame[m_iSelect].tStart);

		return true;
	}

	return false;
}

bool CAnimationEditScene::IsInFrame(const POSITION& tPos)
{
	PANIMATIONCLIP pClip = m_pAniEditDlg->GetClip();

	size_t iSize = pClip->vecFrame.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		if (pClip->vecFrame[iSize - i - 1].tStart.x > tPos.x ||
			pClip->vecFrame[iSize - i - 1].tStart.y > tPos.y ||
			pClip->vecFrame[iSize - i - 1].tEnd.x < tPos.x ||
			pClip->vecFrame[iSize - i - 1].tEnd.y < tPos.y)
			continue;

		return true;
	}

	return false;
}
