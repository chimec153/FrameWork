#pragma once
// 애니메이션 한 장당 시간, 프레임 x 개수, y 개수, 파일이름, 컬러키값
#include "Scene.h"

enum ANIMATION_PART
{
	AP_BASE,
	AP_UNDERWEAR,
	AP_HAIR,
	AP_SHIRT,
	AP_ARM,
	AP_END
};

typedef struct _tagAnimationInfo
{
	ANIMATION_EDIT_MODE eType;
	float	fAnimationTime;
	int		iLengthX;
	int		iLengthY;
	TCHAR	strFilename[MAX_PATH];
	COLORREF	tColorKey;
	vector<COLORREF> vecPixel;
	int		iPos;

	_tagAnimationInfo()
	{
		eType = AEM_BODY;
		fAnimationTime = 0.f;
		iLengthX = 6;
		iLengthY = 1;
		memset(strFilename, 0, sizeof(TCHAR) * MAX_PATH);
		tColorKey = COLORREF();
		vecPixel.resize(32 * 64 * 6);
		memset(&vecPixel[0], 0, sizeof(PIXEL) * 32 * 64 * 6);
		iPos = 0;
	}
}AniInfo, *PAniInfo;

class CAnimationEditScene :
	public Scene
{
private:
	friend class SceneManager;

private:
	class Stage*						m_pStage;
	POSITION							m_tPos;
	float								m_fAnimationTime;
	float								m_fLimitTime;
	int									m_iFrame;
	int									m_iEditMode;
	POSITION							m_tHaitCopyPos;
	POSITION							m_tShirtCopyPos;
	int									m_iDir;
	static class UIPanel*				m_pBody;
	class UIPanel*						m_pHair;
	class UIPanel*						m_pShirt;
	vector<COLORREF>					m_vecPixel;
	vector<COLORREF>					m_vecUnderwear;
	vector<COLORREF>					m_vecArm;
	vector<COLORREF>					m_vecHair;
	vector<COLORREF>					m_vecShirt;
	static TCHAR						m_strText[MAX_PATH];
	static PAniInfo						m_pAniInfo;
	static vector<vector<COLORREF>>		m_vecvecPixel[AP_END];
	static int							m_iBasePos;
	static int							m_iUnderwearPos;
	static int							m_iHairPos;
	static int							m_iShirtPos;
	static int							m_iArmPos;
	class CAniEditDlg*					m_pAniEditDlg;

private:
	CAnimationEditScene();
	~CAnimationEditScene();

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);

public:
	static INT_PTR CALLBACK DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static INT_PTR CALLBACK DlgProc2(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static void AnimationSave(const TCHAR* pFilePath,const vector<COLORREF>& vecCOLOR);

public:
	void OnEditDlg();

};

