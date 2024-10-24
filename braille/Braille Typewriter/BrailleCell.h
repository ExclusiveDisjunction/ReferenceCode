#pragma once

#include "BrailleDot.h"

class BrailleCell : public ControlBase
{
private:
	BrailleDot *One, *Two, *Three, *Four, *Five, *Six;
	AaColor Base;

	static ATOM _ThisAtom;
	static void InitBase(HINSTANCE ins);
	inline static LRESULT __stdcall WndProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp)
	{
		BrailleCell* This = reinterpret_cast<BrailleCell*>(GetWindowLongPtrW(Window, GWLP_USERDATA));
		if (!This)
			return DefWindowProcW(Window, Message, wp, lp);

		if (Message == WM_ERASEBKGND)
			return This->EraseBackground();
		if (Message == WM_PAINT)
			return This->Paint();
		if (Message == WM_DESTROY)
			return This->OnDestroy();

		return DefWindowProcW(Window, Message, wp, lp);
	}

public:
	BrailleCell(int XCoord, int YCoord, AaColor Base, AaColor Accent, HINSTANCE ins, HWND Parent);

	void GetState(bool& One, bool& Two, bool& Three, bool& Four, bool& Five, bool& Six)
	{
		One = this->One->State();
		Two = this->Two->State();
		Three = this->Three->State();
		Four = this->Four->State();
		Five = this->Five->State();
		Six = this->Six->State();
	}
	void SetState(bool One, bool Two, bool Three, bool Four, bool Five, bool Six)
	{
		this->One->State(One);
		this->Two->State(Two);
		this->Three->State(Three);
		this->Four->State(Four);
		this->Five->State(Five);
		this->Six->State(Six);
	}
	bool IsInState(bool One, bool Four, bool Two, bool Five, bool Three, bool Six);

	int Width()
	{
		RECT WndRect;
		GetClientRect(_Base, &WndRect);

		return static_cast<int>(WndRect.right);
	}
	int Height()
	{
		RECT WndRect;
		GetClientRect(_Base, &WndRect);

		return static_cast<int>(WndRect.bottom);
	}

	LRESULT Paint() override;
	LRESULT EraseBackground()
	{
		return 1;
	}
	LRESULT OnDestroy() override;
};