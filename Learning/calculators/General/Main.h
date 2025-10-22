#pragma once

#include <Windows.h>
#include <CommCtrl.h>
#include <gdiplus.h>

#include "Calc.h"

void ButtonStyle(LPDRAWITEMSTRUCT lpd);

extern HINSTANCE gIns;

class Main
{
private:
	static HWND Base;
	static HWND TB1, TB2, DropDown, CalCmd, ExitCmd; //1, 2, 3, 4, 5
	static ATOM ThisAtom;

	static LRESULT __stdcall WndProc(HWND, UINT, WPARAM, LPARAM);
public:
	Main() {}

	static void Init();
	static WPARAM Open();
};