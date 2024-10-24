#include "PasswordInput.h"

#include "..\..\UICommon.h"

#include <thread>
#include <chrono>
using namespace std;

namespace Mason::UI::Tool
{
	using namespace Core;
	using namespace Controls;

	PasswordInput::PasswordInput()
	{		
	}
	void PasswordInput::Construct(HINSTANCE ins)
	{
		if (!_ThisAtom)
			InitBase(ins);

		_Base = CreateWindowExW(0l, MAKEINTATOM(_ThisAtom), L"Password Input", WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, 600, 250, NULL, NULL, ins, NULL);
		SetWindowLongPtrW(_Base, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

		ShowWindow(_Base, SW_NORMAL);
		UpdateWindow(_Base);

		LoadControls();
	}

	String PasswordInput::Execute(HINSTANCE ins)
	{
		PasswordInput* Obj = new PasswordInput();

		WindowState* Running = new WindowState(true);
		thread Thread = thread(RunMessageLoop, Obj, ins, Running);
		while ((*Running))
			this_thread::sleep_for(chrono::milliseconds(100));

		Thread.detach();
		delete Running;
		String Return = Obj->Return;
		delete Obj;

		return Return;
	}
	LRESULT PasswordInput::RunMessageLoop(PasswordInput* Object, HINSTANCE ins, WindowState* _Running)
	{
		Object->Construct(ins);
		WindowState& Running = *_Running;
		Running = true;

		int Result;
		MSG msg;
		while ((Result = GetMessageW(&msg, nullptr, 0, 0)) != 0)
		{
			if (Result < 0)
				break;
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}

		Running = false;
		return msg.wParam;
	}

	ATOM PasswordInput::_ThisAtom = ATOM();
	void PasswordInput::InitBase(HINSTANCE ins)
	{
		WNDCLASSW wn = { 0 };
		wn.lpfnWndProc = PasswordInput::WndProc;
		wn.hInstance = ins;
		wn.hCursor = MasonCursor;
		wn.hbrBackground = NULL;
		wn.lpszMenuName = NULL;
		wn.lpszClassName = L"PasswordInputClass";
		wn.hIcon = MasonIcon;
		wn.cbClsExtra = 0;
		wn.cbWndExtra = sizeof(PasswordInput*);
		wn.style = CS_HREDRAW | CS_VREDRAW;

		_ThisAtom = RegisterClassW(&wn);
	}
	LRESULT __stdcall PasswordInput::WndProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp)
	{
		PasswordInput* This = reinterpret_cast<PasswordInput*>(GetWindowLongPtrW(Window, GWLP_USERDATA));
		if (!This)
			return DefWindowProcW(Window, Message, wp, lp);

		switch (Message)
		{
		case WM_PAINT:
			return This->Paint();
		case WM_KEYDOWN:
			return This->KeyDown(wp);
		case WM_COMMAND:
			return This->Command(wp, lp);
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		default:
			return DefWindowProcW(Window, Message, wp, lp);
		}
	}

	void PasswordInput::LoadControls()
	{
		RECT WndRect;
		GetClientRect(_Base, &WndRect);
		HINSTANCE ins = reinterpret_cast<HINSTANCE>(GetWindowLongPtrW(_Base, GWLP_HINSTANCE));

		TextStyleSheet TextStyle;
		StyleSheet Style;
		Color BaseBk = Grey1;

		{
			TextStyle.FontFamily = StandardFontName;
			TextStyle.Foreground = FontColor;
			TextStyle.FontSize = 13;
			
			Style.Background = Grey3;
			Style.BaseBackground = BaseBk;
			Style.BorderBrush = Accent3;
			Style.BorderThickness = 3;
			Style.Radius = 20;
		}

		{
			int XCoord = 10;
			int YCoord = 10;
			int Width = WndRect.right - (10 + XCoord);
			int Height = 27;

			MiscControls.Add(new Label(XCoord, YCoord, Width, Height, _Base, ins, L"Please input the password for this project.", BaseBk, TextStyle, false));
			YCoord += 20 + Height;

			Password = new TextBox(XCoord, YCoord, Width, Height, _Base, ins, String(), Style, TextStyle, false, true);
			YCoord += 20 + Height;

			Width = 130;
			XCoord = (WndRect.right / 2) - (Width + 5);

			Submit = new Button(XCoord, YCoord, Width, Height, L"Submit", _Base, (HMENU)1, ins, Style, TextStyle);
			XCoord += 10 + Width;

			Close = new Button(XCoord, YCoord, Width, Height, L"Cancel", _Base, (HMENU)2, ins, Style, TextStyle);
		}

		SetFocus(*Password);
	}

	LRESULT PasswordInput::Paint()
	{
		PAINTSTRUCT ps;
		HDC Dc = BeginPaint(_Base, &ps);

		RECT WndRect;
		GetClientRect(_Base, &WndRect);

		HBRUSH Bk = CreateSolidBrush(Grey1);
		FillRect(Dc, &WndRect, Bk);

		DeleteObject(Bk);
		EndPaint(_Base, &ps);
		return 0;
	}
	LRESULT PasswordInput::KeyDown(WPARAM wp)
	{
		switch (wp)
		{
		case VK_RETURN:
			Command(1, 0);
			break;
		case VK_ESCAPE:
			Command(2, 0);
			break;
		}

		return 0;
	}
	LRESULT PasswordInput::Command(WPARAM wp, LPARAM lp)
	{
		switch (wp)
		{
		case 1:
			Return = Password->GetText();
			DestroyWindow(_Base);
			break;
		case 2:
			Return = L"";
			DestroyWindow(_Base);
			break;
		}
		return 0;
	}
}