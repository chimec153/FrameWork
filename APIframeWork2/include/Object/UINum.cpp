#include "UINum.h"
#include "..//Core/Camera.h"
#include "../Core/Input.h"

UINum::UINum()	:
	m_iNum(0),
	m_fDist(0.f),
	m_fLimitDist(30.f),
	m_fSpeed(30.f),
	m_bOneLine(false)
{
}

UINum::UINum(const UINum& num)	:
	UI(num)
{
	m_iNum = num.m_iNum;
	m_fLimitDist = num.m_fLimitDist;
	m_fSpeed = num.m_fSpeed;
	m_fDist = 0.f;
	m_bOneLine = num.m_bOneLine;
	m_tOriginOffset = num.m_tOriginOffset;
}

UINum::~UINum()
{
	Safe_Release_VecList(m_vecNum);
}

void UINum::SetNum(int iNum)
{
	m_iNum = iNum % 10;

	if (m_iNum > 5 && !m_bOneLine)
		SetImageOffset(m_tOriginOffset.x + m_tSize.x * (m_iNum-6), m_tOriginOffset.y + m_tSize.y);

	else
		SetImageOffset(m_tOriginOffset.x + m_tSize.x * m_iNum, m_tOriginOffset.y);
}

void UINum::AddObjectToLayer(Layer* pLayer)
{
	pLayer->AddObject(this);

	size_t iSize = m_vecNum.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		if(!pLayer->HasObject(m_vecNum[i]))
			pLayer->AddObject(m_vecNum[i]);
	}
}

void UINum::DeleteObjectFromLayer(Layer* pLayer)
{
	pLayer->DeleteObject(this);

	size_t iSize = m_vecNum.size();

	for (size_t i = 0; i < iSize; ++i)
			pLayer->DeleteObject(m_vecNum[i]);
}

void UINum::DisableNumber()
{
	size_t iSize = m_vecNum.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		if (m_vecNum[i])
			m_vecNum[i]->SetEnable(false);
	}

	SetEnable(false);
}

void UINum::EnableNumber()
{
	size_t iSize = m_vecNum.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		if (m_vecNum[i])
			m_vecNum[i]->SetEnable(true);
	}

	SetEnable(true);
}

void UINum::SetPosAll(const POSITION& tPos)
{
	size_t iSize = m_vecNum.size();

	SetPos(tPos);

	for (size_t i = 0; i < iSize; ++i)
	{
		POSITION tNewPos = tPos;

		tNewPos.x -= m_tSize.x * (i +1);

		if(m_vecNum[i])
			m_vecNum[i]->SetPos(tNewPos);
	}
}

void UINum::CreateNum(int iNum)
{
	DeleteNum();

	int i = 1;

	SetNum(iNum);

	while (iNum >= 10)
	{
		UINum* pObj = nullptr;

		if(m_bOneLine)
			pObj = (UINum*)CreateCloneObj("NumSm", "num", m_pLayer);

		else
			pObj = (UINum*)CreateCloneObj("Num", "num", m_pLayer);
		
		if (m_fSpeed == 0.f)
		{
			if(pObj)
				pObj->SetSpeed(0.f);
		}
			

		POSITION tPos = m_tPos;

		tPos.x -= m_tSize.x * i;

		if (pObj)
		{
			pObj->SetPos(tPos);

			pObj->SetNum(iNum / 10);

			m_vecNum.push_back(pObj);
		}

		iNum /= 10;

		++i;
	}
}

void UINum::DeleteNum()
{

	auto iter = m_vecNum.begin();
	auto iterEnd = m_vecNum.end();

	for (; iter != iterEnd;)
	{
		(*iter)->Die();

		SAFE_RELEASE((*iter));

		iter = m_vecNum.erase(iter);
		iterEnd = m_vecNum.end();
	}

	m_vecNum.clear();
}

bool UINum::Init()
{
	if (!UI::Init())
		return false;

	m_tOriginOffset = m_tImageOffset;

	return true;
}

void UINum::Input(float fDeltaTime)
{
	UI::Input(fDeltaTime);
}

int UINum::Update(float fDeltaTime)
{
	UI::Update(fDeltaTime);

	m_fDist += m_fSpeed * fDeltaTime;
	m_tPos.y -= m_fSpeed * fDeltaTime;

	if (m_fDist >= m_fLimitDist)
	{
		Die();
	}

	return 0;
}

int UINum::LateUpdate(float fDeltaTime)
{
	UI::LateUpdate(fDeltaTime);
	return 0;
}

void UINum::Collision(float fDeltaTime)
{
	UI::Collision(fDeltaTime);
}

void UINum::Render(HDC hDC, float fDeltaTime)
{
	Obj::Render(hDC, fDeltaTime);

	size_t iSize = m_vecNum.size();

	for (size_t i = 0; i < iSize; ++i)
		m_vecNum[i]->Render(hDC, fDeltaTime);

#ifdef _DEBUG
	/*
	if (KEYPRESS("Debug"))
	{
		POSITION tPos = m_tPos - GET_SINGLE(Camera)->GetPos();

		TCHAR strHP[32] = {};

		wsprintf(strHP, TEXT("Layer: %p"), m_pLayer);
		TextOut(hDC, (int)tPos.x, (int)tPos.y, strHP, lstrlen(strHP));
		wsprintf(strHP, TEXT("Scene: %p"), m_pScene);
		TextOut(hDC, (int)tPos.x, (int)tPos.y - 20, strHP, lstrlen(strHP));
	}*/
#endif
}

UINum* UINum::Clone()
{
	return new UINum(*this);
}
