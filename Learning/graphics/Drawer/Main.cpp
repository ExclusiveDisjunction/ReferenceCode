#include "Main.h"

#include "Files\Stream.h"

Main::Main(HINSTANCE ins)
{
	if (!_ThisAtom)
		InitBase(ins);

	_Base = CreateWindowExW(0l, MAKEINTATOM(_ThisAtom), L"Draw", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 900, 700, NULL, NULL, ins, NULL);
	SetWindowLongPtrW(_Base, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

	TextStyleSheet TextStyle;
	TextStyle.FontFamily = L"Candara";
	TextStyle.FontSize = 13;
	TextStyle.Foreground = 0xFFFFFFFF;

	StyleSheet Style;
	Style.Background = 0xFF313131;
	Style.BorderBrush = 0xFFFF00FF;
	Style.BorderThickness = 3;
	Style.Radius = 20;

	if (!Stream::Exists(L"ThisProject.vectorproj"))
		ThisProject = VectorProject::Create(L"ThisProject.vectorproj");
	else
		ThisProject = new VectorProject(L"ThisProject.vectorproj");

	RECT WndRect;
	GetClientRect(_Base, &WndRect);

	{
		ToolBar = new Grid(0, 0, 100, WndRect.bottom, _Base, ins, Style);

		int XCoord = 5;
		int YCoord = 5;
		int Width = 90;
		int Height = 27;

		StrokeSizeDisp = new Label(XCoord, YCoord, Width, Height, ToolBar, ins, L"3", 0xFF313131, TextStyle, false);
		YCoord += 20 + Height;

		LineModeBttn = new Button(XCoord, YCoord, Width, Height, L"Lines", ToolBar, (HMENU)1, ins, Style, TextStyle);
		YCoord += 10 + Height;

		RayModeBttn = new Button(XCoord, YCoord, Width, Height, L"Rays", ToolBar, (HMENU)2, ins, Style, TextStyle);
		YCoord += 10 + Height;

		VectorModeBttn = new Button(XCoord, YCoord, Width, Height, L"Vectors", ToolBar, (HMENU)4, ins, Style, TextStyle);
		YCoord += 10 + Height;

		DrawModeBttn = new Button(XCoord, YCoord, Width, Height, L"Draw", ToolBar, (HMENU)3, ins, Style, TextStyle);
		YCoord += 30 + Height;

		Width = 80;
		Height = Width;

		Colors = new ColorDisplayer(10, YCoord, Width, Height, ToolBar, ins, Style.Background, 0xFFFFFFFF, 0xFFFF00FF);
	}

	LineList* Lines = ThisProject->Lines();
	for (uint i = 0; i < Lines->Count(); i++)
	{
		Line* Item = Lines->Item(i);
		TrackedCoords.Add(new TrackedCoord(Item->One, 20, _Base, RGB(0xFF, 0x30, 0x00), Bk, ins));
		TrackedCoords.Add(new TrackedCoord(Item->Two, 20, _Base, RGB(0xFF, 0x30, 0x00), Bk, ins));
	}

	RayList* Rays = ThisProject->Rays();
	for (uint i = 0; i < Rays->Count(); i++)
	{
		Ray* Item = Rays->Item(i);
		TrackedCoords.Add(new TrackedCoord(Item->Root, 20, _Base, RGB(0x90, 0x00, 0x90), Bk, ins));
		TrackedCoords.Add(new TrackedCoord(Item->Anchor, 20, _Base, RGB(0x90, 0x50, 0x50), Bk, ins));
	}

	Vector2DList* Vectors = ThisProject->Vectors();
	for (uint i = 0; i < Vectors->Count(); i++)
	{
		Vector2D* Item = Vectors->Item(i);
		TrackedCoords.Add(new TrackedCoord(Item->Root, 20, _Base, RGB(0x90, 0x00, 0x90), Bk, ins));
		TrackedCoords.Add(new TrackedCoord(Item->Anchor, 20, _Base, RGB(0x90, 0x50, 0x50), Bk, ins));
	}

	PolygonList* Polygons = ThisProject->Polygons();
	for (uint i = 0; i < Polygons->Count(); i++)
	{
		class Polygon* Item = Polygons->Item(i);
		for (uint j = 0; j < Item->Coordinates.Size(); j++)
			TrackedCoords.Add(new TrackedCoord(Item->Coordinates[j], 20, _Base, Accent, Bk, ins));
	}

	ShowWindow(_Base, SW_NORMAL);
	UpdateWindow(_Base);
}

ATOM Main::_ThisAtom = ATOM();
void Main::InitBase(HINSTANCE ins)
{
	WNDCLASSW wn = { 0 };
	wn.cbClsExtra = 0;
	wn.cbWndExtra = 0;
	wn.hbrBackground = NULL;
	wn.hCursor = LoadCursorW(NULL, IDC_ARROW);
	wn.hIcon = LoadIconW(NULL, IDI_APPLICATION);
	wn.lpfnWndProc = Main::WndProc;
	wn.lpszClassName = L"Main";
	wn.lpszMenuName = L"";
	wn.style = CS_HREDRAW | CS_VREDRAW;

	_ThisAtom = RegisterClassW(&wn);
}
LRESULT __stdcall Main::WndProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp)
{
	Main* Item = reinterpret_cast<Main*>(GetWindowLongPtrW(Window, GWLP_USERDATA));
	if (!Item)
		return DefWindowProcW(Window, Message, wp, lp);

	switch (Message)
	{
	case WM_PAINT:
		return Item->Paint();
	case WM_DESTROY:
		return Item->Destroy();
	case WM_CHAR:
		return Item->Char(wp);
	case WM_KEYDOWN:
		return Item->KeyDown(wp);
	case WM_ERASEBKGND:
		return Item->EraseBk();
	case WM_MOUSEWHEEL:
		return Item->MouseScroll(wp);
	case WM_LBUTTONDOWN:
		return Item->MouseDown(wp, lp);
	case WM_LBUTTONUP:
		return Item->MouseUp(wp, lp);
	case WM_MOUSEMOVE:
		return Item->MouseMove(wp, lp);
	case WM_SIZE:
		return Item->Size();
	case WM_COMMAND: 
		return Item->Command(wp, lp);
	default:
		return DefWindowProcW(Window, Message, wp, lp);
	}
}

