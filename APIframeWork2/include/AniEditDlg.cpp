#include "AniEditDlg.h"
#include "Core.h"
#include "Core/PathManager.h"
#include "Scene/AnimationEditScene.h"

CAniEditDlg* g_pDlg = nullptr;

CAniEditDlg::CAniEditDlg()	:
	m_hWnd(0),
	m_pAnimation(nullptr)
{
	g_pDlg = this;
}

CAniEditDlg::~CAniEditDlg()
{
}

void CAniEditDlg::OnEditDlg(int iID)
{
	m_hWnd = CreateDialog(WINDOWINSTANCE, MAKEINTRESOURCE(iID), WINDOWHANDLE, CAniEditDlg::DlgProc);
	ShowWindow(m_hWnd, SW_SHOW);

	ShowCursor(true);
}

void CAniEditDlg::Clear()
{
	DestroyWindow(m_hWnd);

	g_pDlg = 0;

	ShowCursor(false);
}

void CAniEditDlg::Save()
{
}

void CAniEditDlg::Load()
{
}

void CAniEditDlg::Save(TCHAR* pFileName)
{
}


INT_PTR __stdcall CAniEditDlg::DlgProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_INITDIALOG:
		g_pDlg->m_hWnd = hWnd;
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:

			break;
		case IDCANCEL:
			DestroyWindow(hWnd);
			break;
		case IDC_BUTTON_ANI_SAVE:
			g_pDlg->Save();
			break;
		}
		break;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return INT_PTR();
}
