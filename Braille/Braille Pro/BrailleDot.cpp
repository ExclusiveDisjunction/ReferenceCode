#include "BrailleDot.h"

#include <Gdiplus.h>
using namespace Gdiplus;

ATOM BrailleDot::_ThisAtom = ATOM();

LRESULT BrailleDot::Paint()
{
    PAINTSTRUCT ps = { 0 };
    HDC Dc = BeginPaint(_Base, &ps);

    Graphics* Graph = Graphics::FromHDC(Dc);

    RECT WndRect;
    GetClientRect(_Base, &WndRect);
    Rect Area(WndRect.left + 3, WndRect.top + 3, WndRect.right - 3, WndRect.bottom - 3);

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
        Redraw();
}
