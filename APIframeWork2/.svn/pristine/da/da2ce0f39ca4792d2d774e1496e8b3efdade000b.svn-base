#pragma once
#include "Scene.h"

class MapEditScene :
	public Scene
{
private:
	friend class SceneManager;

private:
	MapEditScene();
	~MapEditScene();

private:
	TILE_EDIT_MODE			m_eTem;
	TILE_OPTION				m_eEditOption;
	vector<class Texture*>	m_vecTileTex;
	int						m_iEditTileTex;
	class Stage*			m_pStage;
	class Obj*				m_pTileUI;
	vector<char*>			m_vecstrProto;
	class Obj*				m_pPanel;
	int						m_iPage;
	int						m_iThema;
	int						m_iEditLayer;
	class CMapEditDlg*		m_pMapEditDlg;

private:
	static TCHAR	m_strText[MAX_PATH];

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);

public:
	void LoadFile();

public:
	static INT_PTR CALLBACK DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	POSITION SetTileTexture();

public:
	void TileButton0(int iButton, float fTime);
	void TilePrevButton(float fTime);
	void TileNextButton(float fTime);
	void TileUpButton(float fTime);
	void TileDownButton(float fTime);

public:
	void MapEditDlg();
};