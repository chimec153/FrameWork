#pragma once
#include "game.h"
class Core
{
private:
	static Core* m_pInst;
public:
	static Core* GetInst()
	{
		if (!m_pInst)
			m_pInst = new Core;
		return m_pInst;
	}
	static void DestroyInst()
	{
		SAFE_DELETE(m_pInst);
	}

private:
	Core();
	~Core();

private:
	static bool		m_bLoop;

private:
	HINSTANCE	m_hInst;
	HWND		m_hWnd;
	RESOLUTION	m_tRS;
	HDC			m_hDC;
	HBRUSH		m_hRedBrush;
	HBRUSH		m_hBlueBrush;
	HBRUSH		m_hOrangeBrush;

public:
	RESOLUTION GetResolution()	const
	{
		return m_tRS;
	}

	HWND GetWindowHandle() const
	{
		return m_hWnd;
	}

	HINSTANCE GetWindowInstance()	const
	{
		return m_hInst;
	}

	void DestroyGame()
	{
		DestroyWindow(m_hWnd);
	}

	HBRUSH GetBlueBrush()	const
	{
		return m_hBlueBrush;
	}

	HBRUSH GetRedBrush()	const
	{
		return m_hRedBrush;
	}

	HBRUSH GetOrangeBrush()	const
	{
		return m_hOrangeBrush;
	}

public:
	bool Init(HINSTANCE hInst);
	int Run();

private:
	void Logic();
	void Input(float fDeltaTime);
	int Update(float fDeltaTime);
	int LateUpdate(float fDeltaTime);
	void Collision(float fDeltaTime);
	void Render(float fDeltaTime);

private:
	ATOM MyRegisterClass();
	BOOL Create();

public:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};
