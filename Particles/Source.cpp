#include <Windows.h>
#include <gdiplus.h>
#include <math.h>
#include <vector>
#include <chrono>
#include <random>
#pragma comment(lib, "gdiplus.lib")

#include "BaseMath.h"
#include "Particle.h"
#include "BasicWindow.h"

using namespace std;
using namespace Gdiplus;

LRESULT __stdcall WndProc(HWND WIndow, UINT Message, WPARAM wp, LPARAM lp);

int __stdcall wWinMain(HINSTANCE ins, HINSTANCE, LPWSTR lpCmdLine, int nCmdShow)
{
	ThisInstance = ins;

	WNDCLASSW wn = { 0 };
	wn.lpfnWndProc = WndProc;
	wn.hInstance = ins;
	wn.hCursor = LoadCursorW(NULL, IDC_ARROW);
	wn.hbrBackground = NULL;
	wn.lpszMenuName = NULL;
	wn.lpszClassName = L"MainClass";
	wn.hIcon = LoadIconW(NULL, IDI_APPLICATION);
	wn.cbWndExtra = 0;
	wn.style = CS_HREDRAW | CS_VREDRAW;

	ATOM ThisAtom = RegisterClassW(&wn);
	if (!ThisAtom)
		return 1;

	HWND Window = CreateWindowExW(0l, MAKEINTATOM(ThisAtom), NULL, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, ins, NULL);
	if (!Window)
		return 1;

	Timekeeper::Init(Window);

	ULONG_PTR Handle;
	GdiplusStartupInput In;
	GdiplusStartup(&Handle, &In, nullptr);

	ShowWindow(Window, SW_MAXIMIZE);
	UpdateWindow(Window);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) != 0)
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}

	GdiplusShutdown(Handle);
	return msg.wParam;
}

auto Redraw = [](HWND Window) -> void { RedrawWindow(Window, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW); };

double CurrentAngle;

ParticleSystem* System;

ParticleProps Fire;
ParticleProps ThisProp;
Vector Gravity;

LRESULT __stdcall WndProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp)
{
	switch (Message)
	{
	case WM_SIZE:
	{
		Timekeeper::ForceRender();
		return 0;
	}
	case WM_CREATE:
	{
		RandomEngine.seed(static_cast<uint>(time(nullptr)));

		ThisProp.Position = { 0, 0 };
		ThisProp.Velocity = { 0, 100 };
		ThisProp.VelocityAngle = { 0, (2 * PI)};
		ThisProp.VelocityVariation = { 50, 30 };
		Gravity = { 0, 20 };
		ThisProp.ColorBegin = AaColor(0xFF, 0xFF, 0x00, 0x00);
		ThisProp.ColorEnd = AaColor(0xFF, 0x31, 0x31, 0x31);
		ThisProp.SizeBegin = 30;
		ThisProp.SizeEnd = 5;
		ThisProp.LifeTime = 4.0f;

		Fire = ThisProp;

		System = new ParticleSystem(ThisProp, 0.3, 10, Gravity);
		SetTimer(Window, 1, 600, NULL);
		return 0;
	}
	case WM_TIMER:
	{
		for (uint i = 0; i < 7; i++)
			System->Emit();
		return 0;
	}
	case WM_PAINT:
	{		
		return 0;
	}
	case WM_DESTROY:
		Timekeeper::Close();
		delete System;
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProcW(Window, Message, wp, lp);
	}
}