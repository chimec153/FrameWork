#include "ResourcesManager.h"
#include "Texture.h"
#include "../Core/PathManager.h"

DEFINITION_SINGLE(ResourcesManager)

ResourcesManager::ResourcesManager()	:
	m_hInst(nullptr),
	m_hDC(nullptr),
	m_pBackBuffer(nullptr),
	m_iItemCount(0)
{
}

ResourcesManager::~ResourcesManager()
{
	SAFE_RELEASE(m_pBackBuffer);
	Safe_Release_Map(m_mapTexture);
	Safe_Delete_VecList(m_vecItemInfo);
	Safe_Delete_VecList(m_vecAnimalInfo);
	Safe_Delete_VecList(m_vecMonsterInfo);
	SAFE_DELETE_MAP(m_mapShopInfo);
}

Texture * ResourcesManager::GetBackBuffer() const
{
	m_pBackBuffer->AddRef();
	return m_pBackBuffer;
}

bool ResourcesManager::Init(HINSTANCE hInst,HDC hDC)
{
	m_hInst = hInst;
	m_hDC = hDC;
	m_pBackBuffer = LoadTexture("BackBuffer", TEXT("AnimationUI\\BackGround.bmp"));

	Texture* pTexture = LoadTexture("Marlon", TEXT("Characters\\NPC\\MarlonAll.bmp"));

	SAFE_RELEASE(pTexture);

	LoadItemInfo(TEXT("SeedData.txt"));
	LoadAnimalInfo(TEXT("AnimalData.txt"));
	LoadMonsterInfo("monsterdata.txt");
	LoadShopInfo("Harvey", "HarveyShopData.txt");
	LoadShopInfo("Marlon", "MarlonShopData.txt");

	return true;
}

Texture * ResourcesManager::LoadTexture(
	const string & strKey, const wchar_t * pFileName, 
	const string & strPathKey)
{
	Texture*	pTexture = FindTexture(strKey);

	if (pTexture)
		return pTexture;
	
	pTexture = new Texture;

	if (!pTexture->LoadTexture(m_hInst, m_hDC, strKey, pFileName,
		strPathKey))
	{
		SAFE_RELEASE(pTexture);
		return NULL;
	}
	pTexture->AddRef();
	m_mapTexture.insert(make_pair(strKey, pTexture));

	return pTexture;
}

Texture* ResourcesManager::LoadTexture(const string& strKey, const vector<const TCHAR*>& vecFileName, const string& strPathKey)
{
	Texture*	pTexture = FindTexture(strKey);

	if (pTexture)
		return pTexture;
		
	pTexture = new Texture;

	if (!pTexture->LoadTexture(m_hInst, m_hDC, strKey, vecFileName, strPathKey))
	{
		SAFE_RELEASE(pTexture);
		return	nullptr;
	}

	pTexture->AddRef();
	m_mapTexture.insert(make_pair(strKey, pTexture));

	return pTexture;
}

Texture* ResourcesManager::LoadTexture(const string& strKey, const TCHAR* pFileName, int iCount, const string& strPathKey)
{
	Texture*	pTexture = FindTexture(strKey);

	if (pTexture)
		return pTexture;

	pTexture = new Texture;

	if (!pTexture->LoadTexture(m_hInst, m_hDC, strKey, pFileName, iCount, strPathKey))
	{
		SAFE_RELEASE(pTexture);
		return	nullptr;
	}

	pTexture->AddRef();
	m_mapTexture.insert(make_pair(strKey, pTexture));

	return pTexture;
}

Texture* ResourcesManager::LoadTextureFromFullPath(const string& strKey, const wchar_t* pFileName)
{
	Texture* pTexture = FindTexture(strKey);

	if (pTexture)
		return pTexture;

	pTexture = new Texture;

	if (!pTexture->LoadTextureFromFullPath(m_hInst, m_hDC, strKey, pFileName))
	{
		SAFE_RELEASE(pTexture);
		return NULL;
	}
	pTexture->AddRef();
	m_mapTexture.insert(make_pair(strKey, pTexture));

	return pTexture;
}

