#include "Animation.h"
#include "../Resources/Texture.h"
#include "../Resources/ResourcesManager.h"
#include "../Object/Obj.h"

Animation::Animation()	:
	m_pCurClip(NULL),
	m_bMotionEnd(false)
{
}

Animation::Animation(const Animation & anim)
{
	*this = anim;

	m_bMotionEnd = false;
	m_mapClip.clear();

	unordered_map<string, PANIMATIONCLIP>::const_iterator iter;
	unordered_map<string, PANIMATIONCLIP>::const_iterator iterEnd = anim.m_mapClip.end();

	for (iter = anim.m_mapClip.begin(); iter != iterEnd; ++iter)
	{
		PANIMATIONCLIP	pClip = new ANIMATIONCLIP;

		*pClip = *iter->second;

		for (size_t i = 0; i < pClip->vecTexture.size(); ++i)
		{
			pClip->vecTexture[i]->AddRef();
		}

		m_mapClip.insert(make_pair(iter->first, pClip));
	}

	m_pCurClip = NULL;

	m_strCurClip = "";
	SetCurrentClip(anim.m_strCurClip);
}

Animation::~Animation()
{
	unordered_map<string, PANIMATIONCLIP>::iterator iter;
	unordered_map<string, PANIMATIONCLIP>::iterator iterEnd = m_mapClip.end();

	for (iter = m_mapClip.begin(); iter != iterEnd; ++iter)
	{
		for (size_t i = 0; i < iter->second->vecTexture.size(); ++i)
		{
			SAFE_RELEASE(iter->second->vecTexture[i]);
		}
		SAFE_DELETE(iter->second);
	}

	m_mapClip.clear();
}

bool Animation::AddClip(const string & strName, ANIMATION_TYPE eType, 
	ANIMATION_OPTION eOption, float fAnimationLimitTime, int iFrameMaxX, 
	int iFrameMaxY, int iStartX, int iStartY, int iLengthX, int iLengthY, 
	float fOptionLimitTime, const string & strTextKey, 
	const wchar_t * pFileName, const string & strPathKey)
{
	PANIMATIONCLIP pClip = new ANIMATIONCLIP;

	pClip->eType = eType;
	pClip->eOption = eOption;
	pClip->fAnimationLimitTime = fAnimationLimitTime;
	pClip->iFrameMaxY = iFrameMaxY;
	pClip->iFrameMaxX = iFrameMaxX;
	pClip->iStartX = iStartX;
	pClip->iStartY = iStartY;
	pClip->iLengthX = iLengthX;
	pClip->iLengthY = iLengthY;
	pClip->fOptionLimitTime = fOptionLimitTime;
	pClip->fAnimationFrameTime = fAnimationLimitTime / (iLengthX * iLengthY);

	Texture*	pTex = GET_SINGLE(ResourcesManager)->LoadTexture(strTextKey,
		pFileName, strPathKey);

	if(iFrameMaxX != 0)
		pClip->tFrameSize.x = (float)(pTex->GetWidth() / iFrameMaxX);

	if(iFrameMaxY!=0)
		pClip->tFrameSize.y = (float)(pTex->GetHeight() / iFrameMaxY);

	pClip->vecTexture.push_back(pTex);

	pClip->fAnimationTime = 0.f;
	pClip->iFrameX = iStartX;
	pClip->iFrameY = iStartY;
	pClip->fOptionTime = 0.f;

	m_mapClip.insert(make_pair(strName, pClip));

	if (m_strDefaultClip.empty())
		SetDefaultClip(strName);

	if (m_strCurClip.empty())
		SetCurrentClip(strName);

	return true;
}

