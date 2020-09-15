#pragma once
#include "UI.h"
class UIClockHand :
	public UI
{
private:
	friend class Obj;
	friend class Scene;

private:
	UIClockHand();
	UIClockHand(const UIClockHand& clockhand);
	~UIClockHand();

private:
	float				m_fTime;
	POSITION			m_tOriginPos;
	class Obj* m_pNightPanel;
	list<class Obj*>	m_CropList;
	list<class Obj*>	m_AnimalList;
	bool				m_bRain;
	int					m_iDay;
	int					m_iWeek;
	int					m_iMonth;
	float				m_fRainTime;
	float				m_fRainTimeMax;
	class Text*			m_pWeekText;
	class Text*			m_pTimeText;
	vector<TCHAR*>		m_vecWeekText;
	class UIPanel*		m_pWeatherPanel;
	class UIPanel*		m_pSeasonPanel;
	bool				m_bBackToHome;

public:
	void AddTime(float fTime);
	void AddDay();

	void SetTime(float fTime)
	{
		m_fTime = fTime;
	}

	float GetTime()	const
	{
		return m_fTime;
	}

	int GetDay()	const
	{
		return m_iDay;
	}

	void SetNightPanel(class Obj* pObj);
	void SetWeatherPanel(class UIPanel* pObj);
	void SetSeasonPanel(class UIPanel* pObj);

	void AddCrop(class Obj* pObj)
	{
		if (pObj)
		{
			pObj->AddRef();
			m_CropList.push_back(pObj);
		}
	}

	void DeleteCrop(class Obj* pObj)
	{
		auto iter = m_CropList.begin();
		auto iterEnd = m_CropList.end();

		for (; iter != iterEnd; ++iter)
		{
			if ((*iter) == pObj)
			{
				SAFE_RELEASE(pObj);
				m_CropList.erase(iter);
				return;
			}
		}
	}

	void AddAnimal(class Obj* pObj)
	{
		if (pObj)
		{
			pObj->AddRef();
			m_AnimalList.push_back(pObj);
		}
	}

	void DeleteAnimal(class Obj* pObj)
	{
		auto iter = m_AnimalList.begin();
		auto iterEnd = m_AnimalList.end();

		for (; iter != iterEnd; ++iter)
		{
			if ((*iter) == pObj)
			{
				SAFE_RELEASE(pObj);
				m_AnimalList.erase(iter);
				return;
			}
		}
	}

	bool IsRain()	const
	{
		return m_bRain;
	}

	void SetWeekText(Text* pText);
	void SetTimeText(Text* pText);

	void SetBackToHome(bool bHome)
	{
		m_bBackToHome = bHome;
	}

	bool IsBackToHome()	const
	{
		return m_bBackToHome;
	}

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual UIClockHand* Clone();

public:
	void AddObjectToLayer(class Layer* pLayer);
	void AddNightPanelToLayer(class Layer* pLayer);
};

