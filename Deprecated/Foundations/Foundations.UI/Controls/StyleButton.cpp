#include "StyleButton.h"

namespace Core::UI::Controls
{
	LRESULT StyleButton::Paint()
	{
		PAINTSTRUCT ps;
		HDC Dc = BeginPaint(_Base, &ps);

		RECT WndRect;
		GetClientRect(_Base, &WndRect);

		AaColor BkColor = (_Clicked || _HasMouse ? _Style.BorderBrush : _Style.Background), BorderColor = _Style.BorderBrush, ForegroundColor = _TextStyle.Foreground;

		if (!IsEnabled)
		{
			BorderColor.Reduce(0.7f);
			ForegroundColor.Reduce(0.7f);
			BkColor.Reduce(0.7f);
		}

		HBRUSH BkBrush = CreateSolidBrush(BkColor);
		HFONT Font = CreateFontW(-MulDiv(_TextStyle.FontSize, GetDeviceCaps(Dc, LOGPIXELSY), 72), 0, 0, 0, (_TextStyle.Bold ? FW_BOLD : 0), _TextStyle.Italic, false, false, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE, static_cast<LPCWSTR>(_TextStyle.FontFamily));
		SetTextColor(Dc, ForegroundColor);
		SelectObject(Dc, BkBrush);
		SelectObject(Dc, Font);
		SetBkMode(Dc, TRANSPARENT);

		SetTextCharacterExtra(Dc, GetTextCharacterExtra(Dc) + _TextStyle.CharSpacing);
		SelectObject(Dc, GetStockObject(NULL_PEN));

		Rectangle(Dc, WndRect.left, WndRect.top, WndRect.right, WndRect.bottom);

		{
			int Size = _Border.left;
			HPEN Pen = CreatePen(PS_SOLID, Size, BorderColor);
			SelectObject(Dc, Pen);

			if (Size != 0)
			{
				MoveToEx(Dc, 0, 0, nullptr);
				LineTo(Dc, 0, WndRect.bottom);
			}

			DeleteObject(Pen);

			Size = _Border.top;
			Pen = CreatePen(PS_SOLID, Size, BorderColor);
			SelectObject(Dc, Pen);

			if (Size != 0)
			{
				MoveToEx(Dc, 0, 0, nullptr);
				LineTo(Dc, WndRect.right, 0);
			}

			DeleteObject(Pen);

			Size = _Border.right;
			Pen = CreatePen(PS_SOLID, Size, BorderColor);
			SelectObject(Dc, Pen);

			if (Size != 0)
			{
				MoveToEx(Dc, WndRect.right, 0, nullptr);
				LineTo(Dc, WndRect.right, WndRect.bottom);
			}

			DeleteObject(Pen);

			Size = _Border.bottom;
			Pen = CreatePen(PS_SOLID, Size, BorderColor);
			SelectObject(Dc, Pen);

			if (Size != 0)
			{
				MoveToEx(Dc, WndRect.right, WndRect.bottom, nullptr);
				LineTo(Dc, 0, WndRect.bottom);
			}

			DeleteObject(Pen);
		}

		const wchar_t* Text = _Text.ConstWchar();
		int TextLen = lstrlenW(Text);

		DrawTextW(Dc, Text, TextLen, &WndRect, (_Alignment == TA_CenterAlignment ? DT_CENTER : _Alignment == TA_LeftAlignment ? DT_LEFT : DT_RIGHT) | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS | DT_NOPREFIX);

		DeleteObject(BkBrush);
		DeleteObject(Font);

		EndPaint(_Base, &ps);
		return 0;
	}
}