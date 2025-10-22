#pragma once

#include <Windows.h>
#include <string>
#include <vector>

#include "Common.h"
#include "Controls\Control.h"

#include <gdiplus.h>
#pragma comment (lib, "gdiplus.lib")

#if (!defined(_WIN32) and !defined(_WIN64)) or !defined(__cplusplus)
#error Win32 or Win64 is required, and the code must be compled in C++
#endif

#ifdef FOUNDATIONSUI_EXPORTS
#define UI_API _declspec(dllexport)
#else
#define UI_API _declspec(dllimport)
#endif

namespace Core::UI
{
#if defined(UNICODE)
	using String = std::wstring;
#else
	using String = std::string;
#endif

	using Char = CHAR;

	String UI_API GetText(HWND Window);
	void UI_API SetText(HWND Window, String New);
}