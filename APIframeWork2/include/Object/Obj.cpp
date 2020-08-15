#include "Obj.h"
#include "../Scene/Layer.h"
#include "../Scene/SceneManager.h"
#include "../Scene/Scene.h"
#include "../Resources/Texture.h"
#include "../Resources/ResourcesManager.h"
#include "../Core/Camera.h"
#include "../Collider/Collider.h"
#include "../Animation/Animation.h"
#include "../Core/PathManager.h"
#include "../Collider/ColliderPixel.h"
#include "../Collider/ColliderPoint.h"
#include "../Collider/ColliderRect.h"
#include "../Collider/ColliderSphere.h"

list<Obj*> Obj::m_ObjList;

Obj::Obj()	:
	m_pScene(nullptr),
	m_pLayer(nullptr),
	m_blsPhysics(false),
	m_fGravityTime(0.f),
	m_tPos(),
	m_tSize(),
	m_tImageOffset(),
	m_tPivot(),
	m_pTexture(NULL),
	m_pAnimation(NULL),
	m_cAlpha(0),
	m_bAlphaOn(false),
	m_eBlock(OB_NONE),
	m_bEnableCollide(true)
{
}

Obj::Obj(const Obj& obj)
{
	*this = obj;
	m_iRef = 1;
	if (obj.m_pAnimation)
	{
		m_pAnimation = obj.m_pAnimation->Clone();

		m_pAnimation->m_pObj = this;
	}

	m_fGravityTime = 0.f;

	if (m_pTexture)
		m_pTexture->AddRef();

	m_ColliderList.clear();

	list<Collider*>::const_iterator	iter;
	list<Collider*>::const_iterator	iterEnd = obj.m_ColliderList.end();
	for (iter = obj.m_ColliderList.begin(); iter != iterEnd; ++iter)
	{
		Collider*	pColl = (*iter)->Clone();

		pColl->SetObj(this);

		m_ColliderList.push_back(pColl);
	}
}

Obj::~Obj()
{
	SAFE_RELEASE(m_pAnimation);
	Safe_Release_VecList(m_ColliderList);
	SAFE_RELEASE(m_pTexture);
}

void Obj::AddObj(Obj * pObj)
{
	pObj->AddRef();
	m_ObjList.push_back(pObj);
}

Obj * Obj::FindObject(const string & strTag)
{
	list<Obj*>::iterator	iter;
	list<Obj*>::iterator	iterEnd = m_ObjList.end();
	for (iter = m_ObjList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->GetTag() == strTag)
		{
			(*iter)->AddRef();
			return *iter;
		}
	}
	return NULL;
}

void Obj::EraseObj(Obj * pObj)
{
	list<Obj*>::iterator	iter;
	list<Obj*>::iterator	iterEnd = m_ObjList.end();
	for (iter = m_ObjList.begin(); iter != iterEnd; ++iter)
	{
		if (*iter== pObj)
		{
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
			return;
		}
	}
}

void Obj::EraseObj(const string & strTag)
{
	list<Obj*>::iterator	iter;
	list<Obj*>::iterator	iterEnd = m_ObjList.end();
	for (iter = m_ObjList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->GetTag() == strTag)
		{
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
			return;
		}
	}
}

void Obj::EraseObj()
{
	Safe_Release_VecList(m_ObjList);
}

Animation * Obj::CreateAnimation(const string & strTag)
{
	SAFE_RELEASE(m_pAnimation);

	m_pAnimation = new Animation;

	m_pAnimation->SetTag(strTag);
	m_pAnimation->SetObj(this);
	if (!m_pAnimation->Init())
	{
		SAFE_RELEASE(m_pAnimation);
		return NULL;
	}

	m_pAnimation->AddRef();

	return m_pAnimation;
}

bool Obj::AddAnimationClip(const string & strName, ANIMATION_TYPE eType,
	ANIMATION_OPTION eOption, float fAnimationLimitTime, int iFrameMaxX, 
	int iFrameMaxY, int iStartX, int iStartY, int iLengthX, int iLengthY, 
	float fOptionLimitTime, const string & strTextKey, 
	const wchar_t * pFileName, const string & strPathKey)
{
	if (!m_pAnimation)
		return false;

	m_pAnimation->AddClip(strName, eType, eOption, fAnimationLimitTime,
		iFrameMaxX, iFrameMaxY, iStartX, iStartY, iLengthX, iLengthY,
		fOptionLimitTime, strTextKey, pFileName, strPathKey);
	return true;
}

