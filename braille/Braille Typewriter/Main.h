#pragma once

#include "Container.h"
#include "Str.h"

#include "BrailleCell.h"
#include "BrailleDot.h"

#include "Controls.h"

String InterpretText(Vector<BrailleCell*>& Items);

class Main
{
private:
	HWND _Base;
	Vector<BrailleCell*> Letters;
	int CurrentX = 10;
	int CurrentY = 10;

	static ATOM _ThisAtom;
	static void InitBase(HINSTANCE ins);
	static LRESULT __stdcall WndProc(HWND, UINT, WPARAM, LPARAM);

	Vector<WPARAM> DownKeys, UpKeys;
	bool KeyMode = false;
	LRESULT KeyDown(WPARAM Key);
	LRESULT KeyUp(WPARAM Key);
	void FillKey();

	LRESULT Paint();
	LRESULT Destroy();

public:
	Main(HINSTANCE ins)
	{
		if (!_ThisAtom)
			InitBase(ins);

		_Base = CreateWindowExW(0l, MAKEINTATOM(_ThisAtom), L"Braille Typewriter", WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, 1200, 700, NULL, NULL, ins, NULL);
		SetWindowLongPtrW(_Base, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

		ShowWindow(_Base, SW_NORMAL);
		UpdateWindow(_Base);
	}
};