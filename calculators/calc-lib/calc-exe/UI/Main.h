#pragma once

#include "RibbonHost.h"
#include "Controls\Controls.h"
#include "Editors\EditorHost.h"
#include "Editors\EditorButtonHost.h"
#include "..\Common.h"

namespace Mason::UI
{
	namespace Editors
	{
		class EditorRegistry;
	}

	class Main : public Editors::EditorButtonHost, public FooterHost, public RibbonHost
	{
	private:
		static ATOM _ThisAtom;
		static LRESULT __stdcall WndProc(HWND, UINT, WPARAM, LPARAM);
		static void InitBase(HINSTANCE ins);

		HWND _Base;
		bool _Loaded = false;

	private:
		//Controls
		void LoadControls();

		//Messages
		LRESULT Paint();
		LRESULT OnClose();
		LRESULT Destroy();
		LRESULT Size();
		LRESULT KeyDown(WPARAM wp);
		LRESULT Timer(WPARAM wp);
		LRESULT Command(WPARAM wp, LPARAM lp);
		LRESULT GetMinMaxInfo(LPARAM lp);

		//Ribbon
		Controls::Grid* RibbonBase;

		Controls::Button* FileMsCmd, * EditMsCmd, * WindowMsCmd, * HelpMsCmd; //MsCmd 1-9
		Controls::Grid* FileGrid, * EditGrid, * WindowGrid, * HelpGrid;

		Controls::Button* OpenCmd, * NewCmd, * CloseCmd, * SaveCmd, * SaveAsCmd, * ApplyCmd, * ApplyAllCmd; //File 10-19
		//Button* SettingsCmd, *ProjectSettingsCmd, *RefrenceGroupsCmd, *QuickSearchCmd; //Edit 30 - 37
		Controls::Button* MaximizeCmd, * MinimizeCmd, * NormalSizeCmd, * SideHost, * Quit; //Window 70 - 79
		Controls::Button* AboutCmd; //Help 80-89

		void HideRibbonGrids();
		void LoadRibbon();
		void ResizeRibbon();
		void ProcessRibbon(WPARAM wp, LPARAM lp);
	public:
		Main(HINSTANCE ins);

		void Open();
		void Close();

		HWND EditorParent() const override { return _Base; }
		HWND FooterParent() const override { return _Base; }

		void SetRibbonStatus() override;

		operator HWND() const
		{
			return _Base;
		}
	};
}