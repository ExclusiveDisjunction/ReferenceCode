#pragma once

#include "EditorCommon.h"
#include "..\UI\Controls\Control.h"
#include "..\Container.h"

namespace Core::Editors
{
	class CORE_API Editor;
	class CORE_API EditorHost;

	class CORE_API EditorButton : public UI::Controls::Control
	{
	private:
		Editor* Source;
		EditorHost* Host;
		bool _Clicked = false, HasMouse;

		AaColor _BaseBk;

		static ATOM _ThisAtom;
		static void InitBase(HINSTANCE ins);
		static LRESULT __stdcall WndProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp);

	public:
		EditorButton(int XCoord, int YCoord, int Width, int Height, HWND Parent, HINSTANCE ins, Editor* Source, EditorHost* Host, AaColor BaseBk);
		EditorButton(int XCoord, int YCoord, int Width, int Height, Control* Parent, HINSTANCE ins, Editor* Source, EditorHost* Host, AaColor BaseBk) : EditorButton(XCoord, YCoord, Width, Height, (!Parent ? NULL : *Parent), ins, Source, Host, BaseBk) {}
		~EditorButton();

		LRESULT Paint() override;
		LRESULT Click() override;
		LRESULT MouseLeave();
		LRESULT MouseMove();
		LRESULT RightClick() override;
		LRESULT TimerTick(LONG wp) override;

		Editors::Editor* Current() { return Source; }
	};

}