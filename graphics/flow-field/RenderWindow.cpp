#include "RenderWindow.h"

#include <iostream>

RenderWindow::RenderWindow(HINSTANCE ins, Renderer* Render)
{
	if (!_ThisAtom)
	{
		RegisterAtom(ins);
		if (!_ThisAtom)
			throw std::logic_error("The ATOM could not be loaded.");
	}


	Window = CreateWindowEx(0l, MAKEINTATOM(_ThisAtom), TEXT("Flow: Output"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, ins, this);
	if (!Window)
		throw std::logic_error("The render window could not be resolved.");

	ShowWindow(Window, SW_SHOWNORMAL);
	UpdateWindow(Window);

	Bind(Render);
}
RenderWindow::~RenderWindow()
{
	Unbind();

	if (Window)
		DestroyWindow(Window);
}

ATOM RenderWindow::_ThisAtom = NULL;
void RenderWindow::RegisterAtom(HINSTANCE ins)
{
	if (_ThisAtom)
		return;

	WNDCLASS wn = { 0 };
	wn.lpfnWndProc = RenderWindow::WndProc;
	wn.lpszClassName = L"RenderWindow";
	wn.hCursor = LoadCursor(NULL, IDC_ARROW);
	wn.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wn.hInstance = ins;
	wn.cbClsExtra = 0;
	wn.cbWndExtra = sizeof(RenderWindow*);
	wn.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wn.lpszMenuName = NULL;
	wn.style = CS_HREDRAW | CS_VREDRAW;

	_ThisAtom = RegisterClass(&wn);
}
LRESULT __stdcall RenderWindow::WndProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp)
{
	if (Message == WM_CREATE)
	{
		LPCREATESTRUCT pcs = (LPCREATESTRUCT)lp;
		RenderWindow* Data = (RenderWindow*)pcs->lpCreateParams;

		SetWindowLongPtr(Window, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(Data));

		return 1;
	}
	else
	{
		RenderWindow* Data = reinterpret_cast<RenderWindow*>(GetWindowLongPtr(Window, GWLP_USERDATA));
		if (!Data)
			return DefWindowProc(Window, Message, wp, lp);

		switch (Message)
		{
		case WM_PAINT:
			Data->Paint();
			ValidateRect(Window, NULL);
			return 0;

		case WM_DESTROY:
			Data->Window = NULL;
			delete Data;
			SetWindowLongPtr(Window, GWLP_USERDATA, 0);
			PostQuitMessage(0);

			return 1;

		case WM_DISPLAYCHANGE:
		{
			InvalidateRect(Window, NULL, FALSE);
			return 0;
		}
		case WM_SIZE:
		{
			UINT width = LOWORD(lp);
			UINT height = HIWORD(lp);
			Data->Size(width, height);
			return 0;
		}
		default:
			return DefWindowProc(Window, Message, wp, lp);
		}
	}
}

void RenderWindow::Bind(Renderer* Render)
{
	Unbind();

	if (Render)
	{
		this->Render = Render;
		this->Render->Target = this;
		this->Render->Init();
	}
	else
		this->Render = nullptr;
}
void RenderWindow::Unbind()
{
	if (this->Render)
	{
		this->Render->DiscardDevResx();
		this->Render->Target = nullptr;
	}
}

void RenderWindow::Paint()
{
	if (this->Render)
	{
		this->Render->OnRender();
	}
	else
	{
		//Paint Mainually
		PAINTSTRUCT ps;
		HDC dc = BeginPaint(Window, &ps);

		RECT rc;
		GetClientRect(Window, &rc);

		HBRUSH Bk = CreateSolidBrush(0x0);
		FillRect(dc, &rc, Bk);

		HBRUSH Txt = CreateSolidBrush(0xFFFFFF);
		SelectObject(dc, Txt);

		DrawText(dc, L"No render target has been bound yet.", 37, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS | DT_NOPREFIX);

		DeleteObject(Txt);
		DeleteObject(Bk);
		EndPaint(Window, &ps);
	}
}
void RenderWindow::Command(WPARAM wp, LPARAM lp)
{
	
}
void RenderWindow::Size(UINT Width, UINT Height)
{
	if (Render)
		Render->OnResize(Width, Height);
}