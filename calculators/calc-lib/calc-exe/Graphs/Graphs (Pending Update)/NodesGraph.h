#pragma once

#include <Windows.h>
#include "Container.h"
#include "Color.h"
#include "Str.h"
#include "..\Calculation\Bezier.h"
#include "..\Calculation\Function.h"
#include "..\Calculation\StdCalc.h"

class NodesGraph
{
public:
	struct Box
	{
		Box(Vector2d Point) : Point(Point) {}

		Vector2d Point;
		int Hue = 0;
		double Size = 1;
		String Text;
	};
	struct Connection
	{
		Connection(Box* One, Box* Two) : One(One), Two(Two) {}

		Box* One, * Two;
		AaColor Color = 0xFF00FFFF;
	};
private:
	enum Modes
	{
		SelectionMode = 0,
		LinkMode = 1,
		MoveMode = 2,
		ConnectionSelMode = 4,
		AddNodeMode = 8,
		AcceptingNumber = 16,
		RadiusChangeMode = 32
	};

	bool IsMouseDown = false;
	Box* CurrentBox;
	Connection* CurrentConnection;
	uint CurrentIndex;

	Vector<Box*> Boxes;
	Vector<Connection*> Connections;

	int Mode = 0;
	int Radius = 5;
	double Zoom = 1;
	String RawTextIn;

	HWND _Base;

	Vector2d Origin, OPos, CursorLast, OPosLast;

	void ZoomThis(double By)
	{
		double OldZoom = Zoom;
		Zoom += By;

		if (Zoom < 0.1)
			Zoom = 0.1;
		if (Zoom > 12)
			Zoom = 12;

		if (OldZoom != Zoom)
			RedrawWindow(_Base, nullptr, nullptr, RDW_INVALIDATE | RDW_ERASENOW);
	}

	LRESULT Paint();
	LRESULT MouseDown();
	LRESULT MouseUp();
	LRESULT MouseMove();
	LRESULT MouseWheel(WPARAM wp);
	LRESULT Char(WPARAM wp);
	LRESULT KeyDown(WPARAM wp);

	static LRESULT __stdcall WndProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp);
	static ATOM _ThisAtom;
	static void InitBase(HINSTANCE ins);

public:
	NodesGraph(int X, int Y, int Width, int Height, HWND Parent, HINSTANCE ins);
	~NodesGraph();

	void Move(int X, int Y, int Width, int Height)
	{
		MoveWindow(_Base, X, Y, Width, Height, true);
	}
};