#include "BrailleCell.h"

#include <Gdiplus.h>
using namespace Gdiplus;

BrailleCell::BrailleCell(int XCoord, int YCoord, AaColor Base, AaColor Accent, HINSTANCE ins, HWND Parent)
{
	if (!_ThisAtom)
		InitBase(ins);
	if (!_ThisAtom)
		return;

	this->Base = Base;

	int DotSize = 10;
	int DotSpace = 4;

	int Width = (DotSize + DotSpace) * 2;
	int Height = (DotSize + DotSpace) * 3;
	_Base = CreateWindowExW(0l, MAKEINTATOM(_ThisAtom), L"", WS_CHILD | WS_VISIBLE, XCoord, YCoord, Width, Height, Parent, NULL, ins, NULL);
	SetWindowLongPtrW(_Base, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

	int BDotX = 0;
	int BDotY = 0;

	One = new BrailleDot(BDotX, BDotY, DotSize, DotSize, _Base, ins, Base, Accent);
	BDotY += DotSpace = DotSize;

	Two = new BrailleDot(BDotX, BDotY, DotSize, DotSize, _Base, ins, Base, Accent);
	BDotY += DotSpace = DotSize;

	Three = new BrailleDot(BDotX, BDotY, DotSize, DotSize, _Base, ins, Base, Accent);
	BDotY += DotSpace = DotSize;

	BDotY = 0;
	BDotX = DotSize + 2;

	Four = new BrailleDot(BDotX, BDotY, DotSize, DotSize, _Base, ins, Base, Accent);
	BDotY += DotSpace = DotSize;

	Five = new BrailleDot(BDotX, BDotY, DotSize, DotSize, _Base, ins, Base, Accent);
	BDotY += DotSpace = DotSize;

	Six = new BrailleDot(BDotX, BDotY, DotSize, DotSize, _Base, ins, Base, Accent);
	BDotY += DotSpace = DotSize;
}

bool BrailleCell::IsInState(bool One, bool Four, bool Two, bool Five, bool Three, bool Six)
{
	return this->One->State() == One &&
		this->Two->State() == Two &&
		this->Three->State() == Three &&
		this->Four->State() == Four &&
		this->Five->State() == Five &&
		this->Six->State() == Six;
}

LRESULT BrailleCell::Paint()
{
	PAINTSTRUCT ps = { 0 };
	HDC Dc = BeginPaint(_Base, &ps);

	Graphics* Graph = Graphics::FromHDC(Dc);
	RECT WndRect;
	GetClientRect(_Base, &WndRect);
	Rect Area(WndRect.left, WndRect.top, WndRect.right, WndRect.bottom);

	SolidBrush Bk(Base);
	Graph->FillRectangle(&Bk, Area);

	delete Graph;
	EndPaint(_Base, &ps);
	return 0;
}
LRESULT BrailleCell::OnDestroy()
{
	CloseControl(One, Two, Three, Four, Five, Six);
	return 0;
}

ATOM BrailleCell::_ThisAtom = ATOM();
void BrailleCell::InitBase(HINSTANCE ins)
{
	WNDCLASSW wn = { 0 };
	wn.lpfnWndProc = BrailleCell::WndProc;
	wn.hInstance = ins;
	wn.hCursor = LoadCursorW(NULL, IDC_ARROW);
	wn.hbrBackground = NULL;
	wn.lpszMenuName = NULL;
	wn.lpszClassName = L"BrailleCell";
	wn.hIcon = NULL;
	wn.cbClsExtra = 0;
	wn.cbWndExtra = sizeof(BrailleCell*);
	wn.style = CS_HREDRAW | CS_VREDRAW;

	_ThisAtom = RegisterClassW(&wn);
}
