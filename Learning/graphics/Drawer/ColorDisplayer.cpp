#include "ColorDisplayer.h"

ColorDisplayer::ColorDisplayer(int XCoord, int YCoord, int Width, int Height, HWND Parent, HINSTANCE ins, AaColor BaseBk, AaColor Fill, AaColor Stroke)
{
    if (!_ThisAtom)
        InitBase(ins);

    _BaseBk = BaseBk;
    _Stroke = Stroke;
    _Fill = Fill;

    _Base = CreateWindowExW(0l, MAKEINTATOM(_ThisAtom), L"", WS_CHILD | WS_VISIBLE, XCoord, YCoord, Width, Height, Parent, NULL, ins, NULL);
    SetWindowLongPtrW(_Base, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
}

ATOM ColorDisplayer::_ThisAtom = ATOM();
void ColorDisplayer::InitBase(HINSTANCE ins)
{
    WNDCLASSW wn = { 0 };
    wn.lpfnWndProc = ColorDisplayer::WndProc;
    wn.hInstance = ins;
    wn.hCursor = LoadCursorW(NULL, IDC_ARROW);
    wn.hbrBackground = NULL;
    wn.lpszMenuName = NULL;
    wn.lpszClassName = L"ColorDisplayerClass";
    wn.hIcon = NULL;
    wn.cbClsExtra = 0;
    wn.cbWndExtra = sizeof(ColorDisplayer*);
    wn.style = CS_HREDRAW | CS_VREDRAW;

    _ThisAtom = RegisterClassW(&wn);
}
LRESULT __stdcall ColorDisplayer::WndProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp)
{
    return CommonWndProc(Window, Message, wp, lp);
}

void ColorDisplayer::SwapColors()
{
    AaColor Temp = _Stroke;
    _Stroke = _Fill;
    _Fill = Temp;

    Redraw();
}
void ColorDisplayer::SetFill(AaColor New)
{
    _Fill = New;
    Redraw();
}
void ColorDisplayer::SetStroke(AaColor New)
{
    _Stroke = New;
    Redraw();
}

LRESULT ColorDisplayer::Paint()
{
    PAINTSTRUCT ps;
    HDC Dc = BeginPaint(_Base, &ps);

    RECT WndRect;
    GetClientRect(_Base, &WndRect);

    RECT OneArea = { WndRect.left, WndRect.top, static_cast<int>(WndRect.right * 0.625), static_cast<int>(WndRect.bottom * 0.625) };
    RECT TwoArea = { static_cast<int>(WndRect.right * 0.325), static_cast<int>(WndRect.bottom * 0.325), WndRect.right, WndRect.bottom };

    RECT OneAreaBorder = { WndRect.left - 4, WndRect.top - 4, static_cast<int>(WndRect.right * 0.625) + 4, static_cast<int>(WndRect.bottom * 0.625) + 4 };

    HBRUSH Base = CreateSolidBrush(_BaseBk), Fill = CreateSolidBrush(_Fill), Stroke = CreateSolidBrush(_Stroke);

    FillRect(Dc, &WndRect, Base);
    FillRect(Dc, &TwoArea, Stroke);
    FillRect(Dc, &OneAreaBorder, Base);
    FillRect(Dc, &OneArea, Fill);

    DeleteObject(Base);
    DeleteObject(Fill);
    DeleteObject(Stroke);

    EndPaint(_Base, &ps);
    return 0;
}
