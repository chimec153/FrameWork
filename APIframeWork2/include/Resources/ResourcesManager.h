#pragma once
#include "../game.h"

typedef struct _tagShopInfo
{
	vector<TCHAR*>		vecText;
	int					iLine;
	int					iSize;
	int					iBuyMessageCount;
	POSITION			tBuyImageOffset;
	POSITION			tImageOffset;
	int					iItemPanelStartIndex;
	int					iItemCount;

	_tagShopInfo()	:
		iLine(0),
		iSize(0),
		iBuyMessageCount(0),
		iItemPanelStartIndex(0),
		iItemCount(0)
	{

	}

	~_tagShopInfo()
	{
		for(int i=0;i<(iSize + iBuyMessageCount) * iLine;++i)
			delete[] vecText[i];
	}
}SHOPINFO, *PSHOPINFO;

typedef struct _tagMonsterInfo
{
	vector<int>		vecAttack;
	vector<int>		vecHP;
}MonsterInfo, *PMonsterInfo;

class ResourcesManager
{
private:
	unordered_map<string, class Texture*>	m_mapTexture;
	vector<PITEMINFO>						m_vecItemInfo;
	vector<PITEMINFO>						m_vecAnimalInfo;
	vector<PMonsterInfo>					m_vecMonsterInfo;
	unordered_map<string, PSHOPINFO>		m_mapShopInfo;
	int										m_iItemCount;
	HINSTANCE								m_hInst;
	HDC										m_hDC;
	class Texture*							m_pBackBuffer;

public:
	int GetItemCount()	const
	{
		return m_iItemCount;
	}

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
	bool EraseTexture(const string& strKey);

public:
	bool LoadItemInfo(const TCHAR* pFileName, const string& strPathKey = DATA_PATH);
	PITEMINFO FindItemInfo(int iIndex);

public:
	bool LoadAnimalInfo(const TCHAR* pFileName, const string& strPathKey = DATA_PATH);
	PITEMINFO FindAnimalInfo(int iIndex);

public:
	bool LoadMonsterInfo(const char* pFileName, const string& strPathKey = DATA_PATH);
	PMonsterInfo FindMonsterInfo(int iIndex);

public:
	bool LoadShopInfo(const string& strName, const char* pFileName, const string& strPathKey = DATA_PATH);
	PSHOPINFO FindShopInfo(const string& strName);

	DECLARE_SINGLE(ResourcesManager)
};