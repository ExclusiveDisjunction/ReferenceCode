#include "VectorGraphControl.h"

VectorGraphControl::VectorGraphControl(int X, int Y, int Width, int Height, HWND Parent, HINSTANCE ins, VGraphStyle Style, Vector<Function2d*> Function)
{
	if (!_ThisAtom)
		InitBase(ins);

	this->Style = Style;
	ToGraph = Function;

	_Base = CreateWindowExW(0l, MAKEINTATOM(_ThisAtom), L"Input Test", WS_CHILD | WS_VISIBLE, X, Y, Width, Height, Parent, nullptr, ins, nullptr);
	SetWindowLongPtrW(_Base, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

	RECT WndRect;
	GetClientRect(_Base, &WndRect);

	OPos.Y = WndRect.bottom / 2.0;
	OPos.X = WndRect.right / 2.0;
}
VectorGraphControl::~VectorGraphControl()
{
	SetWindowLongPtrW(_Base, GWLP_USERDATA, 0);
	DestroyWindow(_Base);

	for (Function2d* Obj : ToGraph)
		delete Obj;

	ToGraph.Clear();
}

LRESULT __stdcall VectorGraphControl::WndProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp)
{
	VectorGraphControl* Obj = reinterpret_cast<VectorGraphControl*>(GetWindowLongPtrW(Window, GWLP_USERDATA));

	if (Message == WM_DESTROY)
	{
		if (Obj)
			delete Obj;
		return 0;
	}

	if (!Obj)
		return DefWindowProcW(Window, Message, wp, lp);

	switch (Message)
	{
	case WM_PAINT:
		return Obj->Paint();
	case WM_SIZE:
		return Obj->Size();
	case WM_COMMAND:
		return Obj->Command(wp, lp);
	case WM_LBUTTONDOWN:
		return Obj->MouseDown(wp, lp);
	case WM_LBUTTONUP:
		return Obj->MouseUp(wp, lp);
	case WM_MOUSEMOVE:
		return Obj->MouseMove(wp, lp);
	case WM_MOUSEWHEEL:
		return Obj->MouseWheel(wp);
	case WM_KEYDOWN:
		return Obj->KeyDown(wp);
	case WM_KEYUP:
		return Obj->KeyUp(wp);
	default:
		return DefWindowProcW(Window, Message, wp, lp);
	}
}

ATOM VectorGraphControl::_ThisAtom = ATOM();
void VectorGraphControl::InitBase(HINSTANCE ins)
{
	WNDCLASSW wn = { 0 };
	wn.style = CS_HREDRAW | CS_VREDRAW;
	wn.lpfnWndProc = WndProc;
	wn.lpszClassName = L"VectorGraphControlClass";
	wn.hCursor = LoadCursorW(nullptr, IDC_ARROW);
	wn.hInstance = ins;
	wn.cbClsExtra = 0;
	wn.cbWndExtra = sizeof(VectorGraphControl*);
	wn.hIcon = LoadCursorW(nullptr, IDI_APPLICATION);
	wn.lpszMenuName = nullptr;
	wn.hbrBackground = nullptr;

	_ThisAtom = RegisterClassW(&wn);
}

