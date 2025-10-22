#include "InputTest.h"

InputTest::InputTest(HINSTANCE ins)
{
	if (!_ThisAtom)
		InitBase(ins);

	_Base = CreateWindowExW(0l, MAKEINTATOM(_ThisAtom), L"Input Test", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, ins, nullptr);
	SetWindowLongPtrW(_Base, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

	RECT WndRect;
	GetClientRect(_Base, &WndRect);

	StyleSheet Style;
	Style.Background = 0xFF373737;
	Controls = new Grid(WndRect.right - PanelSize, 0, PanelSize, WndRect.bottom, _Base, ins, Style);

	Graph = new NodesGraph(0, 0, WndRect.right - PanelSize, WndRect.bottom, _Base, ins);

	{
		RECT Client;
		GetClientRect(*Controls, &Client);

		int XCoord = 5;
		int YCoord = 5;
		int Width = (Client.right - XCoord - XCoord) / 2 - 5;
		int Height = 27;

		AaColor BaseBk = 0xFF373737;
		TextStyleSheet TextStyle;
		TextStyle.FontFamily = L"Candara";
		TextStyle.FontSize = 13;
		TextStyle.Foreground = 0xFFFFFFFF;
	}

	ShowWindow(_Base, SW_MAXIMIZE);
	UpdateWindow(_Base);
}
InputTest::~InputTest()
{
	DestroyWindow(_Base);
}

LRESULT __stdcall InputTest::WndProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp)
{
	InputTest* Obj = reinterpret_cast<InputTest*>(GetWindowLongPtrW(Window, GWLP_USERDATA));
	if (!Obj)
		return DefWindowProcW(Window, Message, wp, lp);

	switch (Message)
	{
	case WM_PAINT:
		return Obj->Paint();
	case WM_SIZE:
		return Obj->Size();
	case WM_COMMAND:
		return Obj->Command(wp, lp);
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProcW(Window, Message, wp, lp);
	}
}

ATOM InputTest::_ThisAtom = ATOM();
void InputTest::InitBase(HINSTANCE ins)
{
	WNDCLASSW wn = { 0 };
	wn.style = CS_HREDRAW | CS_VREDRAW;
	wn.lpfnWndProc = WndProc;
	wn.lpszClassName = L"InputTestClass";
	wn.hCursor = LoadCursorW(nullptr, IDC_ARROW);
	wn.hInstance = ins;
	wn.cbClsExtra = 0;
	wn.cbWndExtra = sizeof(InputTest*);
	wn.hIcon = LoadCursorW(nullptr, IDI_APPLICATION);
	wn.lpszMenuName = nullptr;
	wn.hbrBackground = nullptr;

	_ThisAtom = RegisterClassW(&wn);
}

LRESULT InputTest::Paint()
{
	PAINTSTRUCT ps;
	HDC Dc = BeginPaint(_Base, &ps);

	RECT WndRect;
	GetClientRect(_Base, &WndRect);

	HBRUSH Bk = CreateSolidBrush(0x000000);
	FillRect(Dc, &WndRect, Bk);

	DeleteObject(Bk);
	EndPaint(_Base, &ps);
	return 0;
}
LRESULT InputTest::Size()
{
	RECT WndRect;
	GetClientRect(_Base, &WndRect);

	if (Controls)
		Controls->Move(WndRect.right - PanelSize, 0, PanelSize, WndRect.bottom);

	if (Graph)
		Graph->Move(0, 0, WndRect.right - PanelSize, WndRect.bottom);
	return 0;
}
LRESULT InputTest::Command(WPARAM wp, LPARAM lp)
{
	return 0;
}