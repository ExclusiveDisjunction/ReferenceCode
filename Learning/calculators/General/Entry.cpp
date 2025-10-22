#include <Windows.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

#include "Main.h"

using namespace Gdiplus;

int __stdcall wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	ULONG_PTR GdiHandle;
	GdiplusStartupInput In;
	GdiplusStartup(&GdiHandle, &In, nullptr);

	gIns = hInstance;

	Main::Init();
	WPARAM Return = Main::Open();

	GdiplusShutdown(GdiHandle);
	return Return;
}