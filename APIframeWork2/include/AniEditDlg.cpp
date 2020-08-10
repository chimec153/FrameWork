#include "AniEditDlg.h"
#include "Core.h"

CAniEditDlg* g_pDlg = nullptr;

CAniEditDlg::CAniEditDlg()	:
	m_hWnd(0)
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

INT_PTR __stdcall CAniEditDlg::DlgProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_INITDIALOG:
		g_pDlg->m_hWnd = hWnd;
		break;
	case WM_COMMAND:
		switch (LOWORD(lParam))
		{
		case IDOK:

			break;
		case IDCANCEL:
			DestroyWindow(hWnd);
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
