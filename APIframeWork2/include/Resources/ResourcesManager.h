#pragma once
#include "../game.h"

typedef struct _tagItemInfo
{
	string			strName;
	string			strCropName;
	ITEM_TYPE		eItemType;
	CROP_TYPE		eCropType;
	TOOL_TYPE		eToolType;
	POSITION		tTileOffset;
	POSITION		tCropOffset;
	POSITION		tHarvestOffset;
	vector<int>		vecPrice;
	int				iSeedSellPrice;
	int				iSellPrice;
	int				iMaxStage;
	vector<int>		vecMaxDay;
	int				iEnergyRecovery;
	int				iHPRecovery;
	int				iRegrowthDay;
	int				iAttack;
	vector<TCHAR*>	vecComment;

	_tagItemInfo() :
		eItemType(IT_NONE),
		eCropType(CROP_PARSNIP),
		eToolType(TOOL_SWORD),
		iSeedSellPrice(0),
		iSellPrice(0),
		iMaxStage(0),
		iEnergyRecovery(0),
		iHPRecovery(0),
		iRegrowthDay(0),
		iAttack(0)
	{

	}

	~_tagItemInfo()
	{
		size_t iSize = vecComment.size();

		for (size_t i = 0; i < iSize; ++i)
			delete[] vecComment[i];
	}

}ITEMINFO, *PITEMINFO;

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

class ResourcesManager
{
private:
	unordered_map<string, class Texture*>	m_mapTexture;
	vector<PITEMINFO>						m_vecItemInfo;
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
	bool LoadShopInfo(const string& strName, const char* pFileName, const string& strPathKey = DATA_PATH);
	PSHOPINFO FindShopInfo(const string& strName);

	DECLARE_SINGLE(ResourcesManager)
};