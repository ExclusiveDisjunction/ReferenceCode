#pragma once

#include <Windows.h>

class CallableTimer
{
public:
	virtual void TimerTick(int ID) = 0;
};

extern HINSTANCE ThisInstance;
extern ATOM TimerWindowAtom;

void InitTimerWindowClass();
LRESULT __stdcall TimerWindowProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp);

HWND CreateTimerWindow(CallableTimer* Timer, double Duration); //Duration is in seconds