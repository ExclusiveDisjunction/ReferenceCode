#pragma once

#include <Windows.h>
#include "Arithmetic.h"
#include "..\Calculation\Function.h"

struct GraphStyle
{
	AaColor Background;
	AaColor Axis;
	AaColor TickColor;
	AaColor FunctionColor;
	bool ShowBaseVec;
};

class GraphControl
{
private:
	HWND _Base;
	bool IsMouseDown = false, IsZooming = false;
	Vector2d Origin = { 0, 0 };
	Vector2d OPos;
	Vector2d CursorLast, OPosLast;
	Vector<Function*> ToGraph;
	GraphStyle Style;
	Vector<Vector2d> RenderPoints;

	Vector2d OriginToScreen(const Vector2d& Old, RECT WndRect) const
	{
		return Vector2d{ (Old.X * 50.0) + OPos.X, (Old.Y * -50.0) + OPos.Y };
	}

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
	GraphControl(int X, int Y, int Width, int Height, HWND Parent, HINSTANCE ins, GraphStyle Style, Vector<Function*> Function);
	//GraphControl(int X, int Y, int Width, int Height, ControlBase* Parent, HINSTANCE ins, GraphStyle Style, Vector<Function*> Function) : GraphControl(X, Y, Width, Height, !Parent ? nullptr : *Parent, ins, Style, Function) {}
	~GraphControl();

	double Zoom = 1;

	void Move(int X, int Y, int Width, int Height)
	{
		MoveWindow(_Base, X, Y, Width, Height, true);
	}

	void ActivatePoint(Vector2d Point)
	{
		RenderPoints.Add(Point);
	}
	void DeActivatePoint(Vector2d Point)
	{
		RenderPoints.Remove(Point);
	}
	void DeActivateAllPoints()
	{
		RenderPoints.Clear();
	}
};