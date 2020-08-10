#pragma once

#define SAFE_DELETE(p) if(p) {delete p;p=NULL;}
#define SAFE_DELETE_ARRAY(p)	if(p) {delete[] p; p=NULL;}
#define SAFE_RELEASE(p)	if(p)	{p->Release();p=NULL;}


#define DECLARE_SINGLE(Type)	\
	private:\
		static Type*	m_pInst;\
	public:\
		static Type*	GetInst()\
		{\
			if (!m_pInst)\
				m_pInst = new Type; \
			return m_pInst;\
		}\
		static void DestroyInst()\
		{\
			SAFE_DELETE(m_pInst);\
		}\
	private:\
		Type();\
		~Type();\

#define DEFINITION_SINGLE(Type) Type* Type::m_pInst = NULL;
#define GET_SINGLE(Type)	Type::GetInst()
#define DESTROY_SINGLE(Type)	Type::DestroyInst()

#define GETRESOLUTION	Core::GetInst()->GetResolution()
#define WINDOWHANDLE	Core::GetInst()->GetWindowHandle()
#define WINDOWINSTANCE	Core::GetInst()->GetWindowInstance()
#define BLUEBRUSH		Core::GetInst()->GetBlueBrush()
#define REDBRUSH		Core::GetInst()->GetRedBrush()
#define ORANGEBRUSH		Core::GetInst()->GetOrangeBrush()

#define KEYDOWN(key)	Input::GetInst()->KeyDown(key)
#define KEYPRESS(key)	Input::GetInst()->KeyPress(key)
#define KEYUP(key)		Input::GetInst()->KeyUp(key)
#define KEYKEY(key)		KEYDOWN(key) || KEYPRESS(key)
#define MOUSECLINETPOS	Input::GetInst()->GetMouseClientPos()
#define MOUSEWORLDPOS	Input::GetInst()->GetMouseWorldPos()
#define MOUSEMOVE		Input::GetInst()->GetMouseMove()
#define MOUSE			Input::GetInst()->GetMouse()

#define INVENTORY_MAX	36

#define _CRTDBG_MAP_ALLOC

#ifdef _DEBUG
#define new new (_NORMAL_BLOCK, __FILE__, __LINE__)
#endif