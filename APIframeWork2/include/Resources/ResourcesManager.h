#pragma once
#include "../game.h"

typedef struct _tagItemInfo
{
	string			strName;
	vector<int>		vecPrice;
}ITEMINFO, *PITEMINFO;

class ResourcesManager
{
private:
	unordered_map<string, class Texture*>	m_mapTexture;
	vector<PITEMINFO>						m_vecItemInfo;
	HINSTANCE m_hInst;
	HDC	m_hDC;
	class Texture* m_pBackBuffer;

public:
	class Texture* GetBackBuffer() const;
	bool Init(HINSTANCE hInst,HDC hDC);
	class Texture* LoadTexture(const string& strKey,
		const wchar_t* pFileName,
		const string& strPathKey = TEXTURE_PATH);
	class Texture* LoadTexture(const string& strKey,
		const vector<const TCHAR*>& vecFileName,
		const string& strPathKey = TEXTURE_PATH);
	class Texture* LoadTexture(const string& strKey,
		const TCHAR* pFileName,
		int iCount,	const string& strPathKey = TEXTURE_PATH);
	class Texture* LoadTextureFromFullPath(const string& strKey,
		const wchar_t* pFileName);
	class Texture* LoadTexture(FILE* pFile);
	class Texture* FindTexture(const string& strKey);

public:
	bool LoadItemInfo(const TCHAR* pFileName, const string& strPathKey = DATA_PATH);
	PITEMINFO FindItemInfo(int iIndex);

	DECLARE_SINGLE(ResourcesManager)
};