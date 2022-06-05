#include "Arithmetic.h"

#include <Windows.h>
#include <vector>

using namespace std;

bool MouseDown, RightClick;
Vector2d Origin = { 0, 0 }, iHat = { 1, 0 }, jHat = { 0, -1 }; //Transformation things.
vector<Vector2d> TestPoints; //The points to render
double XMax, XMin, YMax, YMin; //The min and max X & Y values.

Vector2d OPos; //The Windows location of the origin, used to transform points.
Vector2d CursorLast, OPosLast; //Used for Panning.

Vector2d&& OriginToScreen(const Vector2d& Old, RECT WndRect)
{
	return Vector2d{ (Old.X * 100.0) + OPos.X, (Old.Y * 100.0) + OPos.Y};
}

LRESULT __stdcall WndProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp)
{
	switch (Message)
	{
	case WM_CREATE:
	{
		RECT WndRect;
		GetClientRect(Window, &WndRect);

		OPos.Y = WndRect.bottom / 2.0;
		OPos.X = WndRect.right / 2.0;
		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC Dc = BeginPaint(Window, &ps);

		RECT WndRect;
		GetClientRect(Window, &WndRect);
		
		HBRUSH Bk = CreateSolidBrush(0xFFFFFF);
		FillRect(Dc, &WndRect, Bk);

		DeleteObject(Bk);

		HPEN Axis = CreatePen(PS_SOLID, 13, 0xFF0000);

		auto DrawLine = [](Vector2d Origin, Vector2d Axis, HDC Dc, RECT WndRect)
		{
			if (Origin == Axis)
				return;

			Origin = OriginToScreen(Origin, WndRect);
			Axis = OriginToScreen(Axis, WndRect);

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

					MoveToEx(Dc, XIntercept, int(YOne), nullptr);
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

		DrawLine(Origin, iHat, Dc, WndRect);
		DrawLine(Origin, jHat, Dc, WndRect);

		DeleteObject(Axis);
		HPEN BoxPen = CreatePen(PS_SOLID, 5, 0x00FF00);
		SelectObject(Dc, BoxPen);

		SelectObject(Dc, GetStockObject(NULL_BRUSH));

		Vector2d TiHat = OriginToScreen(iHat, WndRect), TjHat = OriginToScreen(jHat, WndRect);
		RECT FirstRect = { long(TiHat.X) - 10, long(TiHat.Y) - 10, long(TiHat.X) + 10, long(TiHat.Y) + 10 };
		Rectangle(Dc, FirstRect.left, FirstRect.top, FirstRect.right, FirstRect.bottom);

		FirstRect = { long(TjHat.X) - 10, long(TjHat.Y) - 10, long(TjHat.X) + 10, long(TjHat.Y) + 10 };
		Rectangle(Dc, FirstRect.left, FirstRect.top, FirstRect.right, FirstRect.bottom);

		{
			Vector2d TOrigin = OriginToScreen(Origin, WndRect);
			int Distance = (int)sqrt(pow(TiHat.X - TOrigin.X, 2.0) + pow(TiHat.Y - TOrigin.Y, 2.0));

			HPEN TickPen = CreatePen(PS_SOLID, 3, 0xFF0000);
			HGDIOBJ OldPen = SelectObject(Dc, TickPen);

			for (int i = (int)TOrigin.X; i <= WndRect.right; i += Distance)
			{
				MoveToEx(Dc, i, int(TOrigin.Y) - 15, nullptr);
				LineTo(Dc, i, int(TOrigin.Y) + 15);
			}
			for (int i = (int)TOrigin.X; i >= 0; i -= Distance)
			{
				MoveToEx(Dc, i, int(TOrigin.Y) - 15, nullptr);
				LineTo(Dc, i, int(TOrigin.Y) + 15);
			}

			Distance = (int)sqrt(pow(TjHat.X - TOrigin.X, 2) + pow(TjHat.Y - TOrigin.Y, 2));

			for (int i = (int)TOrigin.Y; i > 0; i -= Distance)
			{
				MoveToEx(Dc, int(TOrigin.X) - 15, i, nullptr);
				LineTo(Dc, int(TOrigin.X) + 15, i);
			}
			for (int i = (int)TOrigin.Y; i < WndRect.bottom; i += Distance)
			{
				MoveToEx(Dc, int(TOrigin.X) - 15, i, nullptr);
				LineTo(Dc, int(TOrigin.X) + 15, i);
			}

			DeleteObject(TickPen);
			SelectObject(Dc, OldPen);
		}

		if (TestPoints.size() > 0)
		{
			MoveToEx(Dc, int(TestPoints[0].X), int(TestPoints[0].Y), nullptr);
			for (uint i = 1; i < TestPoints.size(); i++)
			{
				Vector2d TestPoint = TestPoints[i];
				Vector2d Temp = TestPoint.X * iHat + TestPoint.Y * jHat;
				Temp = OriginToScreen(Temp, WndRect);

				LineTo(Dc, int(Temp.X), int(Temp.Y));
			}

			LineTo(Dc, int(TestPoints[0].X), int(TestPoints[0].Y));
		}

		return 0;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_MOUSEWHEEL:
	{
		double Change = (double)GET_WHEEL_DELTA_WPARAM(wp);

		if (Change < 0)
			Change = 1 / (-1 * Change);

		Vector2d iBy = { Change, 0 }, jBy = { 0, Change };
		iHat = iHat.X * iBy + iHat.Y * jBy;
		jHat = jHat.X * iBy + jHat.Y * jBy;

		RedrawWindow(Window, nullptr, nullptr, RDW_INVALIDATE | RDW_ERASENOW);
		return 0;
	}

	case WM_RBUTTONDOWN:
	case WM_LBUTTONDOWN:
	{
		POINT Cursor;
		GetCursorPos(&Cursor);

		CursorLast = { double(Cursor.x), double(Cursor.y) };
		OPosLast = OPos;

		MouseDown = true;
		return 0;
	}
	case WM_MOUSEMOVE:
	{
		if (MouseDown)
		{
			if (!(GetAsyncKeyState(VK_LBUTTON) & 0x8000))
			{
				MouseDown = false;
				return 0;
			}

			POINT NewLoc;
			GetCursorPos(&NewLoc);

			double ChangeY = double(NewLoc.y) - CursorLast.Y;
			ChangeY *= 2;
			double ChangeX = double(NewLoc.x) - CursorLast.X;
			ChangeX *= 2;

			OPos = { OPosLast.X + ChangeX, OPosLast.Y + ChangeY };
			RedrawWindow(Window, NULL, NULL, RDW_ERASENOW | RDW_INVALIDATE);
		}
		return 0;
	}
	default:
		return DefWindowProcW(Window, Message, wp, lp);
	}	
}

int __stdcall wWinMain(HINSTANCE ins, HINSTANCE, LPWSTR cmdLine, int cmdShow)
{
	WNDCLASSW wn = { 0 };
	wn.lpfnWndProc = WndProc;
	wn.hInstance = ins;
	wn.hCursor = LoadCursorW(NULL, IDC_ARROW);
	wn.hbrBackground = NULL;
	wn.lpszMenuName = NULL;
	wn.lpszClassName = L"MainClass";
	wn.hIcon = LoadIconW(NULL, IDI_APPLICATION);
	wn.cbClsExtra = 0;
	wn.cbWndExtra = 0;
	wn.style = CS_HREDRAW | CS_VREDRAW;

	ATOM ThisAtom = RegisterClassW(&wn);
	if (!ThisAtom)
		return 0;

	TestPoints.clear();
	for (double i = -200; i <= 200; i += 0.2)
		TestPoints.push_back(Vector2d(i, 2 * (i * i * i) - 4 * i * i));

	HWND Window = CreateWindowExW(0l, MAKEINTATOM(ThisAtom), L"Main", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, ins, NULL);

	ShowWindow(Window, SW_MAXIMIZE);
	UpdateWindow(Window);

	MSG msg;
	while (GetMessage(&msg, 0, 0, 0) != 0)
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}

	return msg.wParam;
}