Texture * ResourcesManager::LoadTexture(FILE * pFile)
{
	int iLength = 0;

	char	strKey[MAX_PATH] = {};
	char	strPathKey[MAX_PATH] = {};
	wchar_t strFileName[MAX_PATH] = {};

	fread(&iLength, 4, 1, pFile);
	fread(strKey, 1, iLength, pFile);
	strKey[iLength] = 0;

	iLength = 0;
	fread(&iLength, 4, 1, pFile);
	fread(strFileName, 2, iLength, pFile);
	strFileName[iLength] = 0;

	iLength = 0;
	fread(&iLength, 4, 1, pFile);
	fread(strPathKey, 1, iLength, pFile);
	strPathKey[iLength] = 0;

	bool	bColorKey = false;
	COLORREF	dwColorKey = 0;
	fread(&bColorKey, 1, 1, pFile);
	fread(&dwColorKey, sizeof(COLORREF), 1, pFile);

	Texture* pTex = GET_SINGLE(ResourcesManager)->LoadTexture(strKey, strFileName, strPathKey);

	if (pTex && bColorKey)
		pTex->SetColorKey(dwColorKey);

	return pTex;
}

Texture * ResourcesManager::FindTexture(const string & strKey)
{
	unordered_map<string, class Texture*>::iterator	iter = m_mapTexture.find(strKey);
	if (iter == m_mapTexture.end())
		return NULL;
	iter->second->AddRef();

	return iter->second;
}

bool ResourcesManager::EraseTexture(const string& strKey)
{
	auto iter = m_mapTexture.find(strKey);

	if (iter == m_mapTexture.end())
		return false;

	m_mapTexture.erase(iter);

	return true;
}