bool Obj::AddAnimationClip(const string & strName, ANIMATION_TYPE eType, 
	ANIMATION_OPTION eOption, float fAnimationLimitTime, int iFrameMaxX, 
	int iFrameMaxY, int iStartX, int iStartY, int iLengthX, int iLengthY, 
	float fOptionLimitTime, const string & strTextKey, 
	const vector<wstring>& vecFileName, const string & strPathKey)
{
	if (!m_pAnimation)
		return false;

	m_pAnimation->AddClip(strName, eType, eOption, fAnimationLimitTime,
		iFrameMaxX, iFrameMaxY, iStartX, iStartY, iLengthX, iLengthY,
		fOptionLimitTime, strTextKey, vecFileName, strPathKey);

	return true;
}

void Obj::SetAnimationCurrentClip(const string& strName)
{
	if (m_pAnimation)
		m_pAnimation->SetCurrentClip(strName);
}

void Obj::SetAnimationDefaultClip(const string& strName)
{
	if (m_pAnimation)
		m_pAnimation->SetDefaultClip(strName);
}

void Obj::SetAnimationClipColorKey(const string & strClip, unsigned char r, unsigned char g, unsigned char b)
{
	if (m_pAnimation)
		m_pAnimation->SetClipColorKey(strClip, r, g, b);
}

Texture* Obj::GetTexture() const
{
	if (m_pTexture)
		m_pTexture->AddRef();

	return m_pTexture;
}

Collider * Obj::GetCollider(const string & strTag)
{
	list < Collider*>::iterator		iter;
	list < Collider*>::iterator		iterEnd=m_ColliderList.end();

	for (iter = m_ColliderList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->GetTag() == strTag)
		{
			(*iter)->AddRef();
			return *iter;
		}
	}
	return nullptr;
}



void Obj::SetTexture(Texture * pTexture)
{
	SAFE_RELEASE(m_pTexture);
	m_pTexture = pTexture;

	if (pTexture)
		pTexture->AddRef();
}

void Obj::SetTexture(const string & strKey, 
	const wchar_t * pFileName, const string & strPathKey)
{
	SAFE_RELEASE(m_pTexture);
	m_pTexture = GET_SINGLE(ResourcesManager)->LoadTexture(
		strKey, pFileName, strPathKey);
}

void Obj::SetTexture(const string& strKey, const vector<const TCHAR*>& vecFileName, const string& strPathKey)
{
	SAFE_RELEASE(m_pTexture);

	m_pTexture = GET_SINGLE(ResourcesManager)->LoadTexture(strKey, vecFileName, strPathKey);
}

void Obj::SetTexture(const string& strKey, const TCHAR* pFileName, int iCount, const string& strPathKey)
{
	SAFE_RELEASE(m_pTexture);

	m_pTexture = GET_SINGLE(ResourcesManager)->LoadTexture(strKey, pFileName, iCount, strPathKey);
}

void Obj::SetColorKey(unsigned char r, unsigned char g, unsigned char b)
{
	if(m_pTexture)
		m_pTexture->SetColorKey(r, g, b);
}

void Obj::Input(float fDeltaTime)
{
}

int Obj::Update(float fDeltaTime)
{
	list<Collider*>::iterator	iter;
	list<Collider*>::iterator	iterEnd=m_ColliderList.end();

	for (iter = m_ColliderList.begin(); iter != iterEnd;)
	{
		if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}
		(*iter)->Update(fDeltaTime);
		if (!(*iter)->GetLife())
		{
			SAFE_RELEASE((*iter));
			iter = m_ColliderList.erase(iter);
			iterEnd = m_ColliderList.end();
		}
		else
			++iter;
	}

	if (m_pAnimation)
		m_pAnimation->Update(fDeltaTime);

	return 0;
}

int Obj::LateUpdate(float fDeltaTime)
{
	list<Collider*>::iterator	iter;
	list<Collider*>::iterator	iterEnd = m_ColliderList.end();

	for (iter = m_ColliderList.begin(); iter != iterEnd;)
	{
		if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}
		(*iter)->LateUpdate(fDeltaTime);
		if (!(*iter)->GetLife())
		{
			SAFE_RELEASE((*iter));
			iter = m_ColliderList.erase(iter);
			iterEnd = m_ColliderList.end();
		}
		else
			++iter;
	}
	return 0;
}

void Obj::Collision(float fDeltaTime)
{
}

