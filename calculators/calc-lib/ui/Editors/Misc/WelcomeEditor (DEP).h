#pragma once

#include "..\Editor.h"
#include "..\..\Controls\Controls.h"

namespace Mason::UI::Editors::Misc
{
	class WelcomeEditor : public Editor
	{
	private:
		Controls::Grid* RecentsViewer;
		Controls::ScrollViewer* RecentsScroll;
		Core::Vector<Controls::Button*> Recents;
		Controls::Button* Open, * New, * OpenLast, * Settings, * Quit, * RemoveRecent;
		Controls::ControlList MiscControls;

		Core::StringList Paths;

		static LRESULT __stdcall WndProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp);

		void RefreshList();
	protected:
		void LoadControls() override;

		LRESULT Size() override;
		LRESULT KeyDown(WPARAM wp) override;
		LRESULT Command(WPARAM wp, LPARAM lp) override;

	public:
		WelcomeEditor();

		EditorTypes EditorType() const override { return EDT_Welcome; }
		bool IsApplyable() const override { return false; }
		bool ConditionSpecific() const override { return false; }
		bool Apply(IO::FileInstance* File, bool PromptErrors = true) override { return true; }
		Core::Vector<void*> CondenseArgs() const override { return Core::Vector<void*>(); }
		bool TestOnCondition(Core::Vector<void*> Args) const override { return true; }
		bool EquatableTo(Editor* Other) const override { return dynamic_cast<WelcomeEditor*>(Other) != nullptr; }
		Core::String GetName() const override { return L"Welcome"; }
		Core::String GetNotes() const override { return L"Please open a previous project, or create a new one to begin."; }
		WNDPROC ThisProc() const override { return WndProc; }

		void Reset() override;
	};
}