#include "BrailleDot.h"

#include <Gdiplus.h>
using namespace Gdiplus;

BrailleDot::BrailleDot(int XCoord, int YCoord, int Width, int Height, HWND Parent, HINSTANCE ins, AaColor Base, AaColor Accent)
{
    if (!_ThisAtom)
        InitBase(ins);

    if (!_ThisAtom)
        return;

    this->Accent = Accent;
    this->Base = Base;

    _Base = CreateWindowExW(0l, MAKEINTATOM(_ThisAtom), L"", WS_CHILD | WS_VISIBLE, XCoord, YCoord, Width, Height, Parent, NULL, ins, NULL);
    SetWindowLongPtrW(_Base, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
}

ATOM BrailleDot::_ThisAtom = ATOM();
void BrailleDot::InitBase(HINSTANCE ins)
{
    WNDCLASSW wn = { 0 };
    wn.lpfnWndProc = BrailleDot::WndProc;
    wn.hInstance = ins;
    wn.hCursor = LoadCursorW(NULL, IDC_ARROW);
    wn.hbrBackground = NULL;
    wn.lpszMenuName = NULL;
    wn.lpszClassName = L"BrailleDot";
    wn.hIcon = NULL;
    wn.cbClsExtra = 0;
    wn.cbWndExtra = sizeof(BrailleDot*);
    wn.style = CS_HREDRAW | CS_VREDRAW;

    _ThisAtom = RegisterClassW(&wn);
}

LRESULT BrailleDot::Paint()
{
    PAINTSTRUCT ps = { 0 };
    HDC Dc = BeginPaint(_Base, &ps);

    Graphics* Graph = Graphics::FromHDC(Dc);

    RECT WndRect;
    GetClientRect(_Base, &WndRect);
    Rect Area(WndRect.left, WndRect.top, WndRect.right, WndRect.bottom);

    SolidBrush Bk(_State ? Accent : Base);
    Pen Border(Accent, 3);

    Graph->FillEllipse(&Bk, Area);

    delete Graph;
    EndPaint(_Base, &ps);
    return 0;
}

void BrailleDot::State(bool New)
{
    bool Change = _State != New;
    _State = New;

    if (Change)
        RedrawWindow(_Base, NULL, NULL, RDW_ERASENOW | RDW_INVALIDATE);
}
