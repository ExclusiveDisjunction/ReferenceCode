#include "Window.h"

ULONG_PTR GdiPlusHandle;
Window MainWindow;

HINSTANCE Instance;

LRESULT __stdcall MainWindowProc(HWND, UINT, WPARAM, LPARAM);

int __stdcall wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	GdiplusStartupInput Input;
	GdiplusStartup(&GdiPlusHandle, &Input, NULL);

	WindowClass Class = WindowClass(L"MainWindow", hInstance, MainWindowProc, LoadIconW(NULL, IDI_APPLICATION), LoadCursorW(NULL, IDC_CROSS));

	Instance = hInstance;

	if (!MainWindow.Create(0l, Class, L"Calculator", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL))
		return 1;

	MainWindow.Show(true);

	MSG Message = { 0 };
	while (GetMessage(&Message, NULL, NULL, NULL))
	{
		TranslateMessage(&Message);
		DispatchMessageW(&Message);
	}

	GdiplusShutdown(GdiPlusHandle);

	return Message.wParam;
}

Window DarkModeToggle;

LRESULT __stdcall MainWindowProc(HWND Window, UINT Message, WPARAM WP, LPARAM LP)
{
	RECT WindowRect, ClientRect;
	MainWindow.GetWindowRect(&WindowRect);
	MainWindow.GetClientRect(&ClientRect);

	switch (Message)
	{
	case WM_COMMAND:
	{
		switch (LOWORD(WP))
		{
		case 1:
			MainWindow.InvalidateRect(&ClientRect);
			break;
		}

		return 0;
	}
	case WM_DRAWITEM:
	{
		LPDRAWITEMSTRUCT drawitem = (LPDRAWITEMSTRUCT)LP;
		switch (WP)
		{
			
		}

		return 0;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC dc = BeginPaint(Window, &ps);

		bool DoDarkMode = DarkModeToggle.SendMsg(BM_GETCHECK, 0, 0) == BST_CHECKED;

		HFONT Font = CreateFontW(-MulDiv(12, GetDeviceCaps(dc, LOGPIXELSY), 72), 0, 0, 0, FW_SEMIBOLD, false, false, false, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, L"Candara");

		DarkModeToggle.SetFont(Font);

		if (DoDarkMode)
		{
			FillRect(dc, &ClientRect, CreateSolidBrush(RGB(0x30, 0x30, 0x30)));
		}
		else
		{
			FillRect(dc, &ClientRect, (HBRUSH)COLOR_WINDOW);
		}

		return 0;
	}
	case WM_CREATE:
	{
		RECT Bounds;
		MainWindow.GetClientRect(&Bounds);

		DarkModeToggle.Create(0l, WindowClass(ButtonControl), L"Dark Mode", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 10, 10, 200, 23, Window, (HMENU)1, Instance, NULL);
		DarkModeToggle.Show(true);

		return 0;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	default:
		return DefWindowProcW(Window, Message, WP, LP);
	}
}