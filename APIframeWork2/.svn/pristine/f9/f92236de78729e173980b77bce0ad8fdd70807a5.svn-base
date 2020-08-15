#include "Core.h"
#include "Scene/SceneManager.h"
#include "Core/Timer.h"
#include "Core/PathManager.h"
#include "Resources/ResourcesManager.h"
#include "Resources/Texture.h"
#include "Core/Camera.h"
#include "Core/Input.h"
#include "Collider/CollisionManager.h"
#include "Object/Mouse.h"
#include "Sound\SoundManager.h"

Core* Core::m_pInst = NULL;
bool Core::m_bLoop = true;

Core::Core()	:
	m_hWnd(nullptr),
	m_hInst(nullptr),
	m_hDC(nullptr),
	m_hRedBrush(0),
	m_hBlueBrush(0),
	m_hOrangeBrush(0)
{
	srand((unsigned int)time(0));
	rand();

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(483);

#ifdef _DEBUG
	//AllocConsole();
#endif 
}

Core::~Core()
{
	DESTROY_SINGLE(SceneManager);
	DESTROY_SINGLE(SoundManager);
	DESTROY_SINGLE(CollisionManager);
	DESTROY_SINGLE(Input);
	DESTROY_SINGLE(Camera);
	DESTROY_SINGLE(ResourcesManager);
	DESTROY_SINGLE(PathManager);
	DESTROY_SINGLE(Timer);

	ReleaseDC(m_hWnd, m_hDC);
#ifdef _DEBUG			//	디버그 모드일때
	//FreeConsole();	//	콘솔창을 해재한다.
#endif 

}

bool Core::Init(HINSTANCE hInst)
{
	m_hInst = hInst;

	MyRegisterClass();
	m_tRS.iW = 1280;
	m_tRS.iH = 800;

	Create();

	m_hDC = GetDC(m_hWnd);

	if (!GET_SINGLE(Timer)->Init(m_hWnd))
		return false;

	if (!GET_SINGLE(PathManager)->Init())
		return false;

	if (!GET_SINGLE(ResourcesManager)->Init(hInst,m_hDC))
		return false;

	if (!GET_SINGLE(Input)->Init(m_hWnd))
		return false;

	if (!GET_SINGLE(Camera)->Init(POSITION(0.f,0.f),
		m_tRS, RESOLUTION(1920,1080)))
		return false;

	if (!GET_SINGLE(SoundManager)->Init())
		return false;

	if (!GET_SINGLE(SceneManager)->Init())
		return false;

	m_hRedBrush = CreateSolidBrush(RGB(255, 0, 0));
	m_hBlueBrush = CreateSolidBrush(RGB(0, 0, 255));
	m_hOrangeBrush = CreateSolidBrush(RGB(255, 127, 0));

	return true;
}

int Core::Run()
{
	MSG msg = {};
	while (m_bLoop)
	{
		if (PeekMessage(&msg,nullptr,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Logic();
		}
	}
	return (int)msg.wParam;
}

void Core::Logic()
{
	GET_SINGLE(Timer)->Update();

	float	fDeltaTime = GET_SINGLE(Timer)->GetDeltaTime();

	Input(fDeltaTime);

	if(Update(fDeltaTime)==SC_CHANGE)
		return;

	if(LateUpdate(fDeltaTime)==SC_CHANGE)
		return;

	Collision(fDeltaTime);
	Render(fDeltaTime);
}

void Core::Input(float fDeltaTime)
{
	GET_SINGLE(Input)->Update(fDeltaTime);

	GET_SINGLE(SceneManager)->Input(fDeltaTime);
	GET_SINGLE(Camera)->Input(fDeltaTime);
}

int Core::Update(float fDeltaTime)
{
	SCENE_CHANGE	sc;
	sc = GET_SINGLE(SceneManager)->Update(fDeltaTime);
	GET_SINGLE(Camera)->Update(fDeltaTime);
	return sc;
}

int Core::LateUpdate(float fDeltaTime)
{
	SCENE_CHANGE	sc;
	sc = GET_SINGLE(SceneManager)->LateUpdate(fDeltaTime);
	return sc;
}

void Core::Collision(float fDeltaTime)
{
	GET_SINGLE(SceneManager)->Collision(fDeltaTime);
	GET_SINGLE(CollisionManager)->Collision(fDeltaTime);
}

void Core::Render(float fDeltaTime)
{
	Texture*	pBackBuffer = GET_SINGLE(ResourcesManager)->GetBackBuffer();

	GET_SINGLE(SceneManager)->Render(pBackBuffer->GetDC(),fDeltaTime);

	Mouse* pMouse = GET_SINGLE(Input)->GetMouse();

	pMouse->Render(pBackBuffer->GetDC(), fDeltaTime);

	POSITION tPos = pMouse->GetPos();

	BitBlt(m_hDC, 0, 0, m_tRS.iW, m_tRS.iH, pBackBuffer->GetDC(),
		0,0,SRCCOPY);
	SAFE_RELEASE(pBackBuffer)
}

ATOM Core::MyRegisterClass()
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = Core::WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_hInst;
	wcex.hIcon = LoadIcon(m_hInst, MAKEINTRESOURCE(IDI_ICON1));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;//MAKEINTRESOURCEW(IDC_WINDOWSPROJECT1);
	wcex.lpszClassName = L"AR20API";
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));

	return RegisterClassExW(&wcex);
}

BOOL Core::Create()
{
	m_hWnd = CreateWindowW(L"AR20API", L"AR20API", 	WS_OVERLAPPED |WS_SYSMENU | WS_CAPTION,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, m_hInst, nullptr);

	if (!m_hWnd)
	{
		return FALSE;
	}
	RECT	rc = { 0,0,(LONG)m_tRS.iW,(LONG)m_tRS.iH };
	AdjustWindowRect(&rc, WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION, FALSE);

	SetWindowPos(m_hWnd, HWND_TOPMOST, 100, 100, rc.right - rc.left,
		rc.bottom - rc.top, SWP_NOMOVE | SWP_NOZORDER);
	MoveWindow(m_hWnd, 100, 100, rc.right - rc.left, rc.bottom - rc.top, false);

	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	return TRUE;
}

LRESULT Core::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		m_bLoop = false;
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
