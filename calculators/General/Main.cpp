#include "Main.h"

using namespace std;
using namespace Gdiplus;

HINSTANCE gIns = nullptr;

HWND Main::Base = 0, Main::TB1 = 0, Main::TB2 = 0, Main::DropDown = 0, Main::CalCmd = 0, Main::ExitCmd = 0;
ATOM Main::ThisAtom = 0;

LRESULT __stdcall Main::WndProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp)
{
	switch (Message)
	{
	case WM_CREATE:
	{
		HINSTANCE ins = gIns;

		{
			const int Width = 200;
			const int Height = 27;
			const int YCoord = 30;
			int XCoord = 10;

			TB1 = CreateWindowExW(0l, L"EDIT", L"0.0", WS_CHILD | WS_VISIBLE, XCoord, YCoord, Width, Height, Window, (HMENU)1, ins, NULL);

			XCoord += Width + 30;
			TB2 = CreateWindowExW(0l, L"EDIT", L"0.0", WS_CHILD | WS_VISIBLE, XCoord, YCoord, Width, Height, Window, (HMENU)2, ins, NULL);
		}

		return 0;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC Dc = BeginPaint(Window, &ps);

		Graphics* Graph = Graphics::FromHDC(Dc);

		Color BackgroundColor(0xFF070707);
		SolidBrush BackgroundBrush(BackgroundColor);

		RECT RawRect = ps.rcPaint;
		Rect WndRect(RawRect.left, RawRect.top, RawRect.right, RawRect.bottom);

		Graph->FillRectangle(&BackgroundBrush, WndRect);

		delete Graph;
		return 0;
	}
	case WM_COMMAND:
	{
		switch (wp)
		{
		case 4:
		{
			CalculationHandle Handle = Calculation::ReadState(TB1, TB2, DropDown);
			Calculation::TranslateState(&Handle);
			wstring Result = Calculation::ProcessState(&Handle);

			MessageBoxW(Window, Result.c_str(), L"Your answer:", MB_OK | MB_ICONINFORMATION);
			break;
		}
		case 5:
			DestroyWindow(Window);
			break;
		}

		return 0;
	}
	case WM_DRAWITEM:
	{
		return 1;
	}
	case WM_SIZE:
	{
		return 0;
	}
	case WM_KEYDOWN:
	{
		if (wp == VK_ESCAPE)
			DestroyWindow(Window);
		else if (wp == VK_RETURN)
			SendMessageW(Window, WM_COMMAND, 4, 0);

		return 0;
	}
	case WM_DESTROY:
		DestroyWindow(TB1);
		DestroyWindow(TB2);
		DestroyWindow(DropDown);
		DestroyWindow(CalCmd);
		DestroyWindow(ExitCmd);

		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(Window, Message, wp, lp);
	}
}

void Main::Init()
{
	WNDCLASSW wn = { 0 };
	wn.lpfnWndProc = Main::WndProc;
	wn.hInstance = gIns;
	wn.lpszClassName = L"MainWindow";
	wn.hCursor = LoadCursorW(NULL, IDC_ARROW);
	wn.cbClsExtra = 0;
	wn.cbWndExtra = 0;
	wn.hIcon = LoadIconW(NULL, IDI_APPLICATION);
	wn.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wn.lpszMenuName = NULL;
	wn.style = CS_HREDRAW | CS_VREDRAW;

	ThisAtom = RegisterClassW(&wn);
}
WPARAM Main::Open()
{
	if (!ThisAtom)
		return 1;

	Base = CreateWindowExW(0l, MAKEINTATOM(ThisAtom), L"Calculator", WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, 450, 350, NULL, NULL, gIns, NULL);
	if (!Base)
		return 1;

	ShowWindow(Base, SW_NORMAL);
	UpdateWindow(Base);

	MSG msg;
	BOOL Result;
	while ((Result = GetMessageW(&msg, NULL, NULL, NULL)) != 0)
	{
		if (Result == -1)
			continue;

		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}

	return msg.wParam;
}