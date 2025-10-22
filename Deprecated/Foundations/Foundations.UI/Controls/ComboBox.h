#pragma once

#include "ControlBase.h"
#include "Win32 Common.h"

class ScrollViewer;
class Grid;
class Label;
class Button;

struct ComboBoxWndInfo;

class ComboBox : public ControlBase
{
private:
	StyleSheet _Style;
	TextStyleSheet _TextStyle;
	std::vector<String> _Items;
	UINT _Current;

	HWND _Wnd;

	bool IsPopout;
	bool FocusOnClick() const override { return false; }

	static ATOM _ThisAtom;
	static ATOM _WndAtom;
	static void InitBase(HINSTANCE ins);
	static LRESULT __stdcall ThisProc(HWND, UINT, WPARAM, LPARAM);
	static LRESULT __stdcall WndProc(HWND, UINT, WPARAM, LPARAM);
public:
	ComboBox(int X, int Y, int Width, int Height, HWND Parent, HMENU ID, HINSTANCE ins, const std::vector<String>& Items, StyleSheet Style, TextStyleSheet TextStyle);
	ComboBox(int X, int Y, int Width, int Height, ControlBase* Parent, HMENU ID, HINSTANCE ins, const std::vector<String>& Items, StyleSheet Style, TextStyleSheet TextStyle) : ComboBox(X, Y, Width, Height, *Parent, ID, ins, Items, Style, TextStyle) {}

	StyleSheet Style() { return _Style; }
	void Style(StyleSheet New)
	{
		_Style = New;

		Redraw();
	}
	TextStyleSheet TextStyle() { return _TextStyle; }
	void TextStyle(TextStyleSheet New)
	{
		_TextStyle = New;

		Redraw();
	}
	std::vector<String> Items() { return _Items; }
	void Items(const std::vector<String>& New);

	String Current();
	bool Seek(String New); // Attempts to find 'New' in the current list. If it finds it, then it will set the current value to that item.
	void UpdateDrawer();

	LRESULT Paint() override;
	LRESULT Click() override;
	LRESULT Size() override;
	LRESULT LostFocus() override;
	LRESULT Command(WPARAM wp, LPARAM lp) override;
	LRESULT OnDestroy() override;
};