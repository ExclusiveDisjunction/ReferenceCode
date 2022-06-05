#include "..\Braille Typewriter\Main.h"
#include "BrailleDot.h"
#include "BrailleResolver.h"

#include <Windows.h>
#include <objbase.h>
#include <gdiplus.h>

using namespace Gdiplus;

#include "Controls.h"

class Main
{
private:
	BrailleGrid One, Two;
	Grid* Marker;
	CheckableButton* TestMode, * Keymode;

	HWND _Base;

	static ATOM _ThisAtom;
	inline static void InitBase(HINSTANCE ins)
	{
		WNDCLASSW wn = { 0 };
		wn.lpfnWndProc = Main::WndProc;
		wn.hInstance = ins;
		wn.hCursor = LoadCursorW(NULL, IDC_ARROW);
		wn.hbrBackground = NULL;
		wn.lpszMenuName = NULL;
		wn.lpszClassName = L"Main";
		wn.hIcon = LoadIconW(NULL, IDI_APPLICATION);
		wn.cbClsExtra = 0;
		wn.cbWndExtra = sizeof(Main*);
		wn.style = CS_HREDRAW | CS_VREDRAW;

		_ThisAtom = RegisterClassW(&wn);
	}
	inline static LRESULT __stdcall WndProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp)
	{
		Main* This = reinterpret_cast<Main*>(GetWindowLongPtrW(Window, GWLP_USERDATA));

		switch (Message)
		{
		case WM_PAINT:
			return This->Paint();
		case WM_KEYDOWN:
			return This->KeyDown(wp);
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		case WM_ERASEBKGND:
			return 1;
		case WM_TIMER:
			return This->Timer(wp);
		case WM_COMMAND:
			return This->Command(wp, lp);
		default:
			return DefWindowProcW(Window, Message, wp, lp);
		}
	}

	void LoadControls()
	{
		HINSTANCE ins = reinterpret_cast<HINSTANCE>(GetWindowLongPtrW(_Base, GWLP_HINSTANCE));
		RECT WndRect;
		GetClientRect(_Base, &WndRect);

		{
			int Width = 150;
			int Height = 30;
			int YCoord = 40;
			int XCoord = (WndRect.right / 2) - (Width / 2);

			TextStyleSheet TextStyle;
			TextStyle.Foreground = 0xFFFFFFFF;
			TextStyle.FontSize = 14;
			TextStyle.FontFamily = L"Candara";

			StyleSheet Style;
			Style.Background = 0xFF101010;
			Style.BaseBackground = 0xFF313131;
			Style.BorderBrush = 0xFF958A56;
			Style.BorderThickness = 3;

			TestMode = new CheckableButton(XCoord, YCoord, Width, Height, _Base, ins, (HMENU)1, true, L"Test Mode", CBT_Radio, Style, TextStyle);
			YCoord += 5 + Height;

			Keymode = new CheckableButton(XCoord, YCoord, Width, Height, _Base, ins, (HMENU)2, false, L"Key mode", CBT_Radio, Style, TextStyle);
		}

		{
			int Width = 30;
			int Height = 30;
			int XCoord = (WndRect.right / 2) - ((2 * Width) + 30);
			int YCoord = (WndRect.bottom / 2) - (Height / 2) - (Height + 10);

			{
				One.One = new BrailleDot(XCoord, YCoord, Width, Height, _Base, ins, 0xFF101010, 0xFF707070);
				YCoord += 10 + Height;

				One.Two = new BrailleDot(XCoord, YCoord, Width, Height, _Base, ins, 0xFF101010, 0xFF707070);
				YCoord += 10 + Height;

				One.Three = new BrailleDot(XCoord, YCoord, Width, Height, _Base, ins, 0xFF101010, 0xFF707070);
				YCoord += 10 + Height;

				YCoord -= 3 * (10 + Height);
				XCoord += Width + 10;

				One.Four = new BrailleDot(XCoord, YCoord, Width, Height, _Base, ins, 0xFF101010, 0xFF707070);
				YCoord += 10 + Height;

				One.Five = new BrailleDot(XCoord, YCoord, Width, Height, _Base, ins, 0xFF101010, 0xFF707070);
				YCoord += 10 + Height;

				One.Six = new BrailleDot(XCoord, YCoord, Width, Height, _Base, ins, 0xFF101010, 0xFF707070);
				YCoord += 10 + Height;
			}

			XCoord = (WndRect.right / 2) + 10;
			YCoord = (WndRect.bottom / 2) - (Height / 2) - (Height + 10);

			{
				Two.One = new BrailleDot(XCoord, YCoord, Width, Height, _Base, ins, 0xFF101010, 0xFF707070);
				YCoord += 10 + Height;

				Two.Two = new BrailleDot(XCoord, YCoord, Width, Height, _Base, ins, 0xFF101010, 0xFF707070);
				YCoord += 10 + Height;

				Two.Three = new BrailleDot(XCoord, YCoord, Width, Height, _Base, ins, 0xFF101010, 0xFF707070);
				YCoord += 10 + Height;

				YCoord -= 3 * (10 + Height);
				XCoord += Width + 10;

				Two.Four = new BrailleDot(XCoord, YCoord, Width, Height, _Base, ins, 0xFF101010, 0xFF707070);
				YCoord += 10 + Height;

				Two.Five = new BrailleDot(XCoord, YCoord, Width, Height, _Base, ins, 0xFF101010, 0xFF707070);
				YCoord += 10 + Height;

				Two.Six = new BrailleDot(XCoord, YCoord, Width, Height, _Base, ins, 0xFF101010, 0xFF707070);
				YCoord += 10 + Height;
			}

			Width = 100;
			Height = 50;
			XCoord = (WndRect.right / 2) - (Width / 2);
			YCoord = WndRect.bottom - Height - 20;

			StyleSheet Style;
			Style.Background = 0xFF404040;
			Style.BorderBrush = 0xFF101010;
			Style.BorderThickness = 1;

			Marker = new Grid(XCoord, YCoord, Width, Height, _Base, ins, Style);
		}
	}

	LRESULT Paint()
	{
		PAINTSTRUCT ps = { 0 };
		HDC Dc = BeginPaint(_Base, &ps);

		Graphics* Graph = Graphics::FromHDC(Dc);

		RECT WndRect;
		GetClientRect(_Base, &WndRect);
		Rect Area(WndRect.left, WndRect.top, WndRect.right, WndRect.bottom);

		SolidBrush Bk(0xFF313131);
		Graph->FillRectangle(&Bk, Area);

		delete Graph;
		EndPaint(_Base, &ps);
		return 0;
	}
	LRESULT KeyDown(WPARAM wp)
	{
		if (wp >= VK_NUMPAD0 && wp <= VK_NUMPAD9)
			wp -= 0x30;

		if (Keymode->GetCheckState())
		{
			FillKey(static_cast<char>(wp), One, Two);
			return 0;
		}

		bool Result = ProcessKeyDown(wp);
		StyleSheet New = Marker->GetStyle();
		if (!Result)
			New.Background = 0xFFFF0000;
		else
			New.Background = 0xFF00FF00;

		Marker->SetStyle(New);

		if (Result)
			SetTimer(_Base, 1, 500, NULL);
		else
			SetTimer(_Base, 2, 500, NULL);
		return 0;
	}
	LRESULT Timer(WPARAM ID)
	{
		if (ID == 2)
		{
			StyleSheet New = Marker->GetStyle();
			New.Background = 0xFF404040;
			Marker->SetStyle(New);
		}
		else
			MoveNext();

		KillTimer(_Base, ID);
		return 0;
	}
	LRESULT Command(WPARAM wp, LPARAM lp)
	{
		switch (wp)
		{
		case 1:
		{
			MoveNext();
			break;
		}
		case 2:
		{
			One.One->State(false);
			One.Two->State(false);
			One.Three->State(false);
			One.Four->State(false);
			One.Five->State(false);
			One.Six->State(false);

			Two.One->State(false);
			Two.Two->State(false);
			Two.Three->State(false);
			Two.Four->State(false);
			Two.Five->State(false);
			Two.Six->State(false);

			break;
		}
		}
		return 0;
	}

	void MoveNext()
	{
		StyleSheet Style = Marker->GetStyle();
		Style.Background = 0xFF404040;

		Marker->SetStyle(Style);

		GenerateItem(One, Two);
	}
public:
	Main(HINSTANCE ins)
	{
		if (!_ThisAtom)
			InitBase(ins);

		_Base = CreateWindowExW(0l, MAKEINTATOM(_ThisAtom), L"Braille", WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, 400, 600, NULL, NULL, ins, NULL);
		SetWindowLongPtrW(_Base, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

		ShowWindow(_Base, SW_NORMAL);
		UpdateWindow(_Base);

		LoadControls();
		MoveNext();
	}
};

ATOM Main::_ThisAtom = ATOM();

int __stdcall wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	ULONG_PTR Handle;
	GdiplusStartupInput Input;
	GdiplusStartup(&Handle, &Input, nullptr);

	Main* Item = new Main(hInstance);

	MSG msg;
	int Result;
	while ((Result = GetMessageW(&msg, NULL, NULL, NULL)) != 0)
	{
		if (Result < 0)
			continue;

		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}

	delete Item;

	GdiplusShutdown(Handle);
	return 0;
}