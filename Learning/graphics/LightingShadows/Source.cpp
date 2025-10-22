#include <Windows.h>
#include <math.h>

#include "Coord.h"

constexpr float PI = 3.14159265l;

LRESULT __stdcall WndProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp);

int __stdcall wWinMain(HINSTANCE Instance, HINSTANCE, LPWSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSW wn = { 0 };
	wn.lpfnWndProc = WndProc;
	wn.hInstance = Instance;
	wn.hCursor = LoadCursorW(NULL, IDC_ARROW);
	wn.hbrBackground = NULL;
	wn.lpszMenuName = NULL;
	wn.lpszClassName = L"MainClass";
	wn.hIcon = LoadIconW(NULL, IDI_APPLICATION);
	wn.cbClsExtra = 0;
	wn.cbWndExtra = 0;
	wn.style = CS_HREDRAW | CS_VREDRAW;

	ATOM ThisAtom = RegisterClassW(&wn);

	HWND Window = CreateWindowExW(0l, MAKEINTATOM(ThisAtom), L"Main", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 900, 750, NULL, NULL, Instance, NULL);

	ShowWindow(Window, SW_MAXIMIZE);
	UpdateWindow(Window);

	MSG msg;
	while (GetMessageW(&msg, nullptr, 0, 0) != 0)
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}

	return msg.wParam;
}

