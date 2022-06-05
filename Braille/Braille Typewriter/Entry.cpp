#include <iostream>
#include <Windows.h>

#include "Main.h"
#include "BrailleCell.h"
#include "BrailleDot.h"

#include <gdiplus.h>
using namespace Gdiplus;

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