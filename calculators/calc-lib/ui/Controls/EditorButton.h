#pragma once

#include "Core\Vector.h"
#include "ControlBase.h"

namespace Mason::UI
{
	namespace Editors
	{
		class Editor;
		class EditorHost;
	}

	namespace Controls
	{
		class EditorButton : public ControlBase
		{
		private:
			Editors::Editor* Source;
			Editors::EditorHost* Host;
			bool _Clicked = false, HasMouse;

			Core::Color _BaseBk;

			static ATOM _ThisAtom;
			static void InitBase(HINSTANCE ins);
			static LRESULT __stdcall WndProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp);

		public:
			EditorButton(int XCoord, int YCoord, int Width, int Height, HWND Parent, HINSTANCE ins, Editors::Editor* Source, Editors::EditorHost* Host, Core::Color BaseBk);
			EditorButton(int XCoord, int YCoord, int Width, int Height, ControlBase* Parent, HINSTANCE ins, Editors::Editor* Source, Editors::EditorHost* Host, Core::Color BaseBk) : EditorButton(XCoord, YCoord, Width, Height, (!Parent ? NULL : *Parent), ins, Source, Host, BaseBk) {}
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
}