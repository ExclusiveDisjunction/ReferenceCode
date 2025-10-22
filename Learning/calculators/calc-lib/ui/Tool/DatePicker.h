#pragma once

#include "Core\Vector.h"
#include "Core\DateTime.h"

#include "..\Controls\\Controls.h"
#include "..\..\UICommon.h"

namespace Mason::UI::Tool
{
	class DatePicker
	{
	private:
		DatePicker(const Core::DateTime& Prev);
		void Construct(HINSTANCE ins);

		static ATOM _ThisAtom;
		static void InitBase(HINSTANCE ins);
		static LRESULT __stdcall WndProc(HWND, UINT, WPARAM, LPARAM);

		Core::DateTime Return;
		bool ProperReturn = false, _Loaded = false;

		HWND _Base;
		Controls::TextBox* MM, * DD, * YYYY, * HH, * MiMi;
		Controls::CheckableButton * IsAM, * IsPM;
		Controls::Button* Submit, * Cancel, * Now;
		Controls::Label* Title;
		Core::Vector<Controls::ControlBase*> MiscControls;

		bool Process();
		void LoadControls();

		LRESULT Command(WPARAM wp, LPARAM lp);
		LRESULT KeyDown(WPARAM key);
		LRESULT Paint();
	public:
		DatePicker() = delete;
		DatePicker(const DatePicker& Obj) = delete;
		DatePicker(const DatePicker&& Obj) = delete;
		~DatePicker()
		{
			SetWindowLongPtr(_Base, GWLP_USERDATA, 0);
			DestroyWindow(_Base);
		}

		static Core::DateTime Execute(const Core::DateTime& Prev, HINSTANCE ins);
		static LRESULT RunMessageLoop(DatePicker* Obj, HINSTANCE ins, WindowState* Running);
	};
}