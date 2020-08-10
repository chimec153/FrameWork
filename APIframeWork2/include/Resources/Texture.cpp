#include "Texture.h"
#include "../Core/PathManager.h"
#include "ResourcesManager.h"

Texture::Texture()	:
	m_hMemDC(NULL),
	m_bColorKeyEnable(false),
	m_ColorKey(RGB(255,0,255))
{
}

Texture::~Texture()
{
	Safe_Delete_VecList(m_vecTexture);

	SelectObject(m_hMemDC, m_hOldBitmap);

	DeleteObject(m_hBitmap);

	DeleteDC(m_hMemDC);
}

void Texture::SetColorKeyAll(unsigned char r, unsigned char g, unsigned char b)
{
	size_t iSize = m_vecTexture.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecTexture[i]->iColorKey = RGB(r, g, b);
		m_vecTexture[i]->bColorKey = true;
	}
}

void Texture::SetColorKeyAll(COLORREF colorKey)
{
	size_t iSize = m_vecTexture.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecTexture[i]->iColorKey = colorKey;
		m_vecTexture[i]->bColorKey = true;
	}
}

void Texture::SetColorKey(unsigned char r, unsigned char g, unsigned char b, int iIndex)
{
	m_ColorKey = RGB(r, g, b);
	m_bColorKeyEnable = true;
}

void Texture::SetColorKey(COLORREF colorKey, int iIndex)
{
	m_ColorKey = colorKey;
	m_bColorKeyEnable = true;
}

