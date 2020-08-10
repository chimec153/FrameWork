#pragma once

#include "game.h"

class CAniEditDlg
{
	friend class CAnimationEditScene;
private:
	CAniEditDlg();
	~CAniEditDlg();

private:
	HWND					m_hWnd;
	class Animation*		m_pAnimation;

public:
	void OnEditDlg(int iID);
	void Clear();

private:
	static INT_PTR WINAPI DlgProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
};

