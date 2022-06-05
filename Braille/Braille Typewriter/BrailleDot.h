#pragma once

#include "Controls.h"

class BrailleDot : public ControlBase
{
private:
	AaColor Base, Accent;
	bool _State = false; //When true, it is filled in, when false it is not filled in.

	inline static LRESULT __stdcall WndProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp)
	{
		if (Message == WM_ERASEBKGND)
			return 1;
		return CommonWndProc(Window, Message, wp, lp);
	}
	static ATOM _ThisAtom;
	static void InitBase(HINSTANCE ins);
public:
	BrailleDot(int XCoord, int YCoord, int Width, int Height, HWND Parent, HINSTANCE ins, AaColor Base, AaColor Accent);
	
	LRESULT KeyDown(WPARAM key) override
	{
		return SendMessageW(GetParent(_Base), WM_KEYDOWN, key, 0);
	}
	LRESULT Paint() override;

	bool State() const { return _State; }
	void State(bool New);
};