bool Texture::LoadTexture(HINSTANCE hInst, HDC hDC,
	const string & strKey, const TCHAR * pFileName, const string & strPathKey)
{
	m_strFileName = pFileName;
	m_strKey = strKey;
	m_strPathKey = strPathKey;
	m_hMemDC = CreateCompatibleDC(hDC);

	const wchar_t* pPath = GET_SINGLE(PathManager)->FindPath(strPathKey);

	wstring strPath;

	if (pPath)
		strPath = pPath;

	strPath += pFileName;

	m_hBitmap = (HBITMAP)LoadImage(hInst, strPath.c_str(),
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	m_hOldBitmap = (HBITMAP)SelectObject(m_hMemDC,
		m_hBitmap);

	GetObject(m_hBitmap, sizeof(m_tInfo), &m_tInfo);

	return true;
}

bool Texture::LoadTexture(HINSTANCE hInst, HDC hDC, const string& strKey, const vector<const TCHAR*>& vecFileName, const string& strPathKey)
{
	SetTag(strKey);

	m_strKey = strKey;
	m_strFileName = vecFileName[0];
	m_strPathKey = strPathKey;

	size_t iSize = vecFileName.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		TCHAR strFullPath[MAX_PATH] = {};

		const TCHAR* pRootPath = GET_SINGLE(PathManager)->FindPath(strPathKey);

		if (pRootPath)
			lstrcat(strFullPath, pRootPath);

		lstrcat(strFullPath, vecFileName[i]);

		PTextureInfo pInfo = new TextureInfo;

		pInfo->hDC = CreateCompatibleDC(hDC);
		pInfo->hBmp = (HBITMAP)LoadImage(hInst, strFullPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		pInfo->hPrevBmp = (HBITMAP)SelectObject(pInfo->hDC, pInfo->hBmp);

		GetObject(pInfo->hBmp, sizeof(BITMAP), &pInfo->BmpInfo);

		m_vecTexture.push_back(pInfo);
	}

	return true;
}

bool Texture::LoadTexture(HINSTANCE hInst, HDC hDC, const string& strKey, const TCHAR* pFileName, int iCount, const string& strPathKey)
{
	SetTag(strKey);
	m_strKey = strKey;
	m_strFileName = pFileName;
	m_strPathKey = strPathKey;

	for (int i = 0; i < iCount; ++i)
	{
		TCHAR strFullPath[MAX_PATH] = {};

		const TCHAR* pRootPath = GET_SINGLE(PathManager)->FindPath(strPathKey);

		if (pRootPath)
			lstrcat(strFullPath, pRootPath);

		lstrcat(strFullPath, pFileName);

		TCHAR strNum[MAX_PATH] = {};

		swprintf_s(strNum, TEXT("%d.bmp"), i);

		lstrcat(strFullPath, strNum);

		PTextureInfo pInfo = new TextureInfo;

		pInfo->hDC = CreateCompatibleDC(hDC);
		pInfo->hBmp = (HBITMAP)LoadImage(hInst, strFullPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		pInfo->hPrevBmp = (HBITMAP)SelectObject(pInfo->hDC, pInfo->hBmp);

		GetObject(pInfo->hBmp, sizeof(BITMAP), &pInfo->BmpInfo);

		m_vecTexture.push_back(pInfo);
	}

	return true;
}

bool Texture::LoadTextureFromFullPath(HINSTANCE hInst, HDC hDC, const string& strKey, const wchar_t* pFileName)
{
	m_strFileName = pFileName;
	m_strKey = strKey;

	m_hMemDC = CreateCompatibleDC(hDC);

	wstring strPath;

	strPath += pFileName;

	m_hBitmap = (HBITMAP)LoadImage(hInst, strPath.c_str(),
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	m_hOldBitmap = (HBITMAP)SelectObject(m_hMemDC,
		m_hBitmap);

	GetObject(m_hBitmap, sizeof(m_tInfo), &m_tInfo);

	return true;
}

void Texture::Render(HDC hDC, POSITION tPos, POSITION tImagePos, POSITION tSize, int iIndex)
{
	if (m_vecTexture[iIndex]->bColorKey)
		TransparentBlt(hDC, (int)tPos.x, (int)tPos.y, (int)tSize.x, (int)tSize.y, m_vecTexture[iIndex]->hDC,
			(int)tImagePos.x, (int)tImagePos.y, (int)tSize.x, (int)tSize.y, m_vecTexture[iIndex]->iColorKey);

	else
		BitBlt(hDC, (int)tPos.x, (int)tPos.y, (int)tSize.x, (int)tSize.y, m_vecTexture[iIndex]->hDC,
			(int)tImagePos.x, (int)tImagePos.y, SRCCOPY);
}

void Texture::SaveFromPath(const char * pFileName, const string & strPathKey)
{
}

void Texture::Save(FILE * pFile)
{
	int iLength = (int)m_strKey.length();

	fwrite(&iLength, 4, 1, pFile);
	fwrite(m_strKey.c_str(), 1, iLength, pFile);

	iLength = (int)m_strFileName.length();
	fwrite(&iLength, 4, 1, pFile);
	fwrite(m_strFileName.c_str(), 2, iLength, pFile);

	iLength = (int)m_strPathKey.length();
	fwrite(&iLength, 4, 1, pFile);
	fwrite(m_strPathKey.c_str(), 1, iLength, pFile);

	fwrite(&m_bColorKeyEnable, 1, 1, pFile);
	fwrite(&m_ColorKey, sizeof(COLORREF), 1, pFile);
}

void Texture::LoadFromPath(const char * pFileName, const string & strPathKey)
{

}

void Texture::Load(FILE * pFile)
{
	int iLength = 0;

	fread(&iLength, 4, 1, pFile);

	char strKey[MAX_PATH] = {};
	fread(strKey, 1, iLength, pFile);

	m_strKey = strKey;

	iLength = 0;
	fread(&iLength, 4, 1, pFile);

	wchar_t wstrFileName[MAX_PATH] = {};
	fread(wstrFileName, 2, iLength, pFile);

	m_strFileName = wstrFileName;

	iLength = 0;
	fread(&iLength, 4, 1, pFile);

	char strPathKey[MAX_PATH] = {};
	fread(strPathKey, 1, iLength, pFile);

	m_strPathKey = strPathKey;

	fread(&m_bColorKeyEnable, 1, 1, pFile);
	fread(&m_ColorKey, sizeof(COLORREF), 1, pFile);
}
