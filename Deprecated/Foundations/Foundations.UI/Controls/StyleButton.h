#pragma once

#include "Button.h"
#include "Control.h"

namespace Core::UI::Controls
{
	class CORE_API StyleButton : public Button
	{
	private:
		RECT _Border;

	public:
		StyleButton(int X, int Y, int Width, int Height, String Text, HWND Parent, HMENU ID, HINSTANCE ins, StyleSheet Style, TextStyleSheet TextStyle, RECT Border, TextAlignment Alignment = TA_CenterAlignment) : Button(X, Y, Width, Height, Text, Parent, ID, ins, Style, TextStyle, Alignment) { _Border = Border; }
		StyleButton(int X, int Y, int Width, int Height, String Text, Control* Parent, HMENU ID, HINSTANCE ins, StyleSheet _Style, TextStyleSheet _Text, RECT Border, TextAlignment Alignment = TA_CenterAlignment) : StyleButton(X, Y, Width, Height, Text, *Parent, ID, ins, _Style, _Text, Border, Alignment) {}

		RECT Border() { return _Border; }
		void Border(const RECT& Obj) { _Border = Obj; Redraw(); }

		LRESULT Paint() override;
	};
}