bool ResourcesManager::LoadItemInfo(const TCHAR* pFileName, const string& strPathKey)
{
	TCHAR strFullPath[MAX_PATH] = {};

	const TCHAR* pDataPath = GET_SINGLE(PathManager)->FindPath(strPathKey);

	if (pDataPath)
		lstrcat(strFullPath, pDataPath);

	lstrcat(strFullPath, pFileName);

	char pMultibytePath[MAX_PATH] = {};

#ifdef _UNICODE
	WideCharToMultiByte(CP_ACP, NULL, strFullPath, -1, pMultibytePath, lstrlen(strFullPath), NULL, NULL);
#else
	strcpy_s(pMultibytePath, strFullPath);
#endif

	FILE* pFile = nullptr;

	fopen_s(&pFile, pMultibytePath, "rt");

	if (pFile)
	{
		char cLine[256] = {};

		fgets(cLine, 256, pFile);

		m_iItemCount = atoi(cLine);		//	아이템 개수

		int iItemCount = 0;

		while (true)
		{
			fgets(cLine, 256, pFile);

			PITEMINFO pInfo = new ITEMINFO;

			fgets(cLine, 256, pFile);

			size_t iSize = strlen(cLine);

			for (size_t i = 0; i < iSize; ++i)
			{
				if (cLine[i] == '\t')
				{
					cLine[i] = 0;
					break;
				}
			}

			pInfo->strName = cLine;		//	아이템 이름

			if (pInfo->strName == "end")
			{
				m_iItemCount = iItemCount;
				SAFE_DELETE(pInfo);
				break;
			}

			else
				++iItemCount;

			fgets(cLine, 256, pFile);

			int iTextCount = atoi(cLine);

			for (int i = 0; i < iTextCount; ++i)
			{
				TCHAR* pContext = new TCHAR[256];

				fgets(cLine, 256, pFile);

#ifdef _UNICODE
				int iLength = MultiByteToWideChar(CP_ACP, NULL, cLine, -1, pContext, 0);
				MultiByteToWideChar(CP_ACP, NULL, cLine, -1, pContext, iLength);
#else
				strcpy_s(pContext, cLine);
#endif

				pContext[strlen(cLine) - 1] = 0;

				pInfo->vecComment.push_back(pContext);
			}


			fgets(cLine, 256, pFile);

			pInfo->eItemType = (ITEM_TYPE)atoi(cLine);	//	아이템 타입

			if (pInfo->eItemType == IT_SEED)
			{
				fgets(cLine, 256, pFile);

				iSize = strlen(cLine);

				for (size_t i = 0; i < iSize; ++i)
				{
					if (cLine[i] == '\t')
					{
						cLine[i] = 0;
						break;
					}
				}

				pInfo->strCropName = cLine;	//	작물 이름

				fgets(cLine, 256, pFile);

				pInfo->eCropType = (CROP_TYPE)(iItemCount-1);	//	작물 타입

				fgets(cLine, 256, pFile);

				char* pContext = nullptr;

				char* pResult = strtok_s(cLine, ", ", &pContext);

				pInfo->tTileOffset = POSITION((float)atof(pResult), (float)atof(pContext));	//	씨앗 이미지 오프셋

				fgets(cLine, 256, pFile);

				pContext = nullptr;

				pResult = strtok_s(cLine, ", ", &pContext);

				pInfo->tCropOffset = POSITION((float)atof(pResult), (float)atof(pContext));	//	작물 오프셋

				fgets(cLine, 256, pFile);

				pContext = nullptr;

				pResult = strtok_s(cLine, ", ", &pContext);

				pInfo->tHarvestOffset = POSITION((float)atof(pResult), (float)atof(pContext));	//	수확 아이템 오프셋

				fgets(cLine, 256, pFile);

				int iPriceCount = atoi(cLine);	//	가게 수

				for (int i = 0; i < iPriceCount; ++i)
				{
					fgets(cLine, 256, pFile);

					pInfo->vecPrice.push_back(atoi(cLine));	//	상품 가격
				}

				fgets(cLine, 256, pFile);

				pInfo->iSeedSellPrice = atoi(cLine);	//	씨앗 판매가격

				fgets(cLine, 256, pFile);

				pInfo->iSellPrice = atoi(cLine);	//	작물 판매가격

				fgets(cLine, 256, pFile);

				pInfo->iMaxStage = atoi(cLine);	//	최종 단계

				fgets(cLine, 256, pFile);

				pContext = nullptr;

				pResult = strtok_s(cLine, ", ", &pContext);

				pInfo->vecMaxDay.push_back(atoi(pResult));	//	필요 일수

				for (int i = 0; i < pInfo->iMaxStage - 2; ++i)
				{
					pResult = strtok_s(nullptr, ", ", &pContext);

					pInfo->vecMaxDay.push_back(atoi(pResult));	//	필요 일수
				}

				fgets(cLine, 256, pFile);

				pInfo->iEnergyRecovery = atoi(cLine);	//	에너지 회복량

				fgets(cLine, 256, pFile);

				pInfo->iHPRecovery = atoi(cLine);	//	체력 회복량

				fgets(cLine, 256, pFile);

				pInfo->iRegrowthDay = atoi(cLine);	//	재성장 일
			}

			else if (pInfo->eItemType == IT_TOOL)
			{
				fgets(cLine, 256, pFile);

				pInfo->eToolType = (TOOL_TYPE)atoi(cLine);

				fgets(cLine, 256, pFile);

				char* pContext = nullptr;

				char* pResult = strtok_s(cLine, ", ", &pContext);

				pInfo->tTileOffset = POSITION((float)atof(pResult), (float)atof(pContext));	//	아이템 이미지 오프셋

				fgets(cLine, 256, pFile);

				int iPriceCount = atoi(cLine);	//	가게 수

				for (int i = 0; i < iPriceCount; ++i)
				{
					fgets(cLine, 256, pFile);

					pInfo->vecPrice.push_back(atoi(cLine));	//	상품 가격
				}

				fgets(cLine, 256, pFile);

				pInfo->iSellPrice = atoi(cLine);	//	아이템 판매가격

				if (pInfo->eToolType == TOOL_SWORD || pInfo->eToolType == TOOL_SCYTHE)
				{
					fgets(cLine, 256, pFile);

					pInfo->iAttack = atoi(cLine);	//	공격력
				}
			}

			else if (pInfo->eItemType == IT_ETC)
			{
				fgets(cLine, 256, pFile);

				char* pContext = nullptr;

				char* pResult = strtok_s(cLine, ", ", &pContext);

				pInfo->tTileOffset = POSITION((float)atof(pResult), (float)atof(pContext));	//	아이템 이미지 오프셋

				fgets(cLine, 256, pFile);

				pInfo->iSellPrice = atoi(cLine);
			}

			else if (pInfo->eItemType == IT_EGG)
			{
				fgets(cLine, 256, pFile);

				char* pContext = nullptr;

				char* pResult = strtok_s(cLine, ", ", &pContext);

				pInfo->tTileOffset = POSITION((float)atof(pResult), (float)atof(pContext));	//	아이템 이미지 오프셋

				fgets(cLine, 256, pFile);

				pInfo->eEggType = ((EGG_TYPE)atoi(cLine));

				fgets(cLine, 256, pFile);

				pInfo->iSellPrice = atoi(cLine);
			}

			m_vecItemInfo.push_back(pInfo);
		}

		fclose(pFile);
	}

	return true;
}

