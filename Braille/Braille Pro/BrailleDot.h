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
	inline static void InitBase(HINSTANCE ins)
	{
		WNDCLASSW wn = { 0 };
		wn.lpfnWndProc = BrailleDot::WndProc;
		wn.hInstance = ins;
		wn.hCursor = LoadCursorW(NULL, IDC_ARROW);
		wn.hbrBackground = NULL;
		wn.lpszMenuName = NULL;
		wn.lpszClassName = L"BrailleDot";
		wn.hIcon = NULL;
		wn.cbClsExtra = 0;
		wn.cbWndExtra = sizeof(BrailleDot*);
		wn.style = CS_HREDRAW | CS_VREDRAW;

		_ThisAtom = RegisterClassW(&wn);
	}
public:
	BrailleDot(int XCoord, int YCoord, int Width, int Height, HWND Parent, HINSTANCE ins, AaColor Base, AaColor Accent)
	{
		if (!_ThisAtom)
			InitBase(ins);

		if (!_ThisAtom)
			return;

		this->Accent = Accent;
		this->Base = Base;

		_Base = CreateWindowExW(0l, MAKEINTATOM(_ThisAtom), L"", WS_CHILD | WS_VISIBLE, XCoord, YCoord, Width, Height, Parent, NULL, ins, NULL);
		SetWindowLongPtrW(_Base, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
	}
	
	LRESULT KeyDown(WPARAM key) override
	{
		return SendMessageW(GetParent(_Base), WM_KEYDOWN, key, 0);
	}
	LRESULT Paint() override;

	bool State() const { return _State; }
	void State(bool New);
};