#pragma once

#include "..\EditorHost.h"
#include "..\..\Controls\Controls.h"
#include "..\..\..\UICommon.h"

namespace Mason::UI::Editors::Window
{
	class EditorPopout : public EditorHost
	{
	private:
		static ATOM _ThisAtom;
		static void InitBase(HINSTANCE ins);
		static LRESULT __stdcall WndProc(HWND, UINT, WPARAM, LPARAM);

		HWND _Base;

	protected:
		void ProcessOpenEditor(Editor* Editor) override;
	private:

		LRESULT Paint();
		LRESULT Command(WPARAM wp, LPARAM lp);
		LRESULT KeyDown(WPARAM Key);
		LRESULT OnClose();
		LRESULT Destroy();
		LRESULT Size();
		LRESULT GetMinMaxInfo(LPARAM lp);
	public:
		EditorPopout(HINSTANCE ins);
		~EditorPopout();

		HWND EditorParent() const override { return _Base; }
		bool SingleHostEditor() const override { return true; }

		void Close();
		void RunMsgLoop();
		operator HWND()
		{
			return _Base;
		}
	};
}