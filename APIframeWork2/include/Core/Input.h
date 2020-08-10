#pragma once
#include "../game.h"
typedef struct _tagKeyInfo
{
	string	strName;
	vector<DWORD>	vecKey;
	bool		bDown;
	bool		bPress;
	bool		bUp;

	_tagKeyInfo() :
		bDown(false),
		bPress(false),
		bUp(false)
	{
	}
}KEYINFO, *PKEYINFO;

class Input
{
private:
	HWND	m_hWnd;
	unordered_map<string, PKEYINFO> m_mapKey;
	PKEYINFO		m_pCreateKey;

	POINT		m_tMousePos;
	POINT		m_tMouseMove;
	class Mouse*	m_pMouse;

public:
	class Mouse* GetMouse()	const
	{
		return m_pMouse;
	}

	POSITION GetMouseClientPos()	const;
	POSITION GetMouseWorldPos()	const;
	POSITION GetMouseMove()	const;

public:
	bool Init(HWND hWnd);
	void Update(float fDeltaTime);
	bool KeyDown(const string& strKey) const;
	bool KeyPress(const string& strKey) const;
	bool KeyUp(const string& strKey) const;

public:
	template <typename T>
	bool AddKey(const T& data)
	{
		const char* pTType = typeid(T).name();
		if (strcmp(pTType, "char") == 0 ||
			strcmp(pTType, "int") == 0)
		{
			m_pCreateKey->vecKey.push_back((DWORD)data);
		}
		else
		{
			m_pCreateKey->strName = data;
			m_mapKey.insert(make_pair(m_pCreateKey->strName, m_pCreateKey));
		}
		return true;
	}

	template <typename T, typename ... Types>
	bool AddKey(const T& data, const Types& ...arg)
	{
		if (!m_pCreateKey)
			m_pCreateKey = new KEYINFO;

		const char* pTType = typeid(T).name();

		if (strcmp(pTType, "char") == 0 ||
			strcmp(pTType, "int") == 0)
		{
			m_pCreateKey->vecKey.push_back((DWORD)data);
		}
		else
		{
			m_pCreateKey->strName = data;
			m_mapKey.insert(make_pair(m_pCreateKey->strName, m_pCreateKey));
		}

		AddKey(arg...);

		if (m_pCreateKey)
			m_pCreateKey = NULL;

		return true;
	}
private:
	PKEYINFO FindKey(const string& strKey) const;
	DECLARE_SINGLE(Input)
};