#include <Windows.h>
#include <gdiplus.h>
#include <math.h>
#include <vector>
#include <chrono>
#include <random>
#include <map>
#pragma comment(lib, "gdiplus.lib")

#include "BaseMath.h"
#include "Coord.h"
#include "Mapping.h"

using namespace std;
using namespace Gdiplus;

LRESULT __stdcall WndProc(HWND WIndow, UINT Message, WPARAM wp, LPARAM lp);

HINSTANCE ThisInstance = nullptr;

int __stdcall wWinMain(HINSTANCE ins, HINSTANCE, LPWSTR lpCmdLine, int nCmdShow)
{
	ThisInstance = ins;

	WNDCLASSW wn = { 0 };
	wn.lpfnWndProc = WndProc;
	wn.hInstance = ins;
	wn.hCursor = LoadCursorW(NULL, IDC_ARROW);
	wn.hbrBackground = NULL;
	wn.lpszMenuName = NULL;
	wn.lpszClassName = L"MainClass";
	wn.hIcon = LoadIconW(NULL, IDI_APPLICATION);
	wn.cbWndExtra = 0;
	wn.style = CS_HREDRAW | CS_VREDRAW;

	ATOM ThisAtom = RegisterClassW(&wn);
	if (!ThisAtom)
		return 1;

	HWND Window = CreateWindowExW(0l, MAKEINTATOM(ThisAtom), NULL, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, ins, NULL);
	if (!Window)
		return 1;

	ULONG_PTR Handle;
	GdiplusStartupInput In;
	GdiplusStartup(&Handle, &In, nullptr);

	ShowWindow(Window, SW_MAXIMIZE);
	UpdateWindow(Window);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) != 0)
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}

	GdiplusShutdown(Handle);
	return msg.wParam;
}

BezierPath* Current;
Vector<Modifyer*> Modifyers;
Vector<Anchor*> Anchors;
bool ShowAnc = true;

