#pragma once

#include "Control.h"

namespace Core::UI::Controls
{
	class CORE_API ImageViewer : public Control
	{
	private:
		HGLOBAL _Target;

		static LRESULT __stdcall WndProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp);
		static ATOM _ThisAtom;
		static void InitBase(HINSTANCE ins);
	public:
		ImageViewer(int XCoord, int YCoord, int Width, int Height, HWND Parent, HINSTANCE ins, HGLOBAL _Target);
		ImageViewer(int XCoord, int YCoord, int Width, int Height, Control* Parent, HINSTANCE ins, HGLOBAL _Target);

		LRESULT Paint() override;
	};
}