LRESULT __stdcall WndProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp)
{
	static POINT Ls[] = { {0, 0} };//, { 100, 100 }
	static TrackedCoord* One, * Two;
	int Current = 0;

	switch (Message)
	{
	case WM_ERASEBKGND:
		return 1;
	case WM_CREATE:
	{
		HINSTANCE ins = reinterpret_cast<HINSTANCE>(GetWindowLongPtrW(Window, GWLP_HINSTANCE));

		One = new TrackedCoord(&Ls[0], 20, Window, 0xFFAA00, 0x0, ins);
		//Two = new TrackedCoord(&Ls[1], 20, Window, 0xFFAA00, 0x0, ins);

		return 0;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC Dc = BeginPaint(Window, &ps);

		RECT WndRect = ps.rcPaint;

		HBRUSH WhiteBrush = CreateSolidBrush(0xFFFFFF);
		HBRUSH BkBrush = CreateSolidBrush(0x000000);
		HBRUSH BoxBrush = CreateSolidBrush(0xFF6060);

		RECT Boxes[] = { {150, 70, 270, 70+50}, {550, 240, 650, 340}, {80, 800, 380, 870}, { 800, 370, 830, 700}, { 430, 450, 760, 510}, { 1300, 280, 1500, 500} };

		float Radius = 400;

		FillRect(Dc, &WndRect, BkBrush);

		HRGN Shadow = NULL;
		HRGN OuterShadow = NULL;
		for (RECT Box : Boxes)
		{
			HRGN ThisLRgn = NULL, ThisOuter = NULL;

			for (POINT L : Ls)
			{
				POINT C1 = { 0 }, C2 = { 0 }, I = { 0 }, O;

				O.y = (Box.bottom - Box.top) / 2 + Box.top;
				O.x = (Box.right - Box.left) / 2 + Box.left;

				POINT A = { Box.left, Box.top }, B = { Box.right, Box.top }, C = { Box.right, Box.bottom }, D = { Box.left, Box.bottom };

				if (PtInRect(&Box, L))
					goto Finished;

				RECT PosArea = { Box.left, Box.bottom, Box.right, WndRect.bottom };
				RECT NegArea = { D.x, 0, C.x, D.y };
				RECT WestArea = { 0, Box.top, Box.left, Box.bottom };
				RECT EastArea = { Box.right, Box.top, WndRect.right, Box.bottom };

				int Quad = 0;
				if (PtInRect(&PosArea, L))
				{
					C1 = A;
					C2 = B;
					I = A;
					Quad = 2;
				}
				else if (PtInRect(&NegArea, L))
				{
					C1 = C;
					C2 = D;
					I = C;
					Quad = 3;
				}
				else if (PtInRect(&WestArea, L))
				{
					C1 = A;
					C2 = D;
					I = A;
					Quad = 2;
				}
				else if (PtInRect(&EastArea, L))
				{
					C1 = B;
					C2 = C;
					I = B;
					Quad = 1;
				}
				else if (L.x <= O.x && L.y <= O.y) // II
				{
					C1 = B;
					C2 = D;
					I = C;
					Quad = 2;
				}
				else if (L.x > O.x && L.y <= O.y) // I
				{
					C1 = A;
					C2 = C;
					I = D;
					Quad = 1;
				}
				else if (L.x > O.x && L.y > O.y) // IV
				{
					C1 = B;
					C2 = D;
					I = A;
					Quad = 4;
				}
				else if (L.x <= O.x && L.y > O.y) // III
				{
					C1 = A;
					C2 = C;
					I = B;
					Quad = 3;
				}

				auto Function = [WndRect, O, C1, C2](POINT C, POINT L, float r, int Quad, bool IsSecond) -> POINT
				{
					float DeltaY = C.y - L.y, DeltaX = C.x - L.x;

					float Slope = DeltaY / DeltaX;
					float RawAngle = atanf(sqrtf(powf(DeltaX, 2) + powf(DeltaY, 2))) * (180 / PI);

					float Angle = 0;
					if (Quad == 1 || Quad == 4)
						Angle = RawAngle + 180;
					else
						Angle = RawAngle;

					Angle *= (PI / 180);

					float K = L.y - (Slope * L.x);

					POINT Return = { 0 };
					Return.x = static_cast<LONG>(r * sin(Angle)) + C.x;

					RECT PosArea = { min(C1.x, C2.x), max(C1.y, C2.y), max(C2.x, C1.x), WndRect.bottom };
					RECT NegArea = { min(C1.x, C2.x), 0, max(C2.x, C1.x), min(C1.y, C2.y) };

					if (PtInRect(&NegArea, L) && IsSecond)
						Return.x = L.x - Return.x;
					if (PtInRect(&PosArea, L) && !IsSecond)
						Return.x = L.x - Return.x;

					Return.y = static_cast<LONG>((Slope * Return.x) + K);

					return Return;
				};

				POINT P1 = Function(C1, L, Radius + 10, Quad, false), P2 = Function(C2, L, Radius + 10, Quad, true);
				POINT P[] = { I, C1, P1, P2, C2 };

				HRGN Temp = CreatePolygonRgn(P, sizeof(P) / sizeof(POINT), ALTERNATE);

				if (!ThisLRgn)
					ThisLRgn = Temp;
				else
				{
					CombineRgn(ThisLRgn, ThisLRgn, Temp, RGN_AND);
					DeleteObject(Temp);
				}

				/*if (!ThisOuter)
					ThisOuter = Temp;
				else
				{
					CombineRgn(ThisOuter, ThisOuter, Temp, RGN_DIFF);
					DeleteObject(Temp);
				}*/
			}			

			if (!Shadow)
			{
				Shadow = CreateRectRgn(0, 0, 1, 1);
				CombineRgn(Shadow, ThisLRgn, NULL, RGN_COPY);
			}
			else
				CombineRgn(Shadow, ThisLRgn, Shadow, RGN_OR);

			/*if (!OuterShadow)
			{
				Shadow = CreateRectRgn(0, 0, 1, 1);
				CombineRgn(OuterShadow, ThisOuter, NULL, RGN_COPY);
			}
			else
				CombineRgn(OuterShadow, ThisOuter, OuterShadow, RGN_OR);*/
		}

	Finished:

		HRGN BoxArea = NULL;
		for (RECT Box : Boxes)
		{
			if (!BoxArea)
				BoxArea = CreateRectRgn(Box.left, Box.top, Box.right, Box.bottom);
			else
			{
				HRGN Temp = CreateRectRgn(Box.left, Box.top, Box.right, Box.bottom);
				CombineRgn(BoxArea, BoxArea, Temp, RGN_OR);
				DeleteObject(Temp);
			}
		}

		for (POINT L : Ls)
		{
			RECT Center = { L.x - 10, L.y - 10, L.x + 10, L.y + 10 };

			bool WithinBox = false;
			for (RECT Item : Boxes)
				WithinBox |= PtInRect(&Item, L);

			HRGN BaseCircle = CreateEllipticRgn(L.x + Radius, L.y + Radius, L.x - Radius, L.y - Radius);
			COLORREF NewColor = RGB(0x90, 0xA0, 0x00);
			HBRUSH ThisBrush = CreateSolidBrush(NewColor);
			FillRgn(Dc, BaseCircle, ThisBrush);
			DeleteObject(BaseCircle);
			DeleteObject(ThisBrush);

			if (!WithinBox && false)
			{
				for (float i = Radius; i >= 0.0f; i -= 1.0f)
				{
					HRGN Reigon = CreateEllipticRgn(L.x + i, L.y + i, L.x - i, L.y - i);

					float ColorPos = Radius - i;
					COLORREF NewColor = RGB(0x90 * (ColorPos / Radius), 0xA0 * (ColorPos / Radius), 0x00 * (ColorPos / Radius));

					HBRUSH NewBrush = CreateSolidBrush(NewColor);
					FillRgn(Dc, Reigon, NewBrush);

					DeleteObject(Reigon);
					DeleteObject(NewBrush);
				}
			}

			HBRUSH LightBrush = CreateSolidBrush(0xFFFF00);
			RECT TempArea = { L.x + 10, L.y + 10, L.x - 10, L.y - 10 };
			FillRect(Dc, &TempArea, LightBrush);
			DeleteObject(LightBrush);
		}

		HBRUSH SemiBrush = CreateSolidBrush(RGB(0x40, 0x50, 0x0));

		FillRgn(Dc, Shadow, BkBrush);
		FillRgn(Dc, OuterShadow, SemiBrush);
		FillRgn(Dc, BoxArea, BoxBrush);

		DeleteObject(SemiBrush);
		DeleteObject(WhiteBrush);
		DeleteObject(BoxBrush);
		DeleteObject(BkBrush);

		EndPaint(Window, &ps);

		return 0;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProcW(Window, Message, wp, lp);
	}
}