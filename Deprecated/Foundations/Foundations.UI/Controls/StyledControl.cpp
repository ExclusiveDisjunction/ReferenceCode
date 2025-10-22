#include "Control.h"

namespace Core::UI::Controls
{
    void StyledControl::Background(const AaColor& New)
    {
        _Style.Background = New;

        Redraw();
    }
    void StyledControl::BorderBrush(const AaColor& New)
    {
        _Style.BorderBrush = New;

        Redraw();
    }
    void StyledControl::BorderThickness(int New)
    {
        _Style.BorderThickness = New;

        Redraw();
    }
    void StyledControl::SetStyle(const StyleSheet& New)
    {
        _Style = New;
        Redraw();
    }
    void StyledControl::SetTextStyle(const TextStyleSheet& New)
    {
        _TextStyle = New;
        Redraw();
    }
}