LRESULT VectorGraphControl::Paint()
{
	PAINTSTRUCT ps;
	HDC Dc = BeginPaint(_Base, &ps);

	RECT WndRect;
	GetClientRect(_Base, &WndRect);

	HBRUSH Bk = CreateSolidBrush(Style.Background);
	FillRect(Dc, &WndRect, Bk);

	DeleteObject(Bk);

	HPEN Axis = CreatePen(PS_SOLID, 13, Style.Axis);

	auto DrawLine = [this](Vector2d Origin, Vector2d Axis, HDC Dc, RECT WndRect)
	{
		if (Origin == Axis)
			return;

		Origin = OriginToScreen(Origin, OPos, WndRect);
		Axis = OriginToScreen(Axis, OPos, WndRect);

		double My = Origin.Y - Axis.Y;
		double Mx = Origin.X - Axis.X;

		if (My == 0) //Horizontal
		{
			POINT P1 = { 0, long(Origin.Y) }, P2 = { WndRect.right, long(Origin.Y) };

			MoveToEx(Dc, P1.x, P1.y, NULL);
			LineTo(Dc, P2.x, P2.y);
		}
		else if (Mx == 0) //Vertical Line
		{
			POINT P1 = { long(Origin.X), 0 }, P2 = { long(Origin.X), WndRect.bottom };

			MoveToEx(Dc, P1.x, P1.y, NULL);
			LineTo(Dc, P2.x, P2.y);
		}
		else
		{
			double M = My / Mx;
			double B = Origin.Y - (M * Origin.X);

			double XIntercept = -B / M;

			if (XIntercept > 0)
			{
				double NewM = 1 / M;
				double NewB = -B / M;

				double YOne = 0;
				double YTwo = (NewM * double(WndRect.bottom)) + NewB;

				MoveToEx(Dc, (int)XIntercept, int(YOne), nullptr);
				LineTo(Dc, WndRect.bottom, int(YTwo));
			}
			else
			{
				double YOne = B;
				double YTwo = (M * double(WndRect.right)) + B;

				MoveToEx(Dc, 0, int(YOne), nullptr);
				LineTo(Dc, WndRect.right, int(YTwo));
			}
		}
	};

	SelectObject(Dc, Axis);

	Vector2d iHat = { Zoom, 0 }, jHat = { 0, Zoom };

	DrawLine(Origin, iHat, Dc, WndRect);
	DrawLine(Origin, jHat, Dc, WndRect);

	DeleteObject(Axis);

	Vector2d TiHat = OriginToScreen(iHat, OPos, WndRect), TjHat = OriginToScreen(jHat, OPos, WndRect);

	{
		Vector<Vector2d> RenderPoints;
		if (Style.ShowBaseVec)
		{
			RenderPoints.Add(iHat);
			RenderPoints.Add(jHat);
		}

		HPEN BoxPen = CreatePen(PS_SOLID, (int)(5 * Zoom), 0x00FF00);
		HGDIOBJ OldPen = SelectObject(Dc, BoxPen);
		HGDIOBJ OldBrush = SelectObject(Dc, GetStockObject(NULL_BRUSH));
		for (Vector2d& Obj : RenderPoints)
		{
			Vector2d Tran = Obj.X * iHat + Obj.Y * jHat;
			Tran = OriginToScreen(Tran, OPos, WndRect);

			if (!PtInRect(&WndRect, Tran.operator POINT()))
				continue;

			int By = 10 * Zoom;
			Rectangle(Dc, long(Tran.X) - By, long(Tran.Y) - By, long(Tran.X + By), long(Tran.Y + By));
		}

		DeleteObject(BoxPen);
		SelectObject(Dc, OldPen);
		SelectObject(Dc, OldBrush);
	}

	double XMin, XMax, YMin, YMax;

	{
		Vector2d TOrigin = OriginToScreen(Origin, OPos, WndRect);
		int Distance = (int)sqrt(pow(TiHat.X - TOrigin.X, 2.0) + pow(TiHat.Y - TOrigin.Y, 2.0));

		HPEN TickPen = CreatePen(PS_SOLID, 3, Style.TickColor);
		HGDIOBJ OldPen = SelectObject(Dc, TickPen);

		double j = 0;
		for (int i = (int)TOrigin.X; i <= WndRect.right; i += Distance, j++)
		{
			MoveToEx(Dc, i, int(TOrigin.Y) - 15, nullptr);
			LineTo(Dc, i, int(TOrigin.Y) + 15);
		}
		XMax = j;
		j = 0;
		for (int i = (int)TOrigin.X; i >= 0; i -= Distance, j--)
		{
			MoveToEx(Dc, i, int(TOrigin.Y) - 15, nullptr);
			LineTo(Dc, i, int(TOrigin.Y) + 15);
		}
		XMin = j;
		j = 0;

		Distance = (int)sqrt(pow(TjHat.X - TOrigin.X, 2) + pow(TjHat.Y - TOrigin.Y, 2));

		for (int i = (int)TOrigin.Y; i >= 0; i -= Distance, j--)
		{
			MoveToEx(Dc, int(TOrigin.X) - 15, i, nullptr);
			LineTo(Dc, int(TOrigin.X) + 15, i);
		}
		YMin = j;
		j = 0;
		for (int i = (int)TOrigin.Y; i < WndRect.bottom; i += Distance, j++)
		{
			MoveToEx(Dc, int(TOrigin.X) - 15, i, nullptr);
			LineTo(Dc, int(TOrigin.X) + 15, i);
		}
		YMax = j;

		DeleteObject(TickPen);
		SelectObject(Dc, OldPen);

		XMin++;
		XMax++;
		YMin++;
		YMax++;
	}

	if (!IsMouseDown)
	{
		double Precis = 1 / ((Zoom + 1) * 16);

		bool Break = true; //When true, the algorithim will not connect the previous point to the current point, but rather move to that other point.
		int Rate = int(50 * (1/Zoom)) + 10;
		RECT BoundRect = { -Rate, -Rate, WndRect.right + Rate, WndRect.bottom + Rate };
		for (Function2d* Obj : ToGraph)
		{
			if (!Obj)
				continue;

			double MaxM;
			double MinM;
			Vector<VectorResult> Results;
			bool First = true;

			for (double x = XMin - 1; x < XMax + 1; x += 1)
			{
				for (double y = YMin - 1; y < YMax + 1; y += 1)
				{
					bool Exists = true;
					VectorResult Result = Obj->Evaluate(x, y, Exists);
					if (!Exists)
					{
						Break = true;
						continue;
					}

					Results.Add(Result);
					double Magnitude = Result.Magnitude();
					
					if (First)
					{
						MaxM = MinM = Magnitude;
						First = false;
					}
					else
					{
						MaxM = (MaxM > Magnitude) ? MaxM : Magnitude;
						MinM = (MinM < Magnitude) ? MinM : Magnitude;
					}				
				}
			}

			SelectObject(Dc, GetStockObject(NULL_BRUSH));

			for (VectorResult Result : Results)
			{
				Vector2d PointR = { Result.RX, Result.RY };
				PointR = PointR.X * Vector2d(Zoom, 0) + PointR.Y * Vector2d(0, Zoom);
				PointR = OriginToScreen(PointR, OPos, WndRect);
				POINT Point = PointR.operator POINT();

				/*if (!PtInRect(&BoundRect, Point))
				{
					Break = true;
					continue;
				}*/

				double Radius = 0.5;
				double Angle = Result.Angle();

				Vector2d PointO = { Radius * cos(Angle) + Result.RX, Radius * sin(Angle) + Result.RY };
				Vector2d RBase = PointO;
				PointO = PointO.X * Vector2d(Zoom, 0) + PointO.Y * Vector2d(0, Zoom);
				PointO = OriginToScreen(PointO, OPos, WndRect);
				POINT OPoint = PointO.operator POINT();

				Vector2d RectU = RBase - Vector2d{ 0.05, 0.05}, RectL = RBase + Vector2d{ 0.2, 0.2 };
				RectU = RectU.X * Vector2d(Zoom, 0) + RectU.Y * Vector2d(0, Zoom);
				RectL = RectL.X * Vector2d(Zoom, 0) + RectL.Y * Vector2d(0, Zoom);
				RectU = OriginToScreen(RectU, OPos, WndRect);
				RectL = OriginToScreen(RectL, OPos, WndRect);

				double H = MapLerp(MaxM, MinM, 360, 0, MaxM - Result.Magnitude());
				AaColor LineColor = HSVtoRGB(H, 75, 75);
				HPEN Border = CreatePen(PS_SOLID, 3 * Zoom, LineColor);
				SelectObject(Dc, Border);

				MoveToEx(Dc, Point.x, Point.y, nullptr);
				LineTo(Dc, OPoint.x, OPoint.y);
				Rectangle(Dc, (int)RectU.X, (int)RectU.Y, (int)RectL.X, (int)RectL.Y);

				DeleteObject(Border);
			}

			Break = true;
		}
	}

	EndPaint(_Base, &ps);
	return 0;
}
LRESULT VectorGraphControl::Size()
{
	return 0;
}
LRESULT VectorGraphControl::Command(WPARAM wp, LPARAM lp)
{
	return 0;
}
LRESULT VectorGraphControl::MouseDown(WPARAM wp, LPARAM lp)
{
	SetFocus(_Base);

	POINT Cursor;
	GetCursorPos(&Cursor);

	CursorLast = { double(Cursor.x), double(Cursor.y) };
	OPosLast = OPos;

	IsMouseDown = true;
	return 0;
}
LRESULT VectorGraphControl::MouseMove(WPARAM wp, LPARAM lp)
{
	if (IsZooming)
	{
		POINT Cursor;
		GetCursorPos(&Cursor);

		double Distance = sqrt(pow(Cursor.x - CursorLast.X, 2) + pow(Cursor.y - CursorLast.Y, 2));
		Distance /= 32;
		if ((double)Cursor.y < CursorLast.Y)
			Distance *= -1;

		ZoomThis(Distance);
		CursorLast = { (double)Cursor.x, (double)Cursor.y };
	}
	else if (IsMouseDown)
	{
		if (!(GetAsyncKeyState(VK_LBUTTON) & 0x8000))
		{
			IsMouseDown = false;
			return 0;
		}

		POINT NewLoc;
		GetCursorPos(&NewLoc);

		double ChangeY = double(NewLoc.y) - CursorLast.Y;
		ChangeY *= 2;
		double ChangeX = double(NewLoc.x) - CursorLast.X;
		ChangeX *= 2;

		OPos = OPosLast + Vector2d(ChangeX, ChangeY);
		RedrawWindow(_Base, nullptr, nullptr, RDW_ERASENOW | RDW_INVALIDATE);
	}
	return 0;
}
LRESULT VectorGraphControl::MouseUp(WPARAM wp, LPARAM lp)
{
	IsMouseDown = false;
	RedrawWindow(_Base, nullptr, nullptr, RDW_INVALIDATE | RDW_ERASENOW);
	return 0;
}
LRESULT VectorGraphControl::MouseWheel(WPARAM wp)
{
	double Change = (double)GET_WHEEL_DELTA_WPARAM(wp);
	Change /= ((double)WHEEL_DELTA * 4.0);
	ZoomThis(Change);
	return 0;
}
LRESULT VectorGraphControl::KeyDown(WPARAM Key)
{
	if (Key == VK_CONTROL)
	{
		IsZooming = true;
		POINT Cursor;
		GetCursorPos(&Cursor);

		CursorLast = { double(Cursor.x), double(Cursor.y) };
	}
	else
	{
		RECT WndRect;
		GetClientRect(_Base, &WndRect);

		switch (Key)
		{
		case L'O':
			OPos = { (double)WndRect.right / 2, (double)WndRect.bottom / 2 };
			break;
		case L'1':
			OPos = { 0.0, (double)WndRect.bottom };
			break;
		case '2':
			OPos = { (double)WndRect.right, (double)WndRect.bottom };
			break;
		case '3':
			OPos = { (double)WndRect.right, 0.0 };
			break;
		case '4':
			OPos = { 0.0, 0.0 };
			break;
		}

		Zoom = 1;
		RedrawWindow(_Base, nullptr, nullptr, RDW_INVALIDATE | RDW_ERASENOW);
	}
	return 0;
}
LRESULT VectorGraphControl::KeyUp(WPARAM Key)
{
	if (Key == VK_CONTROL)
		IsZooming = false;
	return 0;
}

void VectorGraphControl::ZoomThis(double By)
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