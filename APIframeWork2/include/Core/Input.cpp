#include "Input.h"
#include "../Object/Mouse.h"
#include "../Animation/Animation.h"
#include "../Scene/Layer.h"
#include "../Collider/CollisionManager.h"
#include "../Scene/SceneManager.h"

DEFINITION_SINGLE(Input)
Input::Input()	:
	m_pCreateKey(NULL),
	m_pMouse(NULL),
	m_tMousePos(),
	m_tMouseMove(),
	m_hWnd(nullptr)
{
}

Input::~Input()
{
	Obj::EraseObj(m_pMouse);
	SAFE_RELEASE(m_pMouse);
	Safe_Delete_Map(m_mapKey);
}

POSITION Input::GetMouseClientPos() const
{
	return m_pMouse->GetPos();
}

POSITION Input::GetMouseWorldPos() const
{
	return m_pMouse->GetWorldPos();
}

POSITION Input::GetMouseMove() const
{
	return m_pMouse->GetMove();
}

bool Input::Init(HWND hWnd)
{
	m_hWnd = hWnd;

	AddKey('W', "MoveFront");
	AddKey('S', "MoveBack");
	AddKey('A', "MoveLeft");
	AddKey('D', "MoveRight");
	AddKey('E', "Inventory");
	AddKey( 'C', "Key_C");
	AddKey('E', VK_CONTROL, "Key_E_CTRL");
	AddKey("Key_D", 'D');
	AddKey("Fire", VK_SPACE);
	AddKey("NextFrame", 'N', VK_CONTROL);
	AddKey(VK_CONTROL, "Skill_First", '1');
	AddKey(VK_LBUTTON, "MouseLButton");
	AddKey(VK_LBUTTON, VK_CONTROL, "CtrlLButton");
	AddKey(VK_RBUTTON, "MouseRButton");
	AddKey("TileSub", '1');
	AddKey("TileAdd", '2');
	AddKey("TileSetChange", '3');
	AddKey("4", '4');
	AddKey("5", '5');
	AddKey("6", '6');
	AddKey("7", '7');
	AddKey("8", '8');
	AddKey("9", '9');
	AddKey("10", '0');
	AddKey("11", VK_OEM_MINUS);
	AddKey("12", VK_OEM_PLUS);
	AddKey("Debug", VK_TAB);
	AddKey("PrevScene", VK_ESCAPE);

	AddKey("FocusUp", VK_UP);
	AddKey("FocusLeft", VK_LEFT);
	AddKey("FocusRight", VK_RIGHT);
	AddKey("FocusDown", VK_DOWN);

	AddKey("CtrlFocusUp", VK_UP,VK_CONTROL);
	AddKey("CtrlFocusLeft", VK_LEFT,VK_CONTROL);
	AddKey("CtrlFocusRight", VK_RIGHT,VK_CONTROL);
	AddKey("CtrlFocusDown", VK_DOWN,VK_CONTROL);

	AddKey("Save", 'S', VK_CONTROL);
	AddKey("Load", 'O', VK_CONTROL);
	AddKey("MapEditDlg", 'E', VK_CONTROL);

	AddKey("PixelUp", 'T');
	AddKey("PixelDown", 'G');
	AddKey("PixelLeft", 'F');
	AddKey("PixelRight", 'H');

	AddKey("CtrlPixelUp", 'T',VK_CONTROL);
	AddKey("CtrlPixelDown", 'G',VK_CONTROL);
	AddKey("CtrlPixelLeft", 'F',VK_CONTROL);
	AddKey("CtrlPixelRight", 'H',VK_CONTROL);

	AddKey("Hat", VK_F1);
	AddKey("SeasonNext", VK_F2);
	AddKey("SetGoToHome", VK_F3);
	AddKey("TimeFaster", VK_F4);
	AddKey("TimeSlower", VK_F5);
	AddKey("TimeOrigin", VK_F6);
	AddKey("CarpenterShop", VK_F7);
	AddKey("Shop", VK_F8);
	AddKey("WeaponShop", VK_F9);
	AddKey("Coop", VK_F10);
	AddKey("NewGame", VK_F11);

	GetCursorPos(&m_tMousePos);

	m_pMouse = Obj::CreateObj<Mouse>("Mouse");

	m_pMouse->SetSize(32, 32);

	m_pMouse->SetTexture("Mouse", L"UI/Cursors.bmp");
	m_pMouse->SetColorKey(255, 255, 255);
	/*
	Animation* pAni = m_pMouse->CreateAnimation("PlayerAnimation");

	vector<wstring>	vecFileName;

	for (int i = 1; i <= 9; ++i)
	{
		wchar_t strFileName[MAX_PATH] = {};
		wsprintf(strFileName, L"Mouse/%d.bmp", i);
		vecFileName.push_back(strFileName);
	}

	m_pMouse->AddAnimationClip("MouseNormal", AT_FRAME, AO_LOOP, 0.8f, 9, 1,
		0, 0, 9, 1, 0.f, "MouseNormal",
		vecFileName);

	m_pMouse->SetAnimationClipColorKey("MouseNormal", 0, 96, 0);

	SAFE_RELEASE(pAni);
	*/
	return true;
}

void Input::Update(float fDeltaTime)
{
	unordered_map<string, PKEYINFO>::iterator	iter;
	unordered_map<string, PKEYINFO>::iterator	iterEnd = m_mapKey.end();

	for (iter = m_mapKey.begin(); iter != iterEnd; ++iter)
	{
		int iPushCount = 0;
		for (size_t i = 0; i < iter->second->vecKey.size(); ++i)
		{
			if (GetAsyncKeyState(iter->second->vecKey[i]) & 0x8000)
				++iPushCount;
		}

		if (iPushCount == iter->second->vecKey.size())
		{
			if (!iter->second->bDown && !iter->second->bPress)
				iter->second->bDown = true;
			else if (iter->second->bDown && !iter->second->bPress)
			{
				iter->second->bPress = true;
				iter->second->bDown = false;
			}
		}
		else
		{
			if (iter->second->bDown || iter->second->bPress)
			{
				iter->second->bUp = true;
				iter->second->bDown = false;
				iter->second->bPress = false;
			}
			else if (iter->second->bUp)
				iter->second->bUp = false;
		}
	}

	m_pMouse->Update(fDeltaTime);
	m_pMouse->LateUpdate(fDeltaTime);

	m_pMouse->SetScene(GET_SINGLE(SceneManager)->GetScene());

	m_pMouse->Collision(fDeltaTime);
	GET_SINGLE(CollisionManager)->AddObject(m_pMouse);
}

bool Input::KeyDown(const string & strKey) const
{
	PKEYINFO	pInfo = FindKey(strKey);

	if (!pInfo)
		return false;
	return pInfo->bDown;
}

bool Input::KeyPress(const string& strKey) const
{
	PKEYINFO	pInfo = FindKey(strKey);

	if (!pInfo)
		return false;
	return pInfo->bPress;
}

bool Input::KeyUp(const string& strKey) const
{
	PKEYINFO	pInfo = FindKey(strKey);

	if (!pInfo)
		return false;
	return pInfo->bUp;
}

PKEYINFO Input::FindKey(const string& strKey) const
{
	unordered_map<string, PKEYINFO>::const_iterator	iter = m_mapKey.find(strKey);
	if (iter == m_mapKey.end())
		return NULL;
	
	return iter->second;
}
