#pragma once

#include <functional>
#include <chrono>
#include <future>
#include <cstdio>
#include <Windows.h>

#ifdef Using_Gdi
#include <Gdiplus.h>
#endif

#include "Container.h"

class TimekeeperSubscriber
{
public:
	virtual void OnUpdate(double ts) = 0;
	
	virtual void OnRender(HDC Dc, RECT Area) = 0;
};

struct TimerInfo
{
	int Duration;
	int Progress;
};

class Timekeeper
{
private:
	Timekeeper() {}
public:
	static void Init(HWND ToRender);
	static void Close();

	static void ForceRender();

	static void FramesPerSecond(int New);
	static void Subscribe(TimekeeperSubscriber* New);
	static void Desubscribe(TimekeeperSubscriber* Obj);
};