bool Animation::AddClip(const string & strName, ANIMATION_TYPE eType, 
	ANIMATION_OPTION eOption, float fAnimationLimitTime, int iFrameMaxX, 
	int iFrameMaxY, int iStartX, int iStartY, int iLengthX, int iLengthY, 
	float fOptionLimitTime, const string & strTextKey, 
	const vector<wstring>& vecFileName, const string & strPathKey)
{
	PANIMATIONCLIP pClip = new ANIMATIONCLIP;

	pClip->eType = eType;
	pClip->eOption = eOption;
	pClip->fAnimationLimitTime = fAnimationLimitTime;
	pClip->iFrameMaxY = iFrameMaxY;
	pClip->iFrameMaxX = iFrameMaxX;
	pClip->iStartX = iStartX;
	pClip->iStartY = iStartY;
	pClip->iLengthX = iLengthX;
	pClip->iLengthY = iLengthY;
	pClip->fOptionLimitTime = fOptionLimitTime;
	pClip->fAnimationFrameTime = fAnimationLimitTime / (iLengthX * iLengthY);

	for (size_t i = 0; i < vecFileName.size(); ++i)
	{
		char	strKey[256] = {};
		sprintf(strKey, "%s%d", strTextKey.c_str(), (int)i + 1);
		Texture*	pTex = GET_SINGLE(ResourcesManager)->LoadTexture(strKey,
			vecFileName[i].c_str(), strPathKey);

		pClip->vecTexture.push_back(pTex);
	}


	pClip->tFrameSize.x = (float)(pClip->vecTexture[0]->GetWidth() / iFrameMaxX);
	pClip->tFrameSize.y = (float)(pClip->vecTexture[0]->GetHeight() / iFrameMaxY);


	pClip->fAnimationTime = 0.f;
	pClip->iFrameX = iStartX;
	pClip->iFrameY = iStartY;
	pClip->fOptionTime = 0.f;

	m_mapClip.insert(make_pair(strName, pClip));

	if (m_strDefaultClip.empty())
		SetDefaultClip(strName);

	if (m_strCurClip.empty())
		SetCurrentClip(strName);

	return true;
}

void Animation::SetClipColorKey(const string& strClip,
	unsigned char r, unsigned char g, unsigned char b)
{
	PANIMATIONCLIP	pClip = FindClip(strClip);

	if (!pClip)
		return;
	for (size_t i = 0; i < pClip->vecTexture.size(); ++i)
	{
		pClip->vecTexture[i]->SetColorKey(r, g, b);
	}
}

void Animation::SetCurrentClip(const string & strCurClip)
{
	ChangeClip(strCurClip);
}

void Animation::SetDefaultClip(const string & strDefaultClip)
{
	m_strDefaultClip = strDefaultClip;
}

void Animation::ChangeClip(const string & strClip)
{
	if (m_strCurClip == strClip)
		return;

	m_strCurClip = strClip;

	if (m_pCurClip)
	{
		m_pCurClip->iFrameX = m_pCurClip->iStartX;
		m_pCurClip->iFrameY = m_pCurClip->iStartY;
		m_pCurClip->fAnimationTime = 0.f;
		m_pCurClip->fOptionTime = 0.f;
	}
	m_pCurClip = FindClip(strClip);

	if (m_pCurClip->eType == AT_ATLAS)
 		m_pObj->SetTexture(m_pCurClip->vecTexture[0]);

	else if (m_pCurClip->eType == AT_FRAME)
		m_pObj->SetTexture(m_pCurClip->vecTexture[m_pCurClip->iFrameX]);

}

void Animation::ReturnClip()
{
	ChangeClip(m_strDefaultClip);
}

PANIMATIONCLIP Animation::FindClip(const string & strName)
{
	unordered_map<string, PANIMATIONCLIP>::iterator	iter = m_mapClip.find(strName);

	if (iter == m_mapClip.end())
		return NULL;

	return iter->second;
}

bool Animation::Init()
{
	return true;
}

