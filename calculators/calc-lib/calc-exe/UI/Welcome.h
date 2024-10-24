#pragma once

#include <Windows.h>
#include "Controls\Controls.h"
#include "Controls\StyleButton.h"
#include "..\Graphs\Graphs.h"

namespace Mason::UI
{
	class Welcome
	{
	private:
		HWND _Base;

		static ATOM _ThisAtom;
		static void InitBase(HINSTANCE ins);
		static LRESULT __stdcall WndProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp);

		RECT GraphControlRect = RECT();

		Graph::GraphControl* SplashScreen = nullptr;
		Controls::StyleButton* Minimize = nullptr, * Quit1 = nullptr;
		Controls::Button* OpenFromPath = nullptr, * OpenLastProject = nullptr;
		Controls::Label* TitleSplash = nullptr, * VersionSplash = nullptr;

		LRESULT Paint();
		LRESULT Command(WPARAM wp, LPARAM lp);

		void LoadControls();

	public:
		Welcome(HINSTANCE ins);
		Welcome(const Welcome& Obj) = delete;
		Welcome(Welcome&& Obj) noexcept = delete;
		~Welcome();

		void Open();
	};
}