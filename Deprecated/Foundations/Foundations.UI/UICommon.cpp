#include "UICommon.h"

namespace Core::UI
{
	String GetText(HWND Window)
	{
		int TextLen = static_cast<int>(GetWindowTextLength(Window)) + 1;
#if defined(UNICODE)
		wchar_t* Chars = new wchar_t[TextLen];
#else
		char* Chars = new char[TextLen];
#endif

		GetWindowText(Window, Chars, TextLen);
		String Return(Chars);

		delete[] Chars;
		return Return;
	}

	void SetText(HWND Window, String New)
	{
#if defined(UNICODE)
		const wchar_t* Chars = New.c_str();
#else
		const char* Chars = New.c_str();
#endif

		SendMessage(Window, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(Chars));
	}
}