bool ResourcesManager::LoadShopInfo(const string& strName, const char* pFileName, const string& strPathKey)
{
	char strFullPath[MAX_PATH] = {};

	const char* pRootPath = GET_SINGLE(PathManager)->FindPathMultiByte(strPathKey);

	if (pRootPath)
		strcat(strFullPath, pRootPath);

	strcat(strFullPath, pFileName);

	FILE* pFile = nullptr;

	fopen_s(&pFile, strFullPath, "rt");

	if (pFile)
	{
		PSHOPINFO pInfo = new SHOPINFO;

		TCHAR cLine[256] = {};

		fgetws(cLine, 256, pFile);

		pInfo->iSize = _wtoi(cLine);

		fgetws(cLine, 256, pFile);

		pInfo->iLine = _wtoi(cLine);

		for (int i = 0; i < pInfo->iSize; ++i)
		{
			fgetws(cLine, 256, pFile);

			for (int j = 0; j < pInfo->iLine; ++j)
			{
				fgetws(cLine, 256, pFile);

				TCHAR* strText = new TCHAR[lstrlen(cLine) + 1];

				lstrcpy(strText, cLine);

				strText[lstrlen(cLine) - 1] = 0;

				pInfo->vecText.push_back(strText);
			}
		}

		fgetws(cLine, 256, pFile);

		fgetws(cLine, 256, pFile);

		pInfo->iBuyMessageCount = _wtoi(cLine);

		if (pInfo->iBuyMessageCount != 0)
		{
			fgetws(cLine, 256, pFile);

			TCHAR* pContext = nullptr;

			pInfo->tBuyImageOffset.x = (float)_wtof(wcstok_s(cLine, TEXT(", "), &pContext));
			pInfo->tBuyImageOffset.y = (float)_wtof(pContext);

			fgetws(cLine, 256, pFile);
		}

		for (int i = 0; i < pInfo->iBuyMessageCount; ++i)
		{
			for (int j = 0; j < pInfo->iLine; ++j)
			{
				fgetws(cLine, 256, pFile);

				TCHAR* strBuy = new TCHAR[lstrlen(cLine) + 1];

				memset(strBuy, 0, sizeof(TCHAR) * (lstrlen(cLine) + 1));

				lstrcat(strBuy, cLine);

				strBuy[lstrlen(cLine) - 1] = 0;

				pInfo->vecText.push_back(strBuy);
			}
		}

		fgetws(cLine, 256, pFile);

		fgetws(cLine, 256, pFile);

		TCHAR* pContext = nullptr;

		pInfo->tImageOffset.x = (float)_wtof(wcstok_s(cLine, TEXT(", "), &pContext));
		pInfo->tImageOffset.y = (float)_wtof(pContext);

		fgetws(cLine, 256, pFile);

		fgetws(cLine, 256, pFile);

		pInfo->iItemPanelStartIndex = (int)_wtoi(wcstok_s(cLine, TEXT(", "), &pContext));
		pInfo->iItemCount = _wtoi(pContext);

		m_mapShopInfo.insert(make_pair(strName, pInfo));

		fclose(pFile);

		return true;
	}

	return false;
}

PITEMINFO ResourcesManager::FindItemInfo(int iIndex)
{
	size_t iSize = m_vecItemInfo.size();

	if (iSize <= iIndex)
		return	nullptr;

	return m_vecItemInfo[iIndex];
}

