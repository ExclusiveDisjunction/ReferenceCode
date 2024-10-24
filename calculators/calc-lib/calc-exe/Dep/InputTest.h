#pragma once

#include <Windows.h>
//#include "Arithmoetic.h"
#include "Container.h"
#include "UI\Controls.h"

#include "Graphs\NodesGraph.h"
#include "UI\BoundLabel.h"

class InputTest
{
private:
	HWND _Base;
	
	const int PanelSize = 350;

	Grid* Controls;
	BoundLabel<double>* Zoom;
	ControlList MiscControls;

	NodesGraph* Graph;
	
	LRESULT Paint();
	LRESULT Size();
	LRESULT Command(WPARAM wp, LPARAM lp);

	static LRESULT __stdcall WndProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp);
	static ATOM _ThisAtom;
	static void InitBase(HINSTANCE ins);
public:
	InputTest(HINSTANCE ins);
	~InputTest();
};