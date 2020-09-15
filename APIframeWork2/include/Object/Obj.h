#pragma once

#include "../Ref.h"
#include "../Collider/Collider.h"
#include "../Scene/Layer.h"

class Obj	:
	public Ref
{
protected:
	friend class Scene;
	friend class Collider;
	friend class ColliderRect;

protected:
	Obj();
	Obj(const Obj& obj);
	virtual ~Obj();

private:
	static list<Obj*>	m_ObjList;

public:
	static void AddObj(Obj* pObj);
	static Obj* FindObject(const string& strTag);
	static void EraseObj(Obj* pObj);
	static void EraseObj(const string& strTag);
	static void EraseObj();

protected:
	class Scene* m_pScene;
	class Layer* m_pLayer;

public:
	void SetScene(class Scene* pScene)
	{
		m_pScene = pScene;
	}
	void SetLayer(class Layer* pLayer)
	{
		m_pLayer = pLayer;
	}
	class Scene* GetScene()	const
	{
		return m_pScene;
	}
	class Layer* GetLayer()	const
	{
		return m_pLayer;
	}

protected:
	bool		m_blsPhysics;
	float		m_fGravityTime;

public:
	void ClearGravity()
	{
		m_fGravityTime = 0.f;
	}
	void SetPhysics(bool bPhysics)
	{
		m_blsPhysics = bPhysics;
	}
protected:
	POSITION			m_tPos;
	_SIZE				m_tSize;
	_SIZE				m_tImageOffset;
	POSITION			m_tPivot;
	class Texture*		m_pTexture;
	list<Collider*>		m_ColliderList;
	class Animation*	m_pAnimation;
	unsigned char		m_cAlpha;
	bool				m_bAlphaOn;

protected:
	OBJ_BLOCK			m_eBlock;

public:
	OBJ_BLOCK GetBlock()	const
	{
		return m_eBlock;
	}

public:
	class Animation* CreateAnimation(const string& strTag);
	bool AddAnimationClip(const string& strName, ANIMATION_TYPE eType,
		ANIMATION_OPTION eOption, float fAnimationLimitTime,
		int iFrameMaxX, int iFrameMaxY, int iStartX, int iStartY,
		int iLengthX, int iLengthY, float fOptionLimitTime, 
		int iOptionFrame, const string& strTextKey, const wchar_t* pFileName,
		const string& strPathKey = TEXTURE_PATH); 
	bool AddAnimationClip(const string& strName, ANIMATION_TYPE eType,
		ANIMATION_OPTION eOption, float fAnimationLimitTime,
		int iFrameMaxX, int iFrameMaxY, int iStartX, int iStartY,
		int iLengthX, int iLengthY, float fOptionLimitTime, 
		int iOptionFrame, const string& strTextKey, const vector<wstring>& vecFileName,
		const string& strPathKey = TEXTURE_PATH);
	void SetAnimationCurrentClip(const string& strName);
	void SetAnimationDefaultClip(const string& strName);
	void SetAnimationClipColorKey(const string& strClip,
		unsigned char r, unsigned char g, unsigned char b);
	FrameInfo GetFrame(const string& strName)	const;

	class Texture* GetTexture()	const;


	const list<Collider*>* GetColliderList() const
	{
		return &m_ColliderList;
	}

	Collider* GetCollider(const string& strTag);
	
	template <typename T>
	void AddCollisionFunction(const string& strTag,
		COLLISION_STATE eState, T* pObj,
		void(T::*pFunc)(Collider*, Collider*, float))
	{/*
		list<Collider*>::iterator		iter;
		list<Collider*>::iterator		iterEnd = m_ColliderList.end();

		for (iter = m_ColliderList.begin(); iter != iterEnd; ++iter)
		{
			if ((*iter)->GetTag() == strTag)
			{
				(*iter)->AddCollisionFunction(eState, pObj, pFunc);
				break;
			}
		}*/
	}

	void SetAlpha(unsigned char cAlpha)
	{
		m_cAlpha = cAlpha;
	}

	void EnableAlpha(bool bAlpha)
	{
		m_bAlphaOn = bAlpha;
	}

	unsigned char GetAlpha()	const
	{
		return m_cAlpha;
	}

public:
	template <typename T>
	T* AddCollider(const string& strTag)
	{
		T* pCollider = new T;

		pCollider->SetObj(this);
		pCollider->SetTag(strTag);

		if (!pCollider->Init())
		{
			SAFE_RELEASE(pCollider);
			return NULL;
		}

		pCollider->AddRef();
		m_ColliderList.push_back(pCollider);

		return pCollider;
	}

	bool CheckCollider()
	{
		return !m_ColliderList.empty();
	}

	bool		m_bEnableCollide;

public:
	void SetCollide(bool bCollide)
	{
		m_bEnableCollide = bCollide;
	}

public:
	float GetLeft() const
	{
		return m_tPos.x - m_tSize.x * m_tPivot.x;
	}

	float GetRight()	const
	{
		return GetLeft() + m_tSize.x;
	}

	float GetTop() const
	{
		return m_tPos.y - m_tSize.y*m_tPivot.y;
	}

	float GetBottom() const
	{
		return GetTop() + m_tSize.y;
	}

	POSITION GetCenter()	const
	{
		return POSITION(GetLeft()+m_tSize.x/2.f, GetTop()+m_tSize.y/2.f);
	}
public:
	POSITION GetPos()	const
	{
		return m_tPos;
	}
	_SIZE GetSize()	const
	{
		return m_tSize;
	}
	POSITION GetPivot() const
	{
		return m_tPivot;
	}

public:
	void SetPos(const POSITION& tPos)
	{
		m_tPos = tPos;
	}
	void SetPos(const POINT& pt)
	{
		m_tPos = pt;
	}
	void SetPos(float x, float y)
	{
		m_tPos.x = x;
		m_tPos.y = y;
	}

	void SetSize(const _SIZE& tSize)
	{
		m_tSize = tSize;
	}
	void SetSize(float x, float y)
	{
		m_tSize.x = x;
		m_tSize.y = y;
	}

	void SetPivot(const _SIZE& tPivot)
	{
		m_tPivot = tPivot;
	}
	void SetPivot(float x, float y)
	{
		m_tPivot.x = x;
		m_tPivot.y = y;
	}
	void SetImageOffset(const _SIZE& tOffset)
	{
		m_tImageOffset = tOffset;
	}

	void SetImageOffset(float x, float y)
	{
		m_tImageOffset.x = x;
		m_tImageOffset.y = y;
	}

	const _SIZE& GetImageOffset()	const
	{
		return m_tImageOffset;
	}

public:
	void SetTexture(class Texture* pTexture);
	void SetTexture(const string& strKey,
		const wchar_t* pFileName = NULL,
		const string& strPathKey = TEXTURE_PATH);
	void SetTexture(const string& strKey,
		const vector<const TCHAR*>& vecFileName,
		const string& strPathKey = TEXTURE_PATH);
	void SetTexture(const string& strKey,
		const TCHAR* pFileName,
		int iCount, const string& strPathKey = TEXTURE_PATH);
	void SetColorKey(unsigned char r, unsigned char g, unsigned char b);

public:
	virtual bool Init()=0;
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual Obj* Clone() = 0;
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

public:
	void SaveToVector(vector<COLORREF>& vecColor, 
		int iFrame, int iStart, POSITION tPos);
	void SaveToVector(vector<COLORREF>& vecColor, 
		int iFrame, int iStart, int iOffset, POSITION tPos);

public:
	void SaveFromPath(const char* pFileName, const string& strPathKey = DATA_PATH);
	void SaveFromFullPath(const char* pFileName);
	void LoadFromPath(const char* pFileName, const string& strPathKey = DATA_PATH);
	void LoadFromFullPath(const char* pFileName);

public:
	template <typename T>
	static T* CreateObj(const string& strTag, class Layer* pLayer = NULL, 
		POSITION tPos = POSITION(0.f, 0.f), POSITION tSize = POSITION(100.f,100.f))
	{
		T* pObj = new T;

		pObj->SetPos(tPos);
		pObj->SetSize(tSize);

		if (!pObj->Init())
		{
			SAFE_RELEASE(pObj);
			return NULL;
		}

		if (pLayer)
		{
			pLayer->AddObject(pObj);
		}
		pObj->SetTag(strTag);
		AddObj(pObj);

		return pObj;
	}

	Obj* CreateCloneObj(const string& strPtototypeKey,
		const string& strTag, class Layer* pLayer=NULL);	

};