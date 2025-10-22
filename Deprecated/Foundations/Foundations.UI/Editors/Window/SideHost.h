#pragma once

#include "..\EditorCommon.h"
#include "..\Editor.h"
#include "..\EditorButtonHost.h"
#include "..\EditorButton.h"

namespace Core::Editors
{
	class CORE_API SideHost : public EditorButtonHost
	{
	private:
		HWND _Base;
		bool _Loaded;

		static ATOM _ThisAtom;
		static void InitBase(HINSTANCE ins);
		static LRESULT __stdcall WndProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp);

		void LoadControls();

		LRESULT Size();
		LRESULT Paint();
		LRESULT Close();
		LRESULT Command(WPARAM wp, LPARAM lp);
	public:
		SideHost(HINSTANCE ins);
		SideHost(const SideHost& Obj) = delete;
		SideHost(const SideHost&& Obj) = delete;
		~SideHost();

		SideHost& operator=(const SideHost& Obj) = delete;
		SideHost& operator=(const SideHost&& Obj) = delete;

		HWND EditorParent() const { return _Base; }
	};
}