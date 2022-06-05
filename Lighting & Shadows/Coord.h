#pragma once

#include <Windows.h>

typedef POINT Coord;

class TrackedCoord
{
private:
	Coord* Modify;
	HWND _Base;
	bool _Track;
	COLORREF Border, Bk;

	static LRESULT __stdcall WndProc(HWND, UINT, WPARAM, LPARAM);
	static ATOM _ThisAtom;
	static void InitBase(HINSTANCE ins);

	LRESULT Paint();
	LRESULT MouseDown();
	LRESULT MouseUp();
	LRESULT Timer(WPARAM wp);
public:
	TrackedCoord(Coord* Bind, int Size, HWND Parent, COLORREF Border, COLORREF Bk, HINSTANCE ins);

	void SetBinding(Coord* New)
	{
		Modify = New;

		int X = New->x;
		int Y = New->y;

		RECT WndRect;
		GetClientRect(_Base, &WndRect);

		int WndX = X - (WndRect.right / 2);
		int WndY = Y - (WndRect.bottom / 2);

		MoveWindow(_Base, WndX, WndY, WndRect.right, WndRect.bottom, true);
	}

	void Destroy()
	{
		DestroyWindow(_Base);
		_Base = nullptr;
	}
};