#pragma once

#include "game.h"

class CMapEditDlg
{
public:
	CMapEditDlg();
	~CMapEditDlg();

private:
	int					m_iID;
	HWND				m_hWnd;
	class Stage*		m_pStage;
	HWND				m_hTypeCombo;
	HWND				m_hBrushCombo;
	HWND				m_hTextureList;
	class Texture*		m_pTexture;
	class Texture*		m_pBackBuffer;
	POSITION			m_tImageOffSet;
	int					m_iHPos;
	int					m_iVPos;
	HWND				m_hHScroll;
	HWND				m_hVScroll;
	POSITION			m_tPictureOffset;
	HWND				m_hPicture;
	HDC					m_hBackDC;
	HBITMAP				m_hBackBitmap;
	HBITMAP				m_hPrevBitmap;
	bool				m_bPictureClick;
	POSITION			m_tTileStart;
	POSITION			m_tTileEnd;

public:
	int GetType()	const
	{
		return (int)SendMessage(m_hTypeCombo, CB_GETCURSEL, 0, 0);
	}

	int GetBrush()	const
	{
		return (int)SendMessage(m_hBrushCombo, CB_GETCURSEL, 0, 0);
	}

	class Texture* GetTexture()	const
	{
		return m_pTexture;
	}

	void SetTexture(class Texture* pTexture);

	POSITION GetStartPos()	const
	{
		BOOL bTrans = false;

		POSITION tStart = {};

		tStart.x = (float)GetDlgItemInt(m_hWnd, IDC_EDIT_START_X, &bTrans, true);
		tStart.y = (float)GetDlgItemInt(m_hWnd, IDC_EDIT_START_Y, &bTrans, true);

		return tStart;
	}

	POSITION GetEndPos()	const
	{
		BOOL bTrans = false;

		POSITION tEnd = {};

		tEnd.x = (float)GetDlgItemInt(m_hWnd, IDC_EDIT_END_X, &bTrans, true);
		tEnd.y = (float)GetDlgItemInt(m_hWnd, IDC_EDIT_END_Y, &bTrans, true);

		return tEnd;
	}

	POSITION GetTotalOffset()	const
	{
		return m_tImageOffSet + m_tPictureOffset;
	}

	bool IsPictureClicked()	const
	{
		return m_bPictureClick;
	}

	POSITION GetTileStart()	const
	{
		return m_tTileStart;
	}

	POSITION GetTileEnd()	const
	{
		return m_tTileEnd;
	}

public:
	void OnDialog(int iID);
	void CreateTileMap();
	void SetStage(class Stage* pStage);
	void Save();
	void Load();
	void Clear();
	void LoadTexture();
	void SelectTexture();
	void Render(HDC hDC, float fTime);
	void SetImagePos(int iPos);
	void SetImageVPos(int iPos);
	void SetWindowOffSet();
	void SetTextureColorKey();
	void SetTileEnd();
	void SetPictureClick(bool bClick)
	{
		m_bPictureClick = bClick;
	}

public:
	void Save(TCHAR* pFile);
	void Load(TCHAR* pFile);
	void LoadTexture(TCHAR* pFile);

private:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
};

