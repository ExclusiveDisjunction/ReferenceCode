#pragma once

#include "Controls Code\ControlBase.h"

class ColorDisplayer : public ControlBase
{
private:
	AaColor _BaseBk;
	AaColor _Fill;
	AaColor _Stroke;

	static ATOM _ThisAtom;
	static void InitBase(HINSTANCE ins);
	static LRESULT __stdcall WndProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp);

public:
	ColorDisplayer(int XCoord, int YCoord, int Width, int Height, HWND Parent, HINSTANCE ins, AaColor BaseBk, AaColor Fill = 0xFFFFFFFF, AaColor Stroke = 0xFF000000);
	ColorDisplayer(int XCoord, int YCoord, int Width, int Height, ControlBase* Parent, HINSTANCE ins, AaColor BaseBk, AaColor Fill = 0xFFFFFFFF, AaColor Stroke = 0xFF000000) : ColorDisplayer(XCoord, YCoord, Width, Height, *Parent, ins, BaseBk, Fill, Stroke) {}

	AaColor GetFill() const { return _Fill; }
	AaColor GetStroke() const { return _Stroke; }
	void SetFill(AaColor New);
	void SetStroke(AaColor New);

	void SwapColors();

	LRESULT Paint() override;
	LRESULT Click() override
	{ 
		SwapColors(); 
		return 0; 
	}
};