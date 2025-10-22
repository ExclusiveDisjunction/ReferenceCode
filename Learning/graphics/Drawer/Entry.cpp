#include <iostream>
#include <math.h>
#include <Windows.h>
#include <Windowsx.h>

#include "Container.h"
#include "Str.h"

#include "Coord.h"
#include "Main.h"

int __stdcall wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	Main* ThisInstance = new Main(hInstance);

	MSG msg;
	int Result;
	while ((Result = GetMessageW(&msg, NULL, NULL, NULL)) != 0)
	{
		if (Result < 0)
			break;

		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}

	return static_cast<int>(msg.wParam);
}