bool ResourcesManager::LoadAnimalInfo(const TCHAR* pFileName, const string& strPathKey)
{
	char strFileName[MAX_PATH] = {};

	const char* pRootPath = GET_SINGLE(PathManager)->FindPathMultiByte(strPathKey);

#ifdef _UNICODE
	WideCharToMultiByte(CP_ACP, NULL, pFileName, -1, strFileName, lstrlen(pFileName), NULL, NULL);
#else
	strcpy_s(strFileName, pFileName);
#endif

	char strFullPath[MAX_PATH] = {};

	if(pRootPath)
		strcat_s(strFullPath, pRootPath);

	strcat_s(strFullPath, strFileName);

	FILE* pFile = nullptr;

	fopen_s(&pFile, strFullPath, "rt");

	if (pFile)
	{
		char cLine[256] = {};

		fgets(cLine, 256, pFile);

		int iAnimalCount = atoi(cLine);

		for (int i = 0; i < iAnimalCount; ++i)
		{
			fgets(cLine, 256, pFile);

			PITEMINFO pInfo = new ITEMINFO;

			fgets(cLine, 256, pFile);

			int iLength = (int)strlen(cLine);

			for (int i = 0; i < iLength; ++i)
			{
				if (cLine[i] == '\t')
				{
					cLine[i] = 0;
					break;
				}
			}

			pInfo->strName = cLine;

			fgets(cLine, 256, pFile);

			int iCount = atoi(cLine);

			for (int i = 0; i < iCount; ++i)
			{
				TCHAR* strDescript = new TCHAR[256];

				fgets(cLine, 256, pFile);

#ifdef _UNICODE
				int iLength = MultiByteToWideChar(CP_ACP, NULL, cLine, -1, strDescript, 0);
				MultiByteToWideChar(CP_ACP, NULL, cLine, -1, strDescript, iLength);
#else
				strcpy_s(strDescript, cLine);
#endif

				strDescript[lstrlen(strDescript)-1] = 0;

				pInfo->vecComment.push_back(strDescript);
			}

			fgets(cLine, 256, pFile);

			pInfo->vecPrice.push_back(atoi(cLine));

			fgets(cLine, 256, pFile);

			pInfo->iSellPrice = atoi(cLine);

			m_vecAnimalInfo.push_back(pInfo);
		}

		fclose(pFile);
	}

	return true;
}

PITEMINFO ResourcesManager::FindAnimalInfo(int iIndex)
{
	size_t iSize = m_vecAnimalInfo.size();

	if (iSize <= iIndex)
		return	nullptr;

	return m_vecAnimalInfo[iIndex];
}

bool ResourcesManager::LoadMonsterInfo(const char* pFileName, const string& strPathKey)
{
	char strFullPath[MAX_PATH] = {};

	const char* pRootPath = GET_SINGLE(PathManager)->FindPathMultiByte(strPathKey);

	if (pRootPath)
		strcat(strFullPath, pRootPath);

	strcat(strFullPath, pFileName);

	FILE* pFile = nullptr;

	fopen_s(&pFile, strFullPath, "rt");

	if (pFile)
	{
		char cLine[256] = {};

		while (true)
		{
			fgets(cLine, 256, pFile);

			int iMaxLevel = atoi(cLine);

			if (iMaxLevel == -1)
				break;

			PMonsterInfo pInfo = new MonsterInfo;

			fgets(cLine, 256, pFile);

			char* pContext = nullptr;

			char* pResult = strtok_s(cLine, "\t", &pContext);

			pInfo->vecHP.push_back(atoi(pResult));

			for (int i = 0; i < iMaxLevel - 1; ++i)
			{
				pResult = strtok_s(nullptr, "\t", &pContext);

				pInfo->vecHP.push_back(atoi(pResult));
			}

			fgets(cLine, 256, pFile);

			pResult = strtok_s(cLine, "\t", &pContext);

			pInfo->vecAttack.push_back(atoi(pResult));

			for (int i = 0; i < iMaxLevel - 1; ++i)
			{
				pResult = strtok_s(nullptr, "\t", &pContext);

				pInfo->vecAttack.push_back(atoi(pResult));
			}

			m_vecMonsterInfo.push_back(pInfo);
		}

		fclose(pFile);
	}

	return true;
}

PMonsterInfo ResourcesManager::FindMonsterInfo(int iIndex)
{
	return m_vecMonsterInfo[iIndex];
}

PSHOPINFO ResourcesManager::FindShopInfo(const string& strName)
{
	auto iter = m_mapShopInfo.find(strName);

	if (iter == m_mapShopInfo.end())
		return nullptr;

	return iter->second;
}
