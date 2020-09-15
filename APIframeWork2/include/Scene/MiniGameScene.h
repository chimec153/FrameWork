#pragma once
#include "Scene.h"
class MiniGameScene :
	public Scene
{
	friend class SceneManager;

private:
	MiniGameScene();
	virtual ~MiniGameScene();

private:
	class PrairieKing*		m_pKing;
	float					m_fTime;
	float					m_fGenTime;
	float					m_fGenLimit;
	float					m_fFiverTime;
	float					m_fFiverTimeLimit;
	bool					m_bFiver;
	float					m_fReadyTime;
	float					m_fStartTime;
	bool					m_bGenStart;
	float					m_fFiverEndTime;
	float					m_fFiverEndTimeLimit;
	class UIBar*			m_pTimeBar;
	MINI_ITEM_TYPE			m_eItemType;
	class UIPanel*			m_pItemPanel;
	class Text*				m_pMoneyText;
	class Text*				m_pLifeText;
	int						m_iStage;
	int						m_iMonsterCount;

public:
	class Obj* GetKing();
	void SetKing(class PrairieKing* pKing);
	void SetItemType(MINI_ITEM_TYPE eItemType)
	{
		m_eItemType = eItemType;
	}

	MINI_ITEM_TYPE GetItemType()	const
	{
		return m_eItemType;
	}

	void SetStage(int iStage);

	void AddMonsterCount(int iCount)
	{
		m_iMonsterCount += iCount;
	}

public:
	virtual bool Init();
	virtual void Start();
	virtual void Input(float fTime);
	virtual int Update(float fTime);

public:
	void ColPortal(class Collider* pSrc, class Collider* pDest, float fTime);
	
};

