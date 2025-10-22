#pragma once

#include "ControlBase.h"
namespace Mason::UI::Controls
{
	class TextBox : public StyledControl
	{
	private:
		wchar_t* Text;
		unsigned int _Size, _Compacity;
		int _MaxComp;
		bool HasFocus = false, _Multiline = false;

		POINT CaretPos, SelectionPos;
		void AdjustCaretPos();

		SCROLLINFO si = { 0 };

		int HSUnit; //Horizontal Scroll Unit
		int VSUnit; //Vertical Scroll Unit
		int HUnitMax; //Average uppercase character height

		int NeedCWidth; //The Size of the Current text
		int NeedCHeight;

		int XPos;
		int YPos;
		void ResetScrolling();
		bool _LoadedScrolls;

		bool _HasScrolls;
		bool _IsPassword;

		HFONT Font;
		int FontHeight;

		static LRESULT __stdcall WndProc(HWND, UINT, WPARAM, LPARAM);
		static ATOM _ThisAtom;
		static void InitBase(HINSTANCE ins);

		void AppendKey(wchar_t New);
		void RemoveItem(bool IsDelete);

	public:
		TextBox(int X, int Y, int Width, int Height, HWND Parent, HINSTANCE ins, Core::String Text, StyleSheet Style, TextStyleSheet TextStyle, bool HasScrolls = false, bool IsPassword = false, int MaxCompacity = -1);
		TextBox(int X, int Y, int Width, int Height, ControlBase* Parent, HINSTANCE ins, Core::String Text, StyleSheet Style, TextStyleSheet TextStyle, bool HasScrolls = false, bool IsPassword = false, int MaxCompacity = -1) : TextBox(X, Y, Width, Height, *Parent, ins, Text, Style, TextStyle, HasScrolls, IsPassword, MaxCompacity) {}
		~TextBox();

		LRESULT KeyDown(WPARAM ID) override;
		LRESULT Command(WPARAM wp, LPARAM lp) override;
		LRESULT Paint() override;
		LRESULT Click() override;
		LRESULT LostFocus() override;
		LRESULT GotFocus() override;
		LRESULT GetDlgCode()
		{
			return DLGC_WANTALLKEYS | DLGC_WANTARROWS;
		}
		LRESULT Char(WPARAM wp) override;
		LRESULT EraseBackground();
		LRESULT VScroll(WPARAM wp, LPARAM lp);
		LRESULT HScroll(WPARAM wp, LPARAM lp);
		LRESULT MouseScroll(WPARAM wp, LPARAM lp);
		LRESULT HMouseScroll(WPARAM wp, LPARAM lp);

		Core::String GetText();
		void SetText(const Core::String& New);
	};
}