LRESULT Main::EraseBk()
{
	return 1;
}
LRESULT Main::Paint()
{
	PAINTSTRUCT ps;
	HDC Dc = BeginPaint(_Base, &ps);

	RECT WndRect;
	GetClientRect(_Base, &WndRect);

	HBRUSH Bk = CreateSolidBrush(this->Bk);
	FillRect(Dc, &WndRect, Bk);
	DeleteObject(Bk);

	RECT Bounds = { WndRect.left + 70, WndRect.top, WndRect.right, WndRect.bottom };

	if (ThisProject)
	{
		ThisProject->Lines()->RenderAllItems(Dc, Bounds);
		ThisProject->Polygons()->RenderAllItems(Dc, Bounds);
		ThisProject->Rays()->RenderAllItems(Dc, Bounds);
		ThisProject->Vectors()->RenderAllItems(Dc, Bounds);
	}

	EndPaint(_Base, &ps);
	return 0;
}
LRESULT Main::Char(WPARAM wp)
{
	switch (wp)
	{
	case L'n':
		Mode |= NewPoly;
		break;
	case L'l':
	{
		if (Mode & NewRay)
			break;

		POINT Mouse;
		GetCursorPos(&Mouse);
		ScreenToClient(_Base, &Mouse);

		RECT WndRect;
		GetClientRect(_Base, &WndRect);

		if (Mouse.x < 0 || Mouse.y < 0 || Mouse.x > WndRect.right || Mouse.y > WndRect.bottom)
			return 0;

		int TargetIndex = wp - L'1';
		Coord TempMouse = { Mouse.x, Mouse.y };

		HINSTANCE ins = reinterpret_cast<HINSTANCE>(GetWindowLongPtrW(_Base, GWLP_HINSTANCE));

		if (Mode & NewLine)
		{
			TempLine.TwoValue = new Coord();
			*TempLine.TwoValue = TempMouse;
			Line* Line = ThisProject->Lines()->Add();
			memcpy(Line->One, TempLine.OneValue, sizeof(Coord));
			memcpy(Line->Two, TempLine.TwoValue, sizeof(Coord));

			Line->StrokeWidth = this->StrokeWidth;
			Line->Stroke = this->StrokeColor;

			TempBind->SetBinding(Line->One);
			TrackedCoords.Add(new TrackedCoord(Line->Two, 20, _Base, RGB(0xFF, 0x30, 0x00), Bk, ins));

			delete TempLine.OneValue;
			delete TempLine.TwoValue;
			TempLine.OneValue = TempLine.TwoValue = nullptr;

			TempBind = nullptr;

			RedrawWindow(_Base, NULL, NULL, RDW_INVALIDATE | RDW_ERASENOW);
			Mode &= ~NewLine;

		}
		else
		{
			Mode |= NewLine;

			TempLine.OneValue = new Coord();
			*TempLine.OneValue = TempMouse;

			TempBind = new TrackedCoord(TempLine.OneValue, 20, _Base, RGB(0xFF, 0x30, 0x00), Bk, ins);
			TrackedCoords.Add(TempBind);
		}
		break;
	}
	case L'r':
	{
		if (Mode & NewLine)
			break;

		POINT Mouse;
		GetCursorPos(&Mouse);
		ScreenToClient(_Base, &Mouse);

		RECT WndRect;
		GetClientRect(_Base, &WndRect);

		if (Mouse.x < 0 || Mouse.y < 0 || Mouse.x > WndRect.right || Mouse.y > WndRect.bottom)
			return 0;

		int TargetIndex = wp - L'1';
		Coord TempMouse = { Mouse.x, Mouse.y };

		HINSTANCE ins = reinterpret_cast<HINSTANCE>(GetWindowLongPtrW(_Base, GWLP_HINSTANCE));

		if (Mode & NewRay)
		{
			TempLine.TwoValue = new Coord();
			*TempLine.TwoValue = TempMouse;
			Ray* New = ThisProject->Rays()->Add();
			memcpy(New->Anchor, TempLine.TwoValue, sizeof(Coord));
			memcpy(New->Root, TempLine.OneValue, sizeof(Coord));

			New->Stroke = this->StrokeColor;
			New->StrokeWidth = this->StrokeWidth;

			TempBind->SetBinding(New->Root);
			TrackedCoords.Add(new TrackedCoord(New->Anchor, 20, _Base, RGB(0x90, 0x50, 0x50), Bk, ins));

			delete TempLine.OneValue;
			delete TempLine.TwoValue;
			TempLine.OneValue = TempLine.TwoValue = nullptr;

			TempBind = nullptr;

			RedrawWindow(_Base, NULL, NULL, RDW_INVALIDATE | RDW_ERASENOW);
			Mode &= ~NewRay;

		}
		else
		{
			Mode |= NewRay;

			TempLine.OneValue = new Coord();
			*TempLine.OneValue = TempMouse;

			TempBind = new TrackedCoord(TempLine.OneValue, 20, _Base, RGB(0x90, 0x00, 0x90), Bk, ins);
			TrackedCoords.Add(TempBind);
		}

		break;
	}
	case L'1':
	case L'2':
	case L'3':
	case L'4':
	case L'5':
	case L'6':
	case L'7':
	case L'8':
	case L'9':
	{
		/*
		POINT Mouse;
		GetCursorPos(&Mouse);
		ScreenToClient(_Base, &Mouse);

		RECT WndRect;
		GetClientRect(_Base, &WndRect);

		if (Mouse.x < 0 || Mouse.y < 0 || Mouse.x > WndRect.right || Mouse.y > WndRect.bottom)
			return 0;

		int TargetIndex = wp - L'1';
		Coord TempMouse = { Mouse.x, Mouse.y };

		if (Mode & NewPoly)
		{
			Vector<Coord*>& Poly = Polygons[TargetIndex];

			uint Index = Poly.Size();
			Poly.Add(new Coord());
			Poly[Index]->X = static_cast<int>(Mouse.x);
			Poly[Index]->Y = static_cast<int>(Mouse.y);

			HINSTANCE ins = reinterpret_cast<HINSTANCE>(GetWindowLongPtrW(_Base, GWLP_HINSTANCE));
			TrackedCoords.Add(new TrackedCoord(Poly[Index], 20, _Base, Accent, Bk, ins));
			RedrawWindow(_Base, NULL, NULL, RDW_INVALIDATE | RDW_ERASENOW);

			Mode &= ~NewPoly;
		}
		*/

		break;
	}
	case L'p':
		if (Mode & PolygonMode)
			Mode &= ~PolygonMode;
		else
			Mode |= PolygonMode;
		RedrawWindow(_Base, NULL, NULL, RDW_INVALIDATE | RDW_ERASENOW);
		break;
	}

	return 0;
}
LRESULT Main::KeyDown(WPARAM wp)
{
	switch (wp)
	{
	case VK_ESCAPE:
		ClearScreen();
		break;
	case L'S':
	{
		if (GetKeyState(VK_CONTROL) & 0x8000)
			ThisProject->Save();
	}
	}

	return 0;
}
LRESULT Main::MouseScroll(WPARAM wp)
{
	int Value = GET_WHEEL_DELTA_WPARAM(wp);
	if (StrokeWidth == 1 && Value < 0)
		return 0;
	else if (StrokeWidth == 50 && Value > 0)
		return 0;
	else
		this->StrokeWidth += (Value < 0 ? -1 : 1);

	StrokeSizeDisp->SetText(StrokeWidth);
	return 0;
}
LRESULT Main::MouseDown(WPARAM wp, LPARAM lp)
{
	if (Mode & NewLine)
	{
		POINT Mouse;
		GetCursorPos(&Mouse);
		ScreenToClient(_Base, &Mouse);

		RECT WndRect;
		GetClientRect(_Base, &WndRect);

		if (Mouse.x < 100 || Mouse.y < 0 || Mouse.x > WndRect.right || Mouse.y > WndRect.bottom)
			return 0;

		int TargetIndex = wp - L'1';
		Coord TempMouse = { Mouse.x, Mouse.y };

		HINSTANCE ins = reinterpret_cast<HINSTANCE>(GetWindowLongPtrW(_Base, GWLP_HINSTANCE));

		if (Mode & MidLine)
		{
			TempLine.TwoValue = new Coord();
			*TempLine.TwoValue = TempMouse;
			Line* Line = ThisProject->Lines()->Add();
			memcpy(Line->One, TempLine.OneValue, sizeof(Coord));
			memcpy(Line->Two, TempLine.TwoValue, sizeof(Coord));

			Line->StrokeWidth = this->StrokeWidth;
			Line->Stroke = this->Colors->GetStroke();

			TempBind->SetBinding(Line->One);
			TrackedCoords.Add(new TrackedCoord(Line->Two, 20, _Base, RGB(0xFF, 0x30, 0x00), Bk, ins));

			delete TempLine.OneValue;
			delete TempLine.TwoValue;
			TempLine.OneValue = TempLine.TwoValue = nullptr;

			TempBind = nullptr;

			RedrawWindow(_Base, NULL, NULL, RDW_INVALIDATE | RDW_ERASENOW);
			Mode &= ~MidLine;

		}
		else
		{
			Mode |= MidLine;

			TempLine.OneValue = new Coord();
			*TempLine.OneValue = TempMouse;

			TempBind = new TrackedCoord(TempLine.OneValue, 20, _Base, RGB(0xFF, 0x30, 0x00), Bk, ins);
			TrackedCoords.Add(TempBind);
		}
	}
	else if (Mode & NewRay)
	{
		POINT Mouse;
		GetCursorPos(&Mouse);
		ScreenToClient(_Base, &Mouse);

		RECT WndRect;
		GetClientRect(_Base, &WndRect);

		if (Mouse.x < 100 || Mouse.y < 0 || Mouse.x > WndRect.right || Mouse.y > WndRect.bottom)
			return 0;

		int TargetIndex = wp - L'1';
		Coord TempMouse = { Mouse.x, Mouse.y };

		HINSTANCE ins = reinterpret_cast<HINSTANCE>(GetWindowLongPtrW(_Base, GWLP_HINSTANCE));

		if (Mode & MidRay)
		{
			TempLine.TwoValue = new Coord();
			*TempLine.TwoValue = TempMouse;
			Ray* New = ThisProject->Rays()->Add();
			memcpy(New->Anchor, TempLine.TwoValue, sizeof(Coord));
			memcpy(New->Root, TempLine.OneValue, sizeof(Coord));

			New->Stroke = this->Colors->GetStroke();
			New->StrokeWidth = this->StrokeWidth;

			TempBind->SetBinding(New->Root);
			TrackedCoords.Add(new TrackedCoord(New->Anchor, 20, _Base, RGB(0x90, 0x50, 0x50), Bk, ins));

			delete TempLine.OneValue;
			delete TempLine.TwoValue;
			TempLine.OneValue = TempLine.TwoValue = nullptr;

			TempBind = nullptr;

			RedrawWindow(_Base, NULL, NULL, RDW_INVALIDATE | RDW_ERASENOW);
			Mode &= ~MidRay;

		}
		else
		{
			Mode |= MidRay;

			TempLine.OneValue = new Coord();
			*TempLine.OneValue = TempMouse;

			TempBind = new TrackedCoord(TempLine.OneValue, 20, _Base, RGB(0x90, 0x00, 0x90), Bk, ins);
			TrackedCoords.Add(TempBind);
		}
	}
	else if (Mode & NewVector)
	{
		POINT Mouse;
		GetCursorPos(&Mouse);
		ScreenToClient(_Base, &Mouse);

		RECT WndRect;
		GetClientRect(_Base, &WndRect);

		if (Mouse.x < 100 || Mouse.y < 0 || Mouse.x > WndRect.right || Mouse.y > WndRect.bottom)
			return 0;

		int TargetIndex = wp - L'1';
		Coord TempMouse = { Mouse.x, Mouse.y };

		HINSTANCE ins = reinterpret_cast<HINSTANCE>(GetWindowLongPtrW(_Base, GWLP_HINSTANCE));

		if (Mode & MidVector)
		{
			TempLine.TwoValue = new Coord();
			*TempLine.TwoValue = TempMouse;
			Vector2D* New = ThisProject->Vectors()->Add();
			memcpy(New->Anchor, TempLine.TwoValue, sizeof(Coord));
			memcpy(New->Root, TempLine.OneValue, sizeof(Coord));

			New->Stroke = this->Colors->GetStroke();
			New->StrokeWidth = this->StrokeWidth;

			TempBind->SetBinding(New->Root);
			TrackedCoords.Add(new TrackedCoord(New->Anchor, 20, _Base, RGB(0x90, 0x50, 0x50), Bk, ins));

			delete TempLine.OneValue;
			delete TempLine.TwoValue;
			TempLine.OneValue = TempLine.TwoValue = nullptr;

			TempBind = nullptr;

			RedrawWindow(_Base, NULL, NULL, RDW_INVALIDATE | RDW_ERASENOW);
			Mode &= ~MidVector;

		}
		else
		{
			Mode |= MidVector;

			TempLine.OneValue = new Coord();
			*TempLine.OneValue = TempMouse;

			TempBind = new TrackedCoord(TempLine.OneValue, 20, _Base, RGB(0x90, 0x00, 0x90), Bk, ins);
			TrackedCoords.Add(TempBind);
		}
	}
	else if (Mode & DrawMode)
	{
		int XCoord = GET_X_LPARAM(lp);
		int YCoord = GET_Y_LPARAM(lp);
		LastPoint.x = XCoord;
		LastPoint.y = YCoord;

		HDC Dc = GetDC(_Base);

		HPEN Pen = CreatePen(PS_SOLID, this->StrokeWidth, StrokeColor);
		SelectObject(Dc, Pen);

		MoveToEx(Dc, LastPoint.x, LastPoint.y, NULL);
		LineTo(Dc, XCoord, YCoord);

		LastPoint.x = XCoord;
		LastPoint.y = YCoord;

		ReleaseDC(_Base, Dc);
		DeleteObject(Pen);
	}


	return 0;
}
LRESULT Main::MouseUp(WPARAM wp, LPARAM lp)
{
	if (Mode & DrawMode)
	{
		int XCoord = GET_X_LPARAM(lp);
		int YCoord = GET_Y_LPARAM(lp);

		HDC Dc = GetDC(_Base);

		HPEN Pen = CreatePen(PS_SOLID, this->StrokeWidth, StrokeColor);
		SelectObject(Dc, Pen);

		MoveToEx(Dc, LastPoint.x, LastPoint.y, NULL);
		LineTo(Dc, XCoord, YCoord);

		LastPoint.x = XCoord;
		LastPoint.y = YCoord;

		ReleaseDC(_Base, Dc);
		DeleteObject(Pen);
	}

	if (!(Mode & DrawMode))
	{
		RedrawWindow(_Base, NULL, NULL, RDW_INVALIDATE | RDW_ERASENOW);
	}

	return 0;
}
LRESULT Main::MouseMove(WPARAM wp, LPARAM lp)
{
	if (Mode & DrawMode)
	{
		int XCoord = GET_X_LPARAM(lp);
		int YCoord = GET_Y_LPARAM(lp);

		HDC Dc = GetDC(_Base);

		HPEN Pen = CreatePen(PS_SOLID, this->StrokeWidth, StrokeColor);
		SelectObject(Dc, Pen);

		MoveToEx(Dc, LastPoint.x, LastPoint.y, NULL);
		LineTo(Dc, XCoord, YCoord);

		LastPoint.x = XCoord;
		LastPoint.y = YCoord;

		ReleaseDC(_Base, Dc);
		DeleteObject(Pen);
	}

	return 0;
}
LRESULT Main::Destroy()
{
	CloseControl(this->ToolBar);
	CloseControl(this->DrawModeBttn, LineModeBttn, RayModeBttn, VectorModeBttn);
	CloseControl(Colors);

	ClearScreen();
	delete ThisProject;

	PostQuitMessage(0);
	return 0;
}
LRESULT Main::Size()
{
	RECT WndRect;
	GetClientRect(_Base, &WndRect);

	ToolBar->Move(0, 0, 100, WndRect.bottom);
	return 0;
}
LRESULT Main::Command(WPARAM wp, LPARAM lp)
{
	switch (wp)
	{
	case 1: //Line Mode
	{
		if ((Mode & MidVector) || (Mode & MidRay) || (Mode & MidLine))
			break;

		if (Mode & NewLine)
			Mode &= ~NewLine;
		else
		{
			Mode |= NewLine;
			Mode &= ~NewVector;
			Mode &= ~NewRay;
		}
		break;
	}
	case 2: //New Ray Mode
	{
		if ((Mode & MidVector) || (Mode & MidRay) || (Mode & MidLine))
			break;

		if (Mode & NewRay)
			Mode &= ~NewRay;
		else
		{
			Mode |= NewRay;
			Mode &= ~NewLine;
			Mode & -~NewVector;
		}
		break;
	}
	case 4:
	{
		if ((Mode & MidVector) || (Mode & MidRay) || (Mode & MidLine))
			break;

		if (Mode & NewVector)
			Mode &= ~NewVector;
		else
		{
			Mode |= NewVector;
			Mode &= ~NewLine;
			Mode &= ~NewRay;
		}
		break;
	}
	case 3: //Draw Mode
	{
		if ((Mode & MidVector) || (Mode & MidRay) || (Mode & MidLine))
			break;

		if (Mode & DrawMode)
			Mode &= ~DrawMode;
		else
			Mode |= DrawMode;
		break;
	}
	}

	return 0;
}

void Main::ClearScreen()
{
	ThisProject->Rays()->ClearItems();
	ThisProject->Lines()->ClearItems();
	ThisProject->Polygons()->ClearItems();
	ThisProject->Vectors()->ClearItems();

	CloseControls(TrackedCoords);

	RedrawWindow(_Base, NULL, NULL, RDW_INVALIDATE | RDW_ERASENOW);
}