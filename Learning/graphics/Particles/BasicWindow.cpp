#include "BasicWindow.h"

HINSTANCE ThisInstance;
ATOM TimerWindowAtom;

void InitTimerWindowClass()
{
	WNDCLASSW wn;
	wn.lpfnWndProc = TimerWindowProc;
	wn.hInstance = ThisInstance;
	wn.hCursor = NULL;
	wn.hbrBackground = NULL;
	wn.lpszMenuName = NULL;
	wn.lpszClassName = L"TIMERCLASS";
	wn.cbClsExtra = 0;
	wn.cbWndExtra = sizeof(CallableTimer*);
	wn.hIcon = NULL;
	wn.style = 0;

	TimerWindowAtom = RegisterClassW(&wn);
}
LRESULT __stdcall TimerWindowProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp)
{
	CallableTimer* This = reinterpret_cast<CallableTimer*>(GetWindowLongPtrW(Window, GWLP_USERDATA));
	if (!This)
		return DefWindowProcW(Window, Message, wp, lp);

	switch (Message)
	{
	case WM_TIMER:
		This->TimerTick(reinterpret_cast<int>(Window));
		return 0;
	default:
		return DefWindowProcW(Window, Message, wp, lp);
	}
}

HWND CreateTimerWindow(CallableTimer* Timer, double Duration)
{
	long long TrueDuration = static_cast<long long>(Duration * 1000.0);

	if (!TimerWindowAtom)
		InitTimerWindowClass();

	HWND Return = CreateWindowExW(0l, MAKEINTATOM(TimerWindowAtom), NULL, WS_OVERLAPPEDWINDOW, 0, 0, 1, 1, NULL, NULL, ThisInstance, NULL);
	SetWindowLongPtrW(Return, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(Timer));
	ShowWindow(Return, SW_HIDE);

	SetTimer(Return, 1, TrueDuration, NULL);
	return Return;
}