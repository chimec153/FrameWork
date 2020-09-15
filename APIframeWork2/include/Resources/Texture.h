#pragma once
#include "../Ref.h"

typedef struct _tagTextureInfo
{
	HDC					hDC;
	HBITMAP				hBmp;
	HBITMAP				hPrevBmp;
	BITMAP				BmpInfo;
	unsigned int		iColorKey;
	bool				bColorKey;

	_tagTextureInfo()
	{
		hDC = 0;
		hBmp = 0;
		hPrevBmp = 0;
		memset(&BmpInfo, 0, sizeof(BITMAP));
		iColorKey = 0;
		bColorKey = false;
	}

	~_tagTextureInfo()
	{
		SelectObject(hDC, hPrevBmp);
		DeleteObject(hBmp);
		DeleteDC(hDC);
	}
}TextureInfo, *PTextureInfo;

class Texture :
	public Ref
{
private:
	friend class ResourcesManager;
public:
	Texture();
	~Texture();

private:
	vector<PTextureInfo>		m_vecTexture;
	HDC							m_hMemDC;
	HBITMAP						m_hBitmap;
	HBITMAP						m_hOldBitmap;
	BITMAP						m_tInfo;
	COLORREF					m_ColorKey;
	bool						m_bColorKeyEnable;
	wstring						m_strFileName;
	string						m_strKey;
	string						m_strPathKey;

public:
	void SetColorKey(unsigned char r, unsigned char g,
		unsigned char b, int iIndex = 0);
	void SetColorKey(COLORREF colorKey, int iIndex = 0);
	void SetColorKeyAll(unsigned char r, unsigned char g, unsigned char b);
	void SetColorKeyAll(COLORREF colorKey);

	COLORREF GetColorKey()	const
	{
		return m_ColorKey;
	}
	bool GetColorKeyEnable() const
	{
		return m_bColorKeyEnable;
	}
	
	long GetWidth() const
	{
		return m_tInfo.bmWidth;
	}

	long GetHeight() const
	{
		return m_tInfo.bmHeight;
	}

	HDC GetDC() const
	{
		return m_hMemDC;
	}

	HDC GetTextureDC(int iIndex = 0)	const
	{
		return m_vecTexture[iIndex]->hDC;
	}
	
public:
	bool LoadTexture(HINSTANCE hInst, HDC hDC,
		const string& strKey, const TCHAR* pFileName,
		const string& strPathKey = TEXTURE_PATH);
	bool LoadTexture(HINSTANCE hInst, HDC hDC,
		const string& strKey, const vector<const TCHAR*>& vecFileName,
		const string& strPathKey = TEXTURE_PATH);
	bool LoadTexture(HINSTANCE hInst, HDC hDC,
		const string& strKey, const TCHAR* pFileName, 
		int iCount,	const string& strPathKey = TEXTURE_PATH);
	bool LoadTextureFromFullPath(HINSTANCE hInst, HDC hDC,
		const string& strKey, const wchar_t* pFileName);
	void Render(HDC hDC, POSITION tPos, POSITION tImagePos, POSITION tSize, int iIndex = 0);
	void RenderByAlpha(unsigned char cAlpha, HDC hDC, POSITION tPos, POSITION tImagePos, POSITION tSize, int iIndex = 0);

	void SaveFromPath(const char* pFileName, const string& strPathKey = DATA_PATH);
	void Save(FILE* pFile);
	void LoadFromPath(const char* pFileName, const string& strPathKey = DATA_PATH);
	void Load(FILE* pFile);
};