LRESULT __stdcall WndProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp)
{
	switch (Message)
	{
	case WM_CREATE:
	{
		Current = new BezierPath();
		Vector2d LastEnd, First;
		Anchor* LastCoord;
		Anchor* FirstCoord;

		Mapping<Anchor*, pair<Modifyer*, Modifyer*>> ToBind;

		RECT WndRect;
		GetClientRect(Window, &WndRect);

		{
			auto Temp = new CubicBezier();
			First = Temp->Begin = { 20, 20 };
			LastEnd = Temp->End = { 20, WndRect.bottom - 20.0 };
			Temp->C1 = { 40, 20 };
			LastEnd = Temp->C2 = { 20, 40 };
			Current->Paths.push_back(Temp);

			Modifyer* One, * Two;
			Anchor* A1, *A2;
			
			Anchors.Add(A1 = FirstCoord = new Anchor(&Temp->Begin, 20, Window, RGB(0x00, 0xFF, 0x00), 0x0, ThisInstance));
			Anchors.Add(A2 = LastCoord = new Anchor(&Temp->End, 20, Window, RGB(0x00, 0xFF, 0x00), 0x0, ThisInstance));
			Modifyers.Add(One = new Modifyer(&Temp->C1, 20, Window, RGB(0xFF, 0x00, 0x00), 0x0, ThisInstance));
			Modifyers.Add(Two = new Modifyer(&Temp->C2, 20, Window, RGB(0xFF, 0x00, 0x00), 0x0, ThisInstance));

			ToBind.Add(A1, pair<Modifyer*, Modifyer*>(One, nullptr));
			ToBind.Add(A2, { Two, nullptr });
		}

		{
			auto Temp = new CubicBezier();
			Temp->Begin = LastEnd;
			Temp->C1 = { 40, WndRect.bottom - 20.0 };
			Temp->C2 = { 20, WndRect.bottom - 40.0 };
			LastEnd = Temp->End = { WndRect.right - 20.0, WndRect.bottom - 20.0 };
			Current->Paths.push_back(Temp);

			Modifyer* One, * Two;
			Anchor* A1 = LastCoord, * A2;

			if (LastCoord)
				LastCoord->AddToBind(&Temp->Begin);
			Modifyers.Add(One = new Modifyer(&Temp->C1, 20, Window, 0x0000FF, 0x0, ThisInstance));
			Modifyers.Add(Two = new Modifyer(&Temp->C2, 20, Window, 0x0000FF, 0x0, ThisInstance));
			Anchors.Add(A2 = LastCoord = new Anchor(&Temp->End, 20, Window, 0x00FF00, 0x0, ThisInstance));	

			ToBind[A1].second = One;
			ToBind.Add(A2, { Two, nullptr });
		}

		{
			auto Temp = new CubicBezier();
			Temp->Begin = LastEnd;
			Temp->C1 = { WndRect.right - 40.0, WndRect.bottom - 20.0 };
			Temp->C2 = { WndRect.right - 20.0, WndRect.bottom - 40.0 };
			LastEnd = Temp->End = { WndRect.right - 20.0, 20 };
			Current->Paths.push_back(Temp);

			Anchor* A1 = LastCoord, * A2;
			Modifyer* One, * Two;

			if (LastCoord)
				LastCoord->AddToBind(&Temp->Begin);
			Anchors.Add(A2 = LastCoord = new Anchor(&Temp->End, 20, Window, RGB(0x00, 0xFF, 0x00), 0x0, ThisInstance));
			Modifyers.Add(One = new Modifyer(&Temp->C1, 20, Window, RGB(0xFF, 0x00, 0x00), 0x0, ThisInstance));
			Modifyers.Add(Two = new Modifyer(&Temp->C2, 20, Window, RGB(0xFF, 0x00, 0x00), 0x0, ThisInstance));

			ToBind[A1].second = One;
			ToBind.Add(A2, { Two, nullptr });
		}

		{
			auto Temp = new CubicBezier();
			Temp->Begin = LastEnd;
			Temp->C1 = { WndRect.right - 40.0, 20};
			Temp->C2 = { WndRect.right - 20.0, 40 };
			LastEnd = Temp->End = First;
			Current->Paths.push_back(Temp);

			Anchor* A1 = LastCoord, * A2 = FirstCoord;
			Modifyer* One, * Two;

			if (LastCoord)
				LastCoord->AddToBind(&Temp->Begin);
			if (FirstCoord)
				FirstCoord->AddToBind(&Temp->End);

			Modifyers.Add(One = new Modifyer(&Temp->C1, 20, Window, RGB(0xFF, 0x00, 0x00), 0x0, ThisInstance));
			Modifyers.Add(Two = new Modifyer(&Temp->C2, 20, Window, RGB(0xFF, 0x00, 0x00), 0x0, ThisInstance));

			ToBind[A1].second = One;
			ToBind[A2].second = Two;
		}

		/*{
			for (uint i = 0; i < ToBind.Size; i++)
			{
				MappingPair<Anchor*, pair<Modifyer*, Modifyer*>> Current = ToBind[i];
				Current.Key->Modifyers(Current.Value);
			}
		}*/
		return 0;
	}
	case WM_PAINT:
	{		
		PAINTSTRUCT ps;
		HDC Dc = BeginPaint(Window, &ps);

		RECT WndRect;
		GetClientRect(Window, &WndRect);

		HBRUSH Bk = CreateSolidBrush(0x000000);
		FillRect(Dc, &WndRect, Bk);

		if (Current)
		{
			Current->Render(Dc, WndRect, { 0xAA50AA, 0xAA50AA, 0xAA50AA, 0xAA50AA });

			HPEN LinkPen = CreatePen(PS_SOLID, 2, 0xFF0000);
			SelectObject(Dc, LinkPen);

			if (ShowAnc)
			{
				for (Bezier* Temp : Current->Paths)
				{
					if (dynamic_cast<CubicBezier*>(Temp))
					{
						CubicBezier* Conv = dynamic_cast<CubicBezier*>(Temp);
						MoveToEx(Dc, Conv->Begin.X, Conv->Begin.Y, nullptr);
						LineTo(Dc, Conv->C1.X, Conv->C1.Y);

						MoveToEx(Dc, Conv->End.X, Conv->End.Y, nullptr);
						LineTo(Dc, Conv->C2.X, Conv->C2.Y);
					}
					else if (dynamic_cast<QuadraticBezier*>(Temp))
					{
						QuadraticBezier* Conv = dynamic_cast<QuadraticBezier*>(Temp);
						MoveToEx(Dc, Conv->Begin.X, Conv->Begin.Y, nullptr);
						LineTo(Dc, Conv->C1.X, Conv->C1.Y);
					}
				}
			}
			DeleteObject(LinkPen);
		}

		DeleteObject(Bk);
		EndPaint(Window, &ps);
		return 0;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_KEYDOWN:
	{
		switch (wp)
		{
		case L'H':
			ShowAnc = !ShowAnc;
			for (uint i = 0; i < Modifyers.Size(); i++)
				ShowWindow(*Modifyers[i], ShowAnc ? SW_SHOW : SW_HIDE);
			RedrawWindow(Window, nullptr, nullptr, RDW_INVALIDATE | RDW_ERASENOW);
			break;
		}
		return 0;
	}
	default:
		return DefWindowProcW(Window, Message, wp, lp);
	}
}