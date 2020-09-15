#pragma once
#include "Item.h"
class Crop :
	public Item
{
private:
	friend class Obj;
	friend class Scene;

protected:
	Crop();
	Crop(const Crop& crop);
	virtual ~Crop();

protected:
	CROP_TYPE		m_eCropType;
	int				m_iDay;
	int				m_iStage;
	int				m_iMaxStage;
	vector<int>		m_vecMaxDay;
	bool			m_bTimeStart;	//	작물이 성장을 시작했는지 나타낸다.
	bool			m_bHarvested;
	int				m_iHP;
	int				m_iEnergy;
	int				m_iRegrowthDay;
	class Crop*		m_pFlower;
	bool			m_bFrozen;

public:
	void SetCropType(CROP_TYPE eType)
	{
		m_eCropType = eType;
	}

	CROP_TYPE GetCropType()	const
	{
		return m_eCropType;
	}

	void TimeStart()
	{
		m_bTimeStart = true;
		Grow();
	}

	bool IsStart()	const
	{
		return m_bTimeStart;
	}

	void AddDay(int iDay);
	void Grow();

	int GetStage()	const
	{
		return m_iStage;
	}

	void Harvest();

	bool IsHarvested()	const
	{
		return m_bHarvested;
	}

	void AddMaxDay(int iDay)
	{
		m_vecMaxDay.push_back(iDay);
	}

	void SetMaxStage(int iStage)
	{
		m_iMaxStage = iStage;
	}

	int GetMaxStage()	const
	{
		return m_iMaxStage;
	}

	bool CanHarvest()	const
	{
		return m_iMaxStage <= m_iStage;
	}

	void SetHP(int iHP)
	{
		m_iHP = iHP;
	}

	void SetEnergy(int iEnergy)
	{
		m_iEnergy = iEnergy;
	}

	int GetHP()	const
	{
		return m_iHP;
	}

	int GetEnergy()	const
	{
		return m_iEnergy;
	}

	void SetRegrowthDay(int iDay)
	{
		m_iRegrowthDay = iDay;
	}

	int GetRegrowthDay()	const
	{
		return m_iRegrowthDay;
	}

	bool IsFrozen()	const
	{
		return m_bFrozen;
	}

	void SetFrozen(bool bFrozen)
	{
		m_bFrozen = bFrozen;
	}

public:
	virtual bool Init();
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual Crop* Clone();

public:
	void MouseOn(class Collider* pSrc, class Collider* pDest, float fTime);
	void MousePress(class Collider* pSrc, class Collider* pDest, float fTime);
	void MouseOff(class Collider* pSrc, class Collider* pDest, float fTime);
};

