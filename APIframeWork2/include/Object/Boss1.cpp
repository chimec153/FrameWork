#include "Boss1.h"
#include "../Collider/ColliderRect.h"
#include "UIPanel.h"
#include "../Animation/Animation.h"

Boss1::Boss1()	:
	m_pScriptPanel(nullptr),
	m_fScriptTime(3.f)
{
}

Boss1::Boss1(const Boss1& boss)	:
	MiniGameMonster(boss)
{
	m_pScriptPanel = boss.m_pScriptPanel;
	m_fScriptTime = boss.m_fScriptTime;
}

Boss1::~Boss1()
{
	SAFE_RELEASE(m_pScriptPanel);
}

bool Boss1::Init()
{
	if (!MiniGameMonster::Init())
		return false;

	Animation* pAni = CreateAnimation("BossAni");

	pAni->LoadFromPath("minigameboss.sac");

	SAFE_RELEASE(pAni);

	return true;
}

int Boss1::Update(float fDeltaTime)
{
	MiniGameMonster::Update(fDeltaTime);

	if (!m_pScriptPanel)
	{
		m_pScriptPanel = CreateObj<UIPanel>("Script", m_pLayer);

		m_pScriptPanel->SetSize(64.f, 64.f);

		m_pScriptPanel->SetTexture("Mouse");

		m_pScriptPanel->SetImageOffset(1152.f, 3584.f);

		POSITION tSize = GetSize();

		m_pScriptPanel->SetPos(m_tPos.x - tSize.x, m_tPos.y - tSize.y * 2.5f);
	}

	if (m_pScriptPanel)
	{
		m_fScriptTime -= fDeltaTime;

		if (m_fScriptTime <= 0.f)
		{
			m_pScriptPanel->Die();

			SAFE_RELEASE(m_pScriptPanel);
		}
	}

	return 0;
}

int Boss1::LateUpdate(float fDeltaTime)
{
	MiniGameMonster::LateUpdate(fDeltaTime);
	return 0;
}

void Boss1::Collision(float fDeltaTime)
{
	MiniGameMonster::Collision(fDeltaTime);
}

void Boss1::Render(HDC hDC, float fDeltaTime)
{
	MiniGameMonster::Render(hDC, fDeltaTime);
}

Boss1* Boss1::Clone()
{
	return new Boss1(*this);
}

void Boss1::CollisionBullet(Collider* pSrc, Collider* pDest, float fDeltaTime)
{
}

void Boss1::CollStay(Collider* pSrc, Collider* pDest, float fDeltaTime)
{
}

void Boss1::CollLeave(Collider* pSrc, Collider* pDest, float fDeltaTime)
{
}
