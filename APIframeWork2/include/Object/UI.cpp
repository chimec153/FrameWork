#include "UI.h"
#include "../Animation/Animation.h"
#include "../Resources/Texture.h"

UI::UI()
{
}

UI::UI(const UI & ui)	:
	Obj(ui)
{
}

UI::~UI()
{
}

bool UI::Init()
{
	return true;
}

void UI::Input(float fDeltaTime)
{
	Obj::Input(fDeltaTime);
}

int UI::Update(float fDeltaTime)
{
	Obj::Update(fDeltaTime);
	return 0;
}

int UI::LateUpdate(float fDeltaTime)
{
	Obj::LateUpdate(fDeltaTime);
	return 0;
}

void UI::Collision(float fDeltaTime)
{
	Obj::Collision(fDeltaTime);
}

void UI::Render(HDC hDC, float fDeltaTime)
{
	if (m_pTexture)
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

		if (m_pTexture->GetColorKeyEnable())
		{
			TransparentBlt(hDC, (int)m_tPos.x, (int)m_tPos.y, (int)m_tSize.x,
				(int)m_tSize.y, m_pTexture->GetDC(), (int)tImagePos.x, (int)tImagePos.y,
				(int)m_tSize.x, (int)m_tSize.y, m_pTexture->GetColorKey());
		}
		else
		{
			BitBlt(hDC, (int)m_tPos.x, (int)m_tPos.y, (int)m_tSize.x,
				(int)m_tSize.y, m_pTexture->GetDC(), (int)tImagePos.x, (int)tImagePos.y,
				SRCCOPY);
		}
	}
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
