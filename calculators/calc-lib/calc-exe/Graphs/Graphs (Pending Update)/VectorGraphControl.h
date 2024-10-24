#pragma once

#include <Windows.h>
#include "..\..\Core\Arithmetic.h"
#include "..\..\Core\Container.h"
#include "..\..\Calculation\Function\Function.h"

namespa
struct VGraphStyle
{
	Core::AaColor Background;
	Core::AaColor Axis;
	Core::AaColor TickColor;
	Core::AaColor FunctionColor;
	bool ShowBaseVec;
};

class VectorGraphControl
{
private:
	HWND _Base;
	bool IsMouseDown = false, IsZooming = false;
	Vector2d Origin = { 0, 0 };
	Vector2d OPos;
	Vector2d CursorLast, OPosLast;
	Vector<Function2d*> ToGraph;
	VGraphStyle Style;

	void ZoomThis(double By);

	LRESULT Paint();
	LRESULT Size();
	LRESULT Command(WPARAM wp, LPARAM lp);
	LRESULT MouseDown(WPARAM wp, LPARAM lp);
	LRESULT MouseMove(WPARAM wp, LPARAM lp);
	LRESULT MouseUp(WPARAM wp, LPARAM lp);
	LRESULT MouseWheel(WPARAM wp);
	LRESULT KeyDown(WPARAM Key);
	LRESULT KeyUp(WPARAM Key);

	static LRESULT __stdcall WndProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp);
	static ATOM _ThisAtom;
	static void InitBase(HINSTANCE ins);
public:
	VectorGraphControl(int X, int Y, int Width, int Height, HWND Parent, HINSTANCE ins, VGraphStyle Style, Vector<Function2d*> Function);
	//GraphControl(int X, int Y, int Width, int Height, ControlBase* Parent, HINSTANCE ins, GraphStyle Style, Vector<Function*> Function) : GraphControl(X, Y, Width, Height, !Parent ? nullptr : *Parent, ins, Style, Function) {}
	~VectorGraphControl();

	double Zoom = 1;

	void Move(int X, int Y, int Width, int Height)
	{
		MoveWindow(_Base, X, Y, Width, Height, true);
	}
};