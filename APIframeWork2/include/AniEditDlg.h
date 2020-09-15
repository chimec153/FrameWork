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
	vector<COLORREF>		m_vecPixel;
	class Texture*			m_pTexture;
	PANIMATIONCLIP			m_pClip;
	HWND					m_hComboOption;
	TCHAR*					m_strFileName;
	char*					m_strName;
	HDC						m_hDC;
	float					m_fFrameTime;
	POSITION				m_tSize;
	HWND					m_hListClip;
	PANIMATIONCLIP			m_pDefaultClip;
	int						m_iSelect;
	POSITION				m_tBasePos;
	bool					m_bDestroyed;

public:
	class Texture* GetTexture()	const
	{
		return m_pTexture;
	}

	PANIMATIONCLIP GetClip()	const
	{
		return m_pClip;
	}

	POSITION GetSize()	const
	{
		return m_tSize;
	}

	void SetPos(const POSITION& tPos)
	{
		TCHAR strPos[32] = {};

		swprintf_s(strPos, TEXT("%d"), (int)tPos.x);

		SetDlgItemText(m_hWnd, IDC_EDIT_POS_X, strPos);

		TCHAR strPosY[32] = {};

		swprintf_s(strPosY, TEXT("%d"), (int)tPos.y);

		SetDlgItemText(m_hWnd, IDC_EDIT_POS_Y, strPosY);
	}

	void SetBasePos(const POSITION& tPos)
	{
		m_tBasePos = tPos;
	}

	POSITION GetBasePos()	const
	{
		return m_tBasePos;
	}

	void SetColorKey(const COLORREF& tColor);
	void ChangeColorKey();

	POSITION GetFramePos()	const
	{
		BOOL bTrans = false;

		int iPosX = GetDlgItemInt(m_hWnd, IDC_EDIT_POS_X, &bTrans, true);

		int iPosY = GetDlgItemInt(m_hWnd, IDC_EDIT_POS_Y, &bTrans, true);

		return POSITION((float)iPosX, (float)iPosY);
	}

	void SetSelect(int iSelect)
	{
		m_iSelect = iSelect;		

		SetDlgItemInt(m_hWnd, IDC_EDIT_RENDER_X, (int)m_pClip->vecRenderPos[m_iSelect].x, true);
		SetDlgItemInt(m_hWnd, IDC_EDIT_RENDER_Y, (int)m_pClip->vecRenderPos[m_iSelect].y, true);
	}

	bool IsDestroyed()	const
	{
		return m_bDestroyed;
	}

public:
	void OnEditDlg(int iID);
	void Clear();
	void Save();
	void Load();
	void LoadTexture();
	void AddFrame(POSITION tStart, POSITION tEnd);
	void AddClip();
	void DeleteClip();
	void ModifyClip();
	void Update(float fTime);
	void Render();
	void ChangeCurrentClip();
	void SetFramePos();
	void ChangeRenderPos();
	void ChangeOption();
	void ChangeOptionFrame();

public:
	void Save(const TCHAR* pFileName);
	void Load(const TCHAR* pFileName);
	void LoadTexture(const TCHAR* pFileName);

private:
	static INT_PTR WINAPI DlgProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
};

