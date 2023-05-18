#include <Windows.h>
#include <math.h>

#include "Arithmetic.h"
#include "Container.h"
#include "UI\TrackedCoord.h"

Vector2d ScreenToSpace(const Vector2d& Coord, const Vector2d& Origin)
{
	return Coord - Origin;
}
Vector2d SpaceToScreen(const Vector2d& Coord, const Vector2d& Origin)
{
	return Coord + Origin;
}

LRESULT __stdcall WndProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp);

int __stdcall wWinMain(HINSTANCE ins, HINSTANCE, LPWSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSW wn = { 0 };
	wn.cbClsExtra = 0;
	wn.cbWndExtra = 0;
	wn.hInstance = ins;
	wn.hIcon = LoadIconW(NULL, IDI_APPLICATION);
	wn.hCursor = LoadCursorW(NULL, IDC_ARROW);
	wn.hbrBackground = NULL;
	wn.lpszMenuName = NULL;
	wn.lpszClassName = L"MainClass";
	wn.lpfnWndProc = WndProc;
	wn.style = CS_HREDRAW | CS_VREDRAW;

	ATOM ThisAtom = RegisterClassW(&wn);
	if (!ThisAtom)
		return 1;

	HWND Window = CreateWindowExW(0l, MAKEINTATOM(ThisAtom), L"Main", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, ins, NULL);
	if (!Window)
		return 1;

	ShowWindow(Window, nCmdShow);
	UpdateWindow(Window);

	MSG msg;
	while (GetMessageW(&msg, nullptr, 0, 0) != 0)
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}

	return msg.wParam;
}

Vector2d Location = { 100, 100 };
Vector2d Box;

class Function
{
public:
	Function() : Function(1, 0) {}
	Function(double M, double YIntercept) : M(M), YIntercept(YIntercept) {}
	Function(Vector2d Point1, Vector2d Point2)
	{
		M = (Point1.Y - Point2.Y) / (Point1.X - Point2.X);
		if (M == 0)
		{
			XEquals = Point1.Y;
			Mode = true;
		}
		else
			YIntercept = Point1.Y - (M * Point1.X);
	}

	double YIntercept;
	double M;
	double XEquals;
	bool Mode = false;

	Function Inverse() const
	{
		Function Return;

		if (M == 0)
		{
			Return.XEquals = YIntercept;
			Return.Mode = true;
		}
		else if (Mode)
		{
			Return.YIntercept = XEquals;
			Return.Mode = false;
		}
				
		Return.M = 1 / M;
		Return.YIntercept = YIntercept / M;

		return Return;
	}
	double operator()(double X) const
	{
		return M * X + YIntercept;
	}
};

bool Intersect(Vector2d& One, Vector2d& Two, Vector2d LOne, Vector2d LTwo, Vector2d CircleOrigin, double r)
{
	LOne = SpaceToScreen(LOne, CircleOrigin);
	LTwo = SpaceToScreen(LTwo, CircleOrigin);

	double dx = LOne.X - LTwo.X;
	double dy = LOne.Y - LTwo.Y;
	double D = (LOne.X * LTwo.Y) - (LTwo.X * LOne.Y);
	double Delta = (r * r) * (pow(dx, 2) + pow(dy, 2)) - pow(D, 2);
	double dr = sqrt((dx * dx) + (dy * dy));

	if (Delta < 0)
		return false;

	auto sgn = [](double X) { return (X < 0 ? -1 : 1); };

	double X1, X2, Y1, Y2;

	X1 = ((D * dy) + (sgn(dy) * dx * sqrt((r * r) * (dr * dr) - (D * D)))) / (dr * dr);
	X2 = ((D * dy) - (sgn(dy) * dx * sqrt((r * r) * (dr * dr) - (D * D)))) / (dr * dr);

	auto abs1 = [](double X) { return X < 0 ? -X : X; };

	Y1 = ((-D * dx) + (abs1(dy) * sqrt((r * r) * (dr * dr) - (D * D)))) / (dr * dr);
	Y2 = ((-D * dx) - (abs1(dy) * sqrt((r * r) * (dr * dr) - (D * D)))) / (dr * dr);

	One = { X1, Y1 };
	Two = { X2, Y2 };

	auto Transform = [](Vector2d& Other) { return Other.X * Vector2d{ 1, 0 } + Other.Y * Vector2d{ 0, -1 }; };

	//One = Transform(One);
	//Two = Transform(Two);

	One = SpaceToScreen(One, CircleOrigin);
	Two = SpaceToScreen(Two, CircleOrigin);
}

