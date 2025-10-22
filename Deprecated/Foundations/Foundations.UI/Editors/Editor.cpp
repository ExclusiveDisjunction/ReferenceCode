#include "Editor.h"

#include "EditorRegistry.h"
#include "EditorPopout.h"
#include "EditorButton.h"
#include "..\UI\Controls\StyleButton.h"
#include "..\UI\UICommon.h"

namespace Core::Editors
{
	
	using namespace IO;
	using namespace UI;
	using namespace UI::Controls;

	Editor::~Editor()
	{
		EditorRegistry::PopEditor(this);

		SetWindowLongPtrW(_Base, GWLP_USERDATA, 0);
		DestroyWindow(_Base);
	}

	ATOM Editor::_EditorAtom = ATOM();

	void Editor::BasicPaint(const String& FontName)
	{
		AaColor Fore = FontColor;
		AaColor Background = EditorGray;

		PAINTSTRUCT ps;
		HDC Dc = BeginPaint(_Base, &ps);

		HBRUSH Bk = CreateSolidBrush(Background);

		SelectObject(Dc, Bk);
		SetTextColor(Dc, Fore);
		SelectObject(Dc, GetStockObject(NULL_PEN));

		HFONT Font = CreateFont(-MulDiv(34, GetDeviceCaps(Dc, LOGPIXELSY), 72), 0, 0, 0, 0, false, false, false, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE, static_cast<LPCWSTR>(FontName));
		SelectObject(Dc, Font);
		SetBkMode(Dc, TRANSPARENT);

		String RawName = GetName();
		const wchar_t* Text = static_cast<const WCHAR*>(RawName);
		int TextLen = lstrlenW(Text);

		SIZE TextSize;
		GetTextExtentPoint32W(Dc, Text, TextLen, &TextSize);

		RECT WndRect;
		GetClientRect(_Base, &WndRect);

		Rectangle(Dc, WndRect.left, WndRect.top, WndRect.right, WndRect.bottom);

		RECT TextArea = { 0, 0, WndRect.right, TextSize.cy };
		DrawText(Dc, Text, TextLen, &TextArea, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		HPEN BorderPen = CreatePen(PS_SOLID, 4, Accent3);
		SelectObject(Dc, BorderPen);

		POINT Begin = { (WndRect.right / 2) - (TextSize.cx / 2) - 10, TextArea.top + TextArea.bottom }, End = { (WndRect.right / 2) + (TextSize.cx / 2) + 10, TextArea.top + TextArea.bottom };
		MoveToEx(Dc, Begin.x, Begin.y, NULL);
		LineTo(Dc, End.x, End.y);
		
		DeleteObject(Font);
		Font = CreateFont(-MulDiv(11, GetDeviceCaps(Dc, LOGPIXELSY), 72), 0, 0, 0, 0, true, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE, static_cast<LPCWSTR>(StandardFontName));
		SelectObject(Dc, Font);

		RECT Area = { 10, Begin.y + 10, WndRect.right - 20, Begin.y + 50 };
		String Notes = GetNotes();
		DrawText(Dc, static_cast<LPCWSTR>(Notes), Notes.Length(), &Area, DT_CENTER | DT_WORDBREAK);

		DeleteObject(Bk);
		DeleteObject(Font);
		DeleteObject(BorderPen);

		EndPaint(_Base, &ps);
	}

	void Editor::GenerateBase(HWND Parent, RECT Margin)
	{
		_Placement = Margin;

		HINSTANCE ins = reinterpret_cast<HINSTANCE>(GetWindowLongPtrW(Parent, GWLP_HINSTANCE));

		if (!_EditorAtom)
			InitBase(ins);

		_Base = CreateWindowExW(0l, MAKEINTATOM(_EditorAtom), NULL, WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, Parent, NULL, ins, NULL);
		SetWindowLongPtrW(_Base, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(ThisProc()));
		SetWindowLongPtrW(_Base, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

		ReSize();
		LoadControls();
	}

	void Editor::LoadUpperButtons(RECT WndRect, HINSTANCE ins)
	{
		int Width = 30;
		int Height = 30;
		int XCoord = WndRect.right - 10 - Width;
		int YCoord = 20;

		TextStyleSheet TextStyle;
		TextStyle.Foreground = 0xFFFFFFFF;
		TextStyle.FontFamily = StandardFontName;
		TextStyle.FontSize = 16;

		StyleSheet Style;
		Style.BorderBrush = 0xFFD81717;
		Style.BorderThickness = 3;
		Style.Background = 0xFF313131;
		Style.Radius = 20;

		RECT Border = { 0, 0, 0, 5 };

		Close = new StyleButton(XCoord, YCoord, Width, Height, L"X", _Base, (HMENU)1, ins, Style, TextStyle, Border);
		XCoord -= 10 + Width;

		Style.BorderBrush = Accent1;

		Move = new StyleButton(XCoord, YCoord, Width, Height, L"^", _Base, (HMENU)2, ins, Style, TextStyle, Border);
		XCoord -= 10 + Width;

		_Apply = new StyleButton(XCoord, YCoord, Width, Height, L"A", _Base, (HMENU)3, ins, Style, TextStyle, Border);
		EnableWindow(*_Apply, this->IsApplyable());
	}
	void Editor::MoveUpperButtons(RECT WndRect)
	{
		int Width = 30;
		int Height = 30;
		int XCoord = WndRect.right - 10 - Width;
		int YCoord = 20;

		if (Close)
			Close->Move(XCoord, YCoord, Width, Height);
		XCoord -= 10 + Width;

		if (Move)
			Move->Move(XCoord, YCoord, Width, Height);

		XCoord -= 10 + Width;

		if (_Apply)
			_Apply->Move(XCoord, YCoord, Width, Height);
	}
	bool Editor::ProcessUpperButtons(WPARAM wp, LPARAM lp)
	{
		switch (wp)
		{
		case 1:
			EditorRegistry::CloseEditor(this);
			return true;
		case 2:
		{
			EditorPopout* Pop = dynamic_cast<EditorPopout*>(Host);
			EditorRegistry::MoveEditor(this, nullptr, Pop == nullptr); //The second parameter is always nullptr because the function creates a popout, or uses the base window as the new host depending on the third paramter. 
			return true;
		}
		case 3:
			//Files::SessionControl::Apply(this);
			return true;
		}

		return false;
	}

	LRESULT __stdcall Editor::EditorProc(Editor* This, HWND Window, UINT Message, WPARAM wp, LPARAM lp)
	{
		switch (Message)
		{
		case WM_PAINT:
			return This->Paint();
		case WM_COMMAND:
			if (!This->ProcessUpperButtons(wp, lp))
				return This->Command(wp, lp);
			else
				return 0;
		case WM_KEYDOWN:
			This->BasicKeyDown(wp);
			return This->KeyDown(wp);
		case WM_LBUTTONDOWN:
			SetFocus(Window);
			return This->MouseDown();
		case WM_LBUTTONUP:
			return This->MouseUp();
		case WM_SIZE:
			return This->Size();
		case WM_DESTROY:
		{
			LRESULT Return = This->Destroy();
			delete This;
			return Return;
		}
		case WM_SHOWWINDOW:
		{
			for (EditorButton* Item : This->EditorButtons)
				Item->Redraw();
			return 0;
		}
		default:
			return DefWindowProcW(Window, Message, wp, lp);
		}

		return 0;
	}

	LRESULT Editor::Paint()
	{
		BasicPaint(TitleFontName);
		return 0;
	}
	void Editor::BasicKeyDown(WPARAM Key)
	{
		//Add CTRL+A for apply, CTRL+SHIFT+P popout/host, CTRL+E for exit
		if (GetKeyState(VK_CONTROL) & 0x8000)
		{
			if (Key == L'A' && !(GetKeyState(VK_SHIFT) & 0x8000))
				Command(3, 0l);
			else if (Key == L'W')
				EditorRegistry::CloseEditor(this, true);
			else if ((GetKeyState(VK_SHIFT) & 0x8000) && Key == L'P')
				Command(2, 0);
		}
	}

	void Editor::CurrentState(int New)
	{
		EditorState = New;
		for (EditorButton* Item : EditorButtons)
			Item->Redraw();
	}
}