void Obj::Render(HDC hDC, float fDeltaTime)
{
	POSITION	tPos = m_tPos - m_tSize * m_tPivot;
	tPos -= GET_SINGLE(Camera)->GetPos();

	RESOLUTION	tClientRS = GET_SINGLE(Camera)->GetClientRS();

	bool	bInClient = true;

	if (tPos.x + m_tSize.x < 0)
		bInClient = false;
	else if (tPos.x > tClientRS.iW)
		bInClient = false;
	else if (tPos.y + m_tSize.y < 0)
		bInClient = false;
	else if (tPos.y > tClientRS.iH)
		bInClient = false;

	if (m_pTexture && bInClient)
	{
		POSITION	tImagePos;

		if (m_pAnimation)
		{
			PANIMATIONCLIP	pClip = m_pAnimation->GetCurrentClip();
			
			if (pClip->eType == AT_ATLAS)
			{
				tImagePos.x = pClip->iFrameX * pClip->tFrameSize.x;
				tImagePos.y = pClip->iFrameY * pClip->tFrameSize.y;
			}
		}

		tImagePos += m_tImageOffset;

		if (!m_bAlphaOn)
		{
			if (m_pTexture->GetColorKeyEnable())
			{
				TransparentBlt(hDC, (int)tPos.x, (int)tPos.y, (int)m_tSize.x,
					(int)m_tSize.y, m_pTexture->GetDC(), (int)tImagePos.x, (int)tImagePos.y,
					(int)m_tSize.x, (int)m_tSize.y, m_pTexture->GetColorKey());
			}
			else
			{
				BitBlt(hDC, (int)tPos.x, (int)tPos.y, (int)m_tSize.x,
					(int)m_tSize.y, m_pTexture->GetDC(), (int)tImagePos.x, (int)tImagePos.y,
					SRCCOPY);
			}
		}

		else
		{
			BLENDFUNCTION	tBF = {};

			tBF.BlendOp = 0;
			tBF.BlendFlags = 0;
			tBF.SourceConstantAlpha = m_cAlpha;

			tBF.AlphaFormat = AC_SRC_ALPHA;

			GdiAlphaBlend(hDC, (int)tPos.x, (int)tPos.y, (int)m_tSize.x, (int)m_tSize.y,
				m_pTexture->GetDC(), (int)tImagePos.x, (int)tImagePos.y, (int)m_tSize.x, (int)m_tSize.y, tBF);
		}
	}

#ifdef _DEBUG
	if (bInClient)
	{
		list<Collider*>::iterator	iter;
		list<Collider*>::iterator	iterEnd = m_ColliderList.end();

		for (iter = m_ColliderList.begin(); iter != iterEnd;)
		{
			if (!(*iter)->GetEnable())
			{
				++iter;
				continue;
			}
			(*iter)->Render(hDC, fDeltaTime);
			if (!(*iter)->GetLife())
			{
				SAFE_RELEASE((*iter));
				iter = m_ColliderList.erase(iter);
				iterEnd = m_ColliderList.end();
			}
			else
				++iter;
		}
	}
#endif
}

void Obj::SaveFromPath(const char * pFileName, const string & strPathKey)
{
	const char* pPath = GET_SINGLE(PathManager)->FindPathMultiByte(DATA_PATH);

	string	strFullPath;
	if (pPath)
		strFullPath = pPath;

	strFullPath += pFileName;
	SaveFromFullPath(strFullPath.c_str());
}

void Obj::SaveFromFullPath(const char * pFullPath)
{
	FILE*	pFile = NULL;

	fopen_s(&pFile, pFullPath, "wb");

	if (pFile)
	{
		Save(pFile);
		fclose(pFile);
	}
}

void Obj::Save(FILE * pFile)
{
	size_t iLength = m_strTag.length();

	fwrite(&iLength, sizeof(iLength), 1, pFile);
	fwrite(m_strTag.c_str(), 1, iLength, pFile);
	fwrite(&m_blsPhysics, 1, 1, pFile);
	fwrite(&m_tPos, sizeof(m_tPos), 1, pFile);
	fwrite(&m_tSize, sizeof(m_tSize), 1, pFile);
	fwrite(&m_tImageOffset, sizeof(m_tImageOffset), 1, pFile);
	fwrite(&m_tPivot, sizeof(m_tPivot), 1, pFile);

	bool	bTexture = false;
	if (m_pTexture)
	{
		bTexture = true;
		fwrite(&bTexture, 1, 1, pFile);
		m_pTexture->Save(pFile);
	}
	else
	{
		fwrite(&bTexture, 1, 1, pFile);
	}

	iLength = m_ColliderList.size();

	fwrite(&iLength, sizeof(iLength), 1, pFile);
	list<Collider*>::iterator	iter;
	list<Collider*>::iterator	iterEnd = m_ColliderList.end();

	for (iter = m_ColliderList.begin(); iter != iterEnd; ++iter)
	{
		COLLIDER_TYPE	eType = (*iter)->GetColliderType();
		fwrite(&eType, 4, 1, pFile);
		(*iter)->Save(pFile);
	}

	bool	bAnimation = false;

	if (m_pAnimation)
	{
		bAnimation = true;
		fwrite(&bAnimation, 1, 1, pFile);
		m_pAnimation->Save(pFile);
	}

	else
		fwrite(&bAnimation, 1, 1, pFile);

	fwrite(&m_eBlock, sizeof(m_eBlock), 1, pFile);
}

