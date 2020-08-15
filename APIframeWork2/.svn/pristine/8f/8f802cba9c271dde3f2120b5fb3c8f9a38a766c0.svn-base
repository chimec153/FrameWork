#pragma once
#include "UI.h"

enum BUTTON_STATE
{
	BS_NONE,
	BS_MOUSEON,
	BS_CLICK
};

class UIButton :
	public UI
{
private:
	friend class Obj;
	friend class Scene;

private:
	UIButton();
	UIButton(const UIButton& ui);
	~UIButton();

private:
	function<void(float fTime)> m_BtnCallback;
	function<void(int, float)>		m_BtnCallback2;
	bool							m_bEnableCallback2;
	bool						m_bEnableCallback;
	BUTTON_STATE				m_eState;
	class Obj*					m_pParentUI;
	POSITION						m_tParentPrevPos;
	int								m_iButtonIndex;

public:
	void SetParent(class Obj* pObj);

	void SetIndex(int iIndex)
	{
		m_iButtonIndex = iIndex;
	}

public:
	template <typename T>
	void SetCallback(T* pObj, void (T::*pFunc)(float))
	{
		m_BtnCallback = bind(pFunc, pObj, placeholders::_1);
		m_bEnableCallback = true;
	}

	void SetCallback(void(*pFunc)(float))
	{
		m_BtnCallback = bind(pFunc, placeholders::_1);
		m_bEnableCallback = true;
	}

	template <typename T>
	void SetCallback(T* pObj, void (T::* pFunc)(int, float))
	{
		m_BtnCallback2 = bind(pFunc, pObj, placeholders::_1, placeholders::_2);
		m_bEnableCallback2 = true;
	}

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual UIButton* Clone();

public:
	void MouseOn(class Collider* pSrc,
		class Collider* pDest, float fDeltaTime);
	void MouseOut(class Collider* pSrc,
		class Collider* pDest, float fDeltaTime);
};
