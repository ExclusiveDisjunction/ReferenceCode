#pragma once

#include "Core\Vector.h"
#include "Core\DateTime.h"

#include "EditorTypes.h"
#include "EditorHost.h"
#include "..\Controls\Controls.h"
#include "..\..\Common.h"
#include "..\..\UICommon.h"

namespace Mason
{
	namespace IO
	{
		class FileInstance;
	}

	namespace UI::Editors
	{
		enum EditorStates
		{
			EDS_None = 0,
			EDS_AppendError = 1,
		};

		/// <summary>
		/// The base class for all editors. If an editor is to be written, it must inherit this class.
		/// </summary>
		class Editor
		{
		private:
			void GenerateBase(HWND Parent, RECT Margin);

			RECT _Placement = RECT();
			EditorHost* _Host = nullptr;
		protected:
			HWND _Base = nullptr;
			Core::Vector<UI::Controls::ControlBase*> MiscControls;
			UI::Controls::Button* Close = nullptr, * Move = nullptr, * _Apply = nullptr;

			bool _Loaded = false;
			const int BaseYCoord = 120;

			virtual void LoadControls() = 0;

			virtual LRESULT Paint();
			virtual LRESULT Command(WPARAM wp, LPARAM lp) = 0;
			virtual LRESULT KeyDown(WPARAM Key) = 0;
			virtual LRESULT Destroy() { return 0; }
			virtual LRESULT MouseDown() { return 0; }
			virtual LRESULT MouseUp() { return 0; }
			virtual LRESULT Size() = 0;

			void BasicPaint(const Core::String& FontName);

			virtual void LoadUpperButtons(RECT WndRect, HINSTANCE ins);
			virtual void MoveUpperButtons(RECT WndRect);
			bool ProcessUpperButtons(WPARAM wp, LPARAM lp); //Returns true if the current editor was destroyed

			static ATOM _EditorAtom;
			inline static void InitBase(HINSTANCE ins)
			{
				WNDCLASSW wn = { 0 };
				wn.lpfnWndProc = Editor::DummyProc;
				wn.hInstance = ins;
				wn.hCursor = LoadCursorW(NULL, IDC_ARROW);
				wn.hbrBackground = NULL;
				wn.lpszMenuName = NULL;
				wn.lpszClassName = L"EditorFrameClass";
				wn.hIcon = NULL;
				wn.cbClsExtra = 0;
				wn.cbWndExtra = sizeof(Editor*);
				wn.style = CS_HREDRAW | CS_VREDRAW;

				_EditorAtom = RegisterClassW(&wn);
			}

			int EditorState = EDS_None;
			Core::Vector<Controls::EditorButton*> EditorButtons;

			inline static LRESULT __stdcall DummyProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp)
			{
				return DefWindowProcW(Window, Message, wp, lp);
			}
			static LRESULT __stdcall EditorProc(Editor* This, HWND Window, UINT Message, WPARAM wp, LPARAM lp);
		public:
			friend EditorRegistry;
			friend Controls::EditorButton;
			virtual ~Editor();

			int CurrentState() const { return EditorState; }
			void CurrentState(int New);
			void ClearState() { CurrentState(0); }
			EditorHost* const& Host = _Host;

			virtual EditorTypes EditorType() const = 0;
			virtual bool IsApplyable() const = 0;
			virtual bool ConditionSpecific() const = 0;
			virtual bool Apply(IO::FileInstance* DestFile, bool PromptErrors = true) = 0; //DOES NOT CLOSE EDITOR AFTER COMPLETION
			virtual Core::Vector<void*> CondenseArgs() const = 0;
			virtual bool TestOnCondition(Core::Vector<void*> Args) const = 0;
			virtual bool EquatableTo(Editor* Other) const = 0;
			virtual Core::String GetName() const = 0;
			virtual Core::String GetNotes() const = 0;
			virtual void Reset() = 0;
			virtual WNDPROC ThisProc() const = 0;

			void BasicKeyDown(WPARAM Key);

			virtual void ReSize()
			{
				RECT WndRect;
				GetClientRect(GetParent(_Base), &WndRect);

				int XCoord = _Placement.left;
				int YCoord = _Placement.top;
				int Width = WndRect.right - _Placement.right - XCoord;
				int Height = WndRect.bottom - _Placement.bottom - YCoord;

				MoveWindow(_Base, XCoord, YCoord, Width, Height, TRUE);
			}		

			operator HWND() const
			{
				return _Base;
			}
		};
	}
}