void Obj::LoadFromPath(const char * pFileName, const string & strPathKey)
{
	const char* pPath = GET_SINGLE(PathManager)->FindPathMultiByte(DATA_PATH);

	string	strFullPath;

	if (pPath)
		strFullPath = pPath;

	strFullPath += pFileName;

	LoadFromFullPath(strFullPath.c_str());
}

void Obj::LoadFromFullPath(const char * pFullPath)
{
	FILE*	pFile = NULL;

	fopen_s(&pFile, pFullPath, "rb");

	if (pFile)
	{
		Load(pFile);
		fclose(pFile);
	}
}

void Obj::Load(FILE * pFile)
{
	size_t iLength = 0;
	char	strText[MAX_PATH] = {};

	fread(&iLength, sizeof(size_t), 1, pFile);
	fread(strText, 1, iLength, pFile);

	strText[iLength] = 0;
	m_strTag = strText;

	fread(&m_blsPhysics, 1, 1, pFile);
	fread(&m_tPos, sizeof(m_tPos), 1, pFile);
	fread(&m_tSize, sizeof(m_tSize), 1, pFile);
	fread(&m_tImageOffset, sizeof(m_tImageOffset), 1, pFile);
	fread(&m_tPivot, sizeof(m_tPivot), 1, pFile);

	bool	bTexture = false;
	fread(&bTexture, 1, 1, pFile);

	SAFE_RELEASE(m_pTexture);

	if (bTexture)
		m_pTexture = GET_SINGLE(ResourcesManager)->LoadTexture(pFile);


	iLength = 0;

	fread(&iLength, sizeof(size_t), 1, pFile);

	for (int i = 0; i < iLength; ++i)
	{
		COLLIDER_TYPE	eType;
		fread(&eType, 4, 1, pFile);

		Collider* pCollider = NULL;
		switch (eType)
		{
		case CT_RECT:
			pCollider = AddCollider<ColliderRect>("");
			break;
		case CT_SPHERE:
			pCollider = AddCollider<ColliderSphere>("");
			break;
		case CT_LINE:
			break;
		case CT_POINT:
			pCollider = AddCollider<ColliderPoint>("");
			break;
		case CT_PIXEL:
			pCollider = AddCollider<ColliderPixel>("");
			break;
		}
		pCollider->Load(pFile);
		SAFE_RELEASE(pCollider);
	}
	//list<Collider*>::iterator	iter;
	//list<Collider*>::iterator	iterEnd = m_ColliderList.end();
	//
	//for (iter = m_ColliderList.begin(); iter != iterEnd; ++iter)
	//{
	//	(*iter)->Load(pFile);
	//}

	bool	bAnimation = false;
	fread(&bAnimation, 1, 1, pFile);

	SAFE_RELEASE(m_pAnimation);

	if (bAnimation)
	{
		m_pAnimation = new Animation;
		m_pAnimation->Init();
		m_pAnimation->Load(pFile);
	}

	fread(&m_eBlock, sizeof(m_eBlock), 1, pFile);
}

void Obj::SaveToVector(vector<COLORREF>& vecColor, int iFrame, int iStart, POSITION tPos)
{
	for (int i = 0; i < m_tSize.y; ++i)
	{
		for (int j = 0; j < m_tSize.x; ++j)
		{
			COLORREF tColorKey = m_pTexture->GetColorKey();
			COLORREF tPixel = GetPixel(m_pTexture->GetDC(), 
				j + (int)m_tImageOffset.x, 
				i + (int)m_tImageOffset.y);

			if(tColorKey != tPixel)
				vecColor[32 * (iFrame * (i + (int)tPos.y) + iStart) + j + (int)tPos.x] = tPixel;				
		}
	}	
}

void Obj::SaveToVector(vector<COLORREF>& vecColor, int iFrame, int iStart, int iOffset, POSITION tPos)
{
	for (int i = 0; i < m_tSize.y; ++i)
	{
		for (int j = 0; j < m_tSize.x; ++j)
		{
			COLORREF tColorKey = m_pTexture->GetColorKey();
			COLORREF tPixel = GetPixel(m_pTexture->GetDC(), 
				j + (int)m_tImageOffset.x + iOffset, 
				i + (int)m_tImageOffset.y);

			if (tColorKey != tPixel)
				vecColor[32 * (iFrame * (i + (int)tPos.y) + iStart) + j + (int)tPos.x] = tPixel;
		}
	}
}

Obj * Obj::CreateCloneObj(const string & strPtototypeKey, 
	const string & strTag,SCENE_CREATE sc, class Layer* pLayer)
{
	Obj*	pProto = Scene::FindProtoType(strPtototypeKey, sc);

	if (!pProto)
		return NULL;

	Obj* pObj = pProto->Clone();

	pObj->SetTag(strTag);

	if (pLayer)
	{
		pLayer->AddObject(pObj);
	}

	AddObj(pObj);

	return pObj;
}