void Animation::Update(float fTime)
{
	m_bMotionEnd = false;
	m_pCurClip->fAnimationTime += fTime;

	while (m_pCurClip->fAnimationTime >= m_pCurClip->fAnimationFrameTime)
	{
		m_pCurClip->fAnimationTime -= m_pCurClip->fAnimationFrameTime;

		++m_pCurClip->iFrameX;

			   
		if (m_pCurClip->iFrameX - m_pCurClip->iStartX == m_pCurClip->iLengthX)
		{
			m_pCurClip->iFrameX = m_pCurClip->iStartX;
			++m_pCurClip->iFrameY;

			if (m_pCurClip->eType == AT_FRAME)
				m_pObj->SetTexture(m_pCurClip->vecTexture[m_pCurClip->iFrameX]);

			if (m_pCurClip->iFrameY - m_pCurClip->iStartY == m_pCurClip->iLengthY)
			{
				m_pCurClip->iFrameY = m_pCurClip->iStartY;
				m_bMotionEnd = true;
				switch (m_pCurClip->eOption)
				{
				case AO_ONCE_RETURN:
					ChangeClip(m_strDefaultClip);
					break;
				case AO_ONCE_DESTROY:
					m_pObj->Die();
					break;
				case AO_TIME_RETURN:
					break;
				case AO_TIME_DESTROY:
					break;
				}
			}
		}
		else
		{
			if (m_pCurClip->eType == AT_FRAME)
				m_pObj->SetTexture(m_pCurClip->vecTexture[m_pCurClip->iFrameX]);
		}
	}
}

Animation * Animation::Clone()
{
	return new Animation(*this);
}

void Animation::SaveFromPath(const char * pFileName, const string & strPathKey)
{
}

void Animation::Save(FILE * pFile)
{
	size_t iLength = m_strTag.length();
	fwrite(&iLength, sizeof(iLength), 1, pFile);
	fwrite(m_strTag.c_str(), 1, iLength, pFile);

	size_t iCount = m_mapClip.size();
	fwrite(&iCount, sizeof(iCount), 1, pFile);

	unordered_map<string, PANIMATIONCLIP>::iterator	iter;
	unordered_map<string, PANIMATIONCLIP>::iterator	iterEnd = m_mapClip.end();

	for (iter = m_mapClip.begin(); iter != iterEnd; ++iter)
	{
		fwrite(&iter->second->eType, 4, 1, pFile);
		fwrite(&iter->second->eOption, 4, 1, pFile);
		iCount = iter->second->vecTexture.size();
		fwrite(&iCount, 4, 1, pFile);
		for (size_t i = 0; i < iter->second->vecTexture.size(); ++i)
		{
			iter->second->vecTexture[i]->Save(pFile);
		}
		fwrite(&iter->second->fAnimationLimitTime, 4, 1, pFile);
		fwrite(&iter->second->fAnimationFrameTime, 4, 1, pFile);
		fwrite(&iter->second->iFrameMaxY, 4, 1, pFile);
		fwrite(&iter->second->iFrameMaxX, 4, 1, pFile);
		fwrite(&iter->second->iStartX, 4, 1, pFile);
		fwrite(&iter->second->iStartY, 4, 1, pFile);
		fwrite(&iter->second->iLengthX, 4, 1, pFile);
		fwrite(&iter->second->iLengthY, 4, 1, pFile);
		fwrite(&iter->second->fOptionTime, 4, 1, pFile);
		fwrite(&iter->second->fOptionLimitTime, 4, 1, pFile);
		fwrite(&iter->second->tFrameSize, sizeof(_SIZE), 1, pFile);
	}

	iLength = m_strDefaultClip.length();
	fwrite(&iLength, 4, 1, pFile);
	fwrite(m_strDefaultClip.c_str(), 1, iLength, pFile);

	iLength = m_strCurClip.length();
	fwrite(&iLength, 4, 1, pFile);
	fwrite(m_strCurClip.c_str(), 1, iLength, pFile);
}

void Animation::LoadFromPath(const char * pFileName, const string & strPathKey)
{
}

void Animation::Load(FILE * pFile)
{
}