LRESULT __stdcall WndProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp)
{
	switch (Message)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps = { 0 };
		HDC Dc = BeginPaint(Window, &ps);

		RECT WndRect;
		GetClientRect(Window, &WndRect);

		HBRUSH Bk = CreateSolidBrush(0x0);
		FillRect(Dc, &WndRect, Bk);

		HPEN LightLine = CreatePen(PS_SOLID, 4, RGB(0xAC, 0x00, 0x00));
		SelectObject(Dc, LightLine);

		MoveToEx(Dc, 0, 0, NULL);
		LineTo(Dc, WndRect.right, WndRect.bottom);

		MoveToEx(Dc, int(Location.X), int(Location.Y), nullptr);

		{
			HGDIOBJ Old = SelectObject(Dc, Bk);
			HPEN BoxBorder = CreatePen(PS_SOLID, 4, RGB(0x00, 0xAC, 0x30));
			SelectObject(Dc, BoxBorder);

			int CX = int(Box.X), CY = int(Box.Y);

			RECT BoxRect{ CX - 80, CY - 80, CX + 80, CY + 80 };
			Ellipse(Dc, CX - 80, CY - 80, CX + 80, CY + 80);

			HPEN ReflectionLine = CreatePen(PS_SOLID, 4, RGB(0x00, 0xAC, 0x00));
			SelectObject(Dc, ReflectionLine);

			auto Distance = [](const Vector2d& One, const Vector2d& Two) { return sqrt(pow(One.X - Two.X, 2) + pow(One.Y - Two.Y, 2)); };

			Function Line{ Location, Box };
			Vector2d One, Two;
			bool Intersects = Intersect(One, Two, { 0.0, 0.0 }, { double(WndRect.right), double(WndRect.bottom) }, Box, 80);

			if (Intersects)
			{				
				double D1 = Distance(Location, One);
				double D2 = Distance(Location, Two);

				One = D1 <= D2 ? One : Two;
			}

			if (!Intersects)
				goto OutOfThisBlock;

			{
				HPEN NewPen = CreatePen(PS_SOLID, 3, 0xFFFFFF);
				HGDIOBJ Old = SelectObject(Dc, NewPen);

				int X = int(One.X);
				int Y = int(One.Y);

				Rectangle(Dc, X - 10, Y - 10, X + 10, Y + 10);
				SelectObject(Dc, Old);
				DeleteObject(NewPen);
			}

			Vector2d Light = ScreenToSpace(Location, One);
			Vector2d Origin = One;
			Vector2d Normal;
			{
				double Angle = 0.0;
				Vector2d RHat = { Box.X + 100, Box.Y };
				Angle = acos((Origin * RHat) / (RHat.Magnitude() * Origin.Magnitude()));

				Normal = Vector2d(Origin.X - Box.X, Origin.Y - Box.Y);

				HPEN NewPen = CreatePen(PS_SOLID, 3, 0xFFFFFF);
				HGDIOBJ Old = SelectObject(Dc, NewPen);

				int X = int(Normal.X);
				int Y = int(Normal.Y);

				POINT OldPnt;
				MoveToEx(Dc, int(One.X), int(One.Y), &OldPnt);
				Vector2d Temp = SpaceToScreen(Normal, Origin);
				LineTo(Dc, int(Temp.X), int(Temp.Y));

				MoveToEx(Dc, int(Box.X), int(Box.Y), NULL);
				LineTo(Dc, int(RHat.X), int(RHat.Y));

				MoveToEx(Dc, OldPnt.x, OldPnt.y, NULL);

				SelectObject(Dc, Old);
				DeleteObject(NewPen);
			}
			LineTo(Dc, int(One.X), int(One.Y));

			{
				Vector2d IHat = { 0, 1 };
				double NAngle = acos((Normal * IHat) / (Normal.Magnitude() * IHat.Magnitude()));
				Normal = -ScreenToSpace(Normal, Origin);
				double Angle = acos((Light * Normal) / (Light.Magnitude() * (Normal).Magnitude()));
				//Angle = (3 * PI / 2) + Angle;

				Vector2d NewLocation{ Light.Magnitude() * cos(Angle), Light.Magnitude() * sin(Angle) };
				NewLocation = SpaceToScreen(NewLocation, Origin);

				LineTo(Dc, int(NewLocation.X), int(NewLocation.Y));
				DeleteObject(ReflectionLine);
			}

			SelectObject(Dc, Old);
			DeleteObject(BoxBorder);
		}

	OutOfThisBlock:

		DeleteObject(Bk);
		EndPaint(Window, &ps);
		return 0;
	}
	case WM_CREATE:
	{
		RECT WndRect;
		GetClientRect(Window, &WndRect);
		Box = Vector2d{ WndRect.right / 2.0, WndRect.bottom / 2.0 };

		new TrackedCoord(&Location, 20, Window, RGB(0x00, 0xAC, 0xAC), 0x0, reinterpret_cast<HINSTANCE>(GetWindowLongPtrW(Window, GWLP_HINSTANCE)));
		new TrackedCoord(&Box, 20, Window, RGB(0x00, 0xAC, 0xAC), 0x0, reinterpret_cast<HINSTANCE>(GetWindowLongPtrW(Window, GWLP_HINSTANCE)));
		return 0;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProcW(Window, Message, wp, lp);
}