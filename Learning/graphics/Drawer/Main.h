#pragma once

#include <Windows.h>
#include <math.h>
#include <Windowsx.h>

#include "Container.h"
#include "Controls.h"
#include "Str.h"

#include "Coord.h"
#include "ColorDisplayer.h"
#include "Files\Component.h"

class Main
{
private:
	enum Modes
	{
		PolygonMode = 1,
		NewPoly = 2,
		NewLine = 4,
		DrawMode = 8,
		NewRay = 16,
		MidLine = 32,
		MidRay = 64,
		NewVector = 128,
		MidVector = 256
	};

	HWND _Base;
	uint Mode = 0;

	Label* StrokeSizeDisp;
	Grid* ToolBar;
	Button* LineModeBttn, * RayModeBttn, *DrawModeBttn, *VectorModeBttn;
	ColorDisplayer* Colors;

	VectorProject* ThisProject;

	COLORREF Bk = RGB(0x10, 0x10, 0x10), Accent = RGB(0x00, 0x9C, 0x9C);

	Vector<TrackedCoord*> TrackedCoords;
	TrackedCoord* TempBind;
	ValuePair<Coord*, Coord*> TempLine;

	uint StrokeWidth = 3;
	AaColor StrokeColor = 0xFFFFFFFF;
	AaColor Fill = Bk;
	POINT LastPoint, SelStartPoint;

	void ClearScreen();

	static ATOM _ThisAtom;
	static void InitBase(HINSTANCE ins);
	static LRESULT __stdcall WndProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp);

	LRESULT Paint();
	LRESULT Char(WPARAM wp);
	LRESULT KeyDown(WPARAM Key);
	LRESULT Command(WPARAM wp, LPARAM lp);
	LRESULT Destroy();
	LRESULT EraseBk();
	LRESULT MouseDown(WPARAM wp, LPARAM lp);
	LRESULT MouseUp(WPARAM wp, LPARAM lp);
	LRESULT MouseMove(WPARAM wp, LPARAM lp);
	LRESULT MouseScroll(WPARAM wp);
	LRESULT Size();

public:
	Main(HINSTANCE ins);

};