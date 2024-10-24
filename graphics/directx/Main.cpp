#include "Main.h"

MainClass::MainClass()
{
	Instance = nullptr;
	MainWindowClass = ATOM();
}
MainClass::~MainClass()
{
	SetWindowLongPtr(Wnd, GWLP_USERDATA, 0);
	DestroyWindow(Wnd);
}

HRESULT MainClass::CreateDesktopWindow()
{
	if (!Instance)
		Instance = (HINSTANCE)GetModuleHandle(nullptr);

	HICON Icon = nullptr;
	{
		WCHAR ExePath[MAX_PATH];
		GetModuleFileName(nullptr, ExePath, MAX_PATH);

		Icon = ExtractIcon(Instance, ExePath, 0);
	}

	WNDCLASS wn = { 0 };
	wn.style = CS_DBLCLKS;
	wn.lpfnWndProc = StaticWindowProc;
	wn.cbClsExtra = 0;
	wn.cbWndExtra = sizeof(MainClass*);
	wn.hInstance = Instance;
	wn.hIcon = Icon;
	wn.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wn.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wn.lpszMenuName = nullptr;
	wn.lpszClassName = L"MainClass";

	if (!(MainWindowClass = RegisterClass(&wn)))
	{
		DWORD Error = GetLastError();
		if (Error != ERROR_CLASS_ALREADY_EXISTS)
			return HRESULT_FROM_WIN32(Error);
	}

	Rc = RECT{ 0,0,900,800 };
	AdjustWindowRect(&Rc, WS_OVERLAPPEDWINDOW, (Menu != nullptr) ? true : false);

	Wnd = CreateWindow(MAKEINTATOM(MainWindowClass), L"Cube", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, Rc.right - Rc.left, Rc.bottom - Rc.top, 0, Menu, Instance, 0);
	if (!Wnd)
	{
		DWORD Error = GetLastError();
		return HRESULT_FROM_WIN32(Error);
	}
	SetWindowLongPtr(Wnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

	return S_OK;
}
HRESULT MainClass::Run(std::shared_ptr<DeviceResources> Resx, std::shared_ptr<Renderer> Renderer)
{
	HRESULT hr = S_OK;

	this->Resx = Resx;
	this->Render = Renderer;

	if (!IsWindowVisible(Wnd))
		ShowWindow(Wnd, SW_SHOW);

	bool GotMsg;
	MSG msg = { 0 };
	msg.message = WM_NULL;
	PeekMessage(&msg, nullptr, 0U, 0U, PM_NOREMOVE);

	while (WM_QUIT != msg.message)
	{
		GotMsg = PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE) != 0;

		if (GotMsg)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Renderer->Update();
			Renderer->Render();
			Resx->Present();
		}
	}

	return hr;
}

LRESULT __stdcall MainClass::StaticWindowProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp)
{
	MainClass* Main = reinterpret_cast<MainClass*>(GetWindowLongPtr(Window, GWLP_USERDATA));

	switch (Message)
	{
	case WM_KEYDOWN:
	{
		switch (wp)
		{
		case VK_ESCAPE:
			PostMessage(Window, WM_CLOSE, 0, 0);
			break;
		}

		break;
	}
	case WM_CLOSE:
	{
		HMENU Menu = GetMenu(Window);
		if (Menu)
			DestroyMenu(Menu);
		
		SetWindowLongPtr(Window, GWLP_USERDATA, 0);
		DestroyWindow(Window);
		UnregisterClass(MAKEINTATOM(MainWindowClass), Instance);
		return 0;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(Window, Message, wp, lp);
}