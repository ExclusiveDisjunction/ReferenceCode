#include "Welcome.h"

#include "..\UICommon.h"

namespace Mason::UI
{
	using namespace Core;
	using namespace Controls;
	using namespace Graph;

	Welcome::Welcome(HINSTANCE ins)
	{
		if (!_ThisAtom)
			InitBase(ins);

		const int ScreenWidth = GetSystemMetrics(SM_CXSCREEN), ScreenHeight = GetSystemMetrics(SM_CYSCREEN);

		const int Width = (ScreenWidth > 1100 ? 900 : static_cast<int>(900 * (ScreenWidth / 1100.0)));
		const int Height = (ScreenWidth > 800 ? 650 : static_cast<int>(800 * (ScreenWidth / 800.0)));
		const int X = (ScreenWidth / 2) - (Width / 2);
		const int Y = (ScreenHeight / 2) - (Height / 2);

		_Base = CreateWindowExW(0, MAKEINTATOM(_ThisAtom), L"Mason Welcome", WS_VISIBLE | WS_POPUP, X, Y, Width, Height, nullptr, nullptr, ins, nullptr);
		SetWindowLongPtrW(_Base, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

		LoadControls();

		ShowWindow(_Base, SW_NORMAL);
		UpdateWindow(_Base);
	}
	Welcome::~Welcome()
	{

	}

	ATOM Welcome::_ThisAtom = ATOM();
	void Welcome::InitBase(HINSTANCE ins)
	{
		WNDCLASSW wn = { 0 };
		wn.cbClsExtra = 0;
		wn.cbWndExtra = sizeof(Welcome*);
		wn.hInstance = ins;
		wn.hIcon = MasonIcon;
		wn.hCursor = MasonCursor;
		wn.hbrBackground = nullptr;
		wn.lpszMenuName = nullptr;
		wn.lpszClassName = L"WelcomeWindowClass";
		wn.lpfnWndProc = Welcome::WndProc;
		wn.style = CS_HREDRAW | CS_VREDRAW;

		_ThisAtom = RegisterClassW(&wn);
	}

	LRESULT Welcome::WndProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp)
	{
		void* FetchedData = reinterpret_cast<void*>(GetWindowLongPtrW(Window, GWLP_USERDATA));
		if (!FetchedData)
			return DefWindowProcW(Window, Message, wp, lp);

		Welcome* Obj = reinterpret_cast<Welcome*>(FetchedData);

		switch (Message)
		{
		case WM_PAINT:
			return Obj->Paint();
		case WM_DESTROY:
		{
			SetWindowLongPtrW(Window, GWLP_USERDATA, 0);
			delete Obj;
			return DefWindowProcW(Window, Message, wp, lp);
		}
		case WM_COMMAND:
			return Obj->Command(wp, lp);
		default:
			return DefWindowProcW(Window, Message, wp, lp);
		}
	}

	void Welcome::LoadControls()
	{
		HINSTANCE ins = reinterpret_cast<HINSTANCE>(GetWindowLongPtrW(_Base, GWLP_HINSTANCE));
		RECT WndRect;
		GetClientRect(_Base, &WndRect);

		GraphControlRect = { 0, 0, WndRect.right, static_cast<LONG>(WndRect.bottom * (3.6 / 7.0)) };

		RECT TitleArea = { 0,GraphControlRect.bottom + 7, WndRect.right, static_cast<LONG>((WndRect.bottom - GraphControlRect.bottom) * (1.0 / 7.0)) };
		
		TextStyleSheet Style;
		Style.FontFamily = L"Candara";
		Style.FontSize = 20;
		Style.Foreground = FontColor;
		Style.Alignment = TA_CenterAlignment;
		
		TitleSplash = new Label(TitleArea.left, TitleArea.top, TitleArea.right, TitleArea.bottom - 20, _Base, ins, L"Mason " + Version, Background1, Style, false);
	}

	LRESULT Welcome::Paint()
	{
		PAINTSTRUCT ps;
		HDC dc = BeginPaint(_Base, &ps);

		RECT WndRect;
		GetClientRect(_Base, &WndRect);

		HBRUSH Brush = CreateSolidBrush(Background1);
		FillRect(dc, &WndRect, Brush);
		
		DeleteObject(Brush);
		Brush = CreateSolidBrush(Grey3);
		FillRect(dc, &GraphControlRect, Brush);

		DeleteObject(Brush);
		EndPaint(_Base, &ps);
		return 0;
	}
	LRESULT Welcome::Command(WPARAM wp, LPARAM lp)
	{
		return 0;
	}

	void Welcome::Open()
	{
		MSG msg;
		int Result;

		while ((Result = GetMessageW(&msg, NULL, NULL, NULL)) != 0)
		{
			if (Result < 0)
				continue;

			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}
}