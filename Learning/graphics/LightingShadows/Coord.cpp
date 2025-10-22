#include "Coord.h"

TrackedCoord::TrackedCoord(Coord* Bind, int Size, HWND Parent, COLORREF Border, COLORREF Bk, HINSTANCE ins)
{
	if (!_ThisAtom)
		InitBase(ins);

	Modify = Bind;
	_Track = false;
	this->Border = Border;
	this->Bk = Bk;

	int XCoord = Bind->x - (Size / 2);
	int YCoord = Bind->y - (Size / 2);

	_Base = CreateWindowExW(0l, MAKEINTATOM(_ThisAtom), L"", WS_CHILD | WS_VISIBLE, XCoord, YCoord, Size, Size, Parent, NULL, ins, NULL);
	SetWindowLongPtrW(_Base, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
}

ATOM TrackedCoord::_ThisAtom = ATOM();
void TrackedCoord::InitBase(HINSTANCE ins)
{
	WNDCLASSW wn = { 0 };
	wn.cbClsExtra = 0;
	wn.cbWndExtra = sizeof(TrackedCoord*);
	wn.hbrBackground = NULL;
	wn.hCursor = LoadCursorW(NULL, IDC_ARROW);
	wn.hIcon = NULL;
	wn.hInstance = ins;
	wn.lpfnWndProc = TrackedCoord::WndProc;
	wn.lpszClassName = L"TrackedCoord";
	wn.lpszMenuName = NULL;
	wn.style = CS_HREDRAW | CS_VREDRAW;

	_ThisAtom = RegisterClassW(&wn);
}
LRESULT __stdcall TrackedCoord::WndProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp)
{
	TrackedCoord* Item = reinterpret_cast<TrackedCoord*>(GetWindowLongPtrW(Window, GWLP_USERDATA));
	if (!Item)
		return DefWindowProcW(Window, Message, wp, lp);

	switch (Message)
	{
	case WM_ERASEBKGND:
		return 1;
	case WM_PAINT:
		return Item->Paint();
	case WM_LBUTTONDOWN:
		return Item->MouseDown();
	case WM_LBUTTONUP:
		return Item->MouseUp();
	case WM_TIMER:
		return Item->Timer(wp);
	default:
		return DefWindowProcW(Window, Message, wp, lp);
	}
}

LRESULT TrackedCoord::Paint()
{
	PAINTSTRUCT ps;
	HDC Dc = BeginPaint(_Base, &ps);

	HBRUSH Bk = CreateSolidBrush(this->Bk), Border = CreateSolidBrush(this->Border);

	RECT WndRect;
	GetClientRect(_Base, &WndRect);

	FillRect(Dc, &WndRect, Border);
	WndRect.left += 3;
	WndRect.top += 3;
	WndRect.right -= 3;
	WndRect.bottom -= 3;

	FillRect(Dc, &WndRect, Bk);

	DeleteObject(Bk);
	DeleteObject(Border);
	EndPaint(_Base, &ps);
	return 0;
}
LRESULT TrackedCoord::MouseDown()
{
	_Track = true;
	SetTimer(_Base, 1, 100, NULL);
	return 0;
}
LRESULT TrackedCoord::MouseUp()
{
	_Track = false;
	KillTimer(_Base, 1);
	return 0;
}
LRESULT TrackedCoord::Timer(WPARAM wp)
{
	if (!_Track)
		return 0;

	POINT Mouse;
	GetCursorPos(&Mouse);
	ScreenToClient(GetParent(_Base), &Mouse);
	int X = static_cast<int>(Mouse.x);
	int Y = static_cast<int>(Mouse.y);

	Coord*& Bind = Modify;
	if (Bind->y == X && Bind->y == Y)
		return 0;

	RECT ParentRect;
	GetClientRect(GetParent(_Base), &ParentRect);

	if (X < 0 || Y < 0 || X > ParentRect.right || Y > ParentRect.bottom)
		return 0;

	Bind->x = X;
	Bind->y = Y;

	RECT WndRect;
	GetClientRect(_Base, &WndRect);

	int WndX = X - (WndRect.right / 2);
	int WndY = Y - (WndRect.bottom / 2);

	MoveWindow(_Base, WndX, WndY, WndRect.right, WndRect.bottom, true);
	RedrawWindow(GetParent(_Base), NULL, NULL, RDW_INVALIDATE | RDW_ERASENOW);

	return 0;
}