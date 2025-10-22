#pragma once

#include <Windows.h>

#include "Container.h"
#include "DateTime.h"
#include "UI\Controls.h"
#include "..\Enums.h"
#include "..\Common.h"
#include "..\Files\ComponentTypes.h"
#include "..\UI\ArminControls.h"

namespace Armin
{
	namespace Files
	{
		class ArminSessionBase;
		class CompletedTask;
		class Component;
		class Image;
		class InventorySystem;
		class User;
		class UserSystem;
		class RefrenceGroup;
		class ResourceSystem;
		class Task;
		class TaskSystem;
		class TimecardEntry;
	}

	namespace Editors
	{
		//Class Defs, used to keep header count low.

		class EditorRegistry;

		/// <summary>
		/// The base class for all editors. If an editor is to be written, it must inherit this class.
		/// </summary>
		class EditorFrame
		{
		private:
			void GenerateBase(HWND Parent, RECT Margin);

			RECT _Placement = RECT();
			EditorHost* _Host = nullptr;
		protected:
			HWND _Base = nullptr;
			Vector<ControlBase*> MiscControls;
			Button* Close = nullptr, * Move = nullptr, * _Apply = nullptr;

			bool _Loaded = false;

			virtual void LoadControls() = 0;

			virtual LRESULT Paint();
			virtual LRESULT Command(WPARAM wp, LPARAM lp) = 0;
			virtual LRESULT KeyDown(WPARAM Key) = 0;
			virtual LRESULT Destroy() { return 0; }
			virtual LRESULT MouseDown() { return 0; }
			virtual LRESULT MouseUp() { return 0; }
			virtual LRESULT Size() = 0;
			virtual LRESULT SpecialCommand(HMENU ID, uint Command, LPARAM Sender) { return 0; }

			void BasicPaint(const String& FontName);

			virtual void LoadUpperButtons(RECT WndRect, HINSTANCE ins);
			virtual void MoveUpperButtons(RECT WndRect);
			bool ProcessUpperButtons(WPARAM wp, LPARAM lp); //Returns true if the current editor was destroyed

			static ATOM _EditorAtom;
			inline static void InitBase(HINSTANCE ins)
			{
				WNDCLASSW wn = { 0 };
				wn.lpfnWndProc = EditorFrame::DummyProc;
				wn.hInstance = ins;
				wn.hCursor = LoadCursorW(NULL, IDC_ARROW);
				wn.hbrBackground = NULL;
				wn.lpszMenuName = NULL;
				wn.lpszClassName = L"EditorFrameClass";
				wn.hIcon = NULL;
				wn.cbClsExtra = 0;
				wn.cbWndExtra = sizeof(EditorFrame*);
				wn.style = CS_HREDRAW | CS_VREDRAW;

				_EditorAtom = RegisterClassW(&wn);
			}

			inline static LRESULT __stdcall DummyProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp)
			{
				return DefWindowProcW(Window, Message, wp, lp);
			}
			static LRESULT __stdcall EditorProc(EditorFrame* This, HWND Window, UINT Message, WPARAM wp, LPARAM lp);
		public:
			friend EditorRegistry;
			virtual ~EditorFrame();

			EditorHost* const& Host = _Host;
			virtual EditorTypes EditorType() const = 0;
			virtual bool IsApplyable() const = 0;
			virtual bool ConditionSpecific() const = 0;
			virtual bool Apply(Files::ArminSessionBase* DestFile, bool PromptErrors = true) = 0; //DOES NOT CLOSE EDITOR AFTER COMPLETION
			virtual Vector<void*> CondenseArgs() const = 0;
			virtual bool TestOnCondition(Vector<void*> Args) const = 0;
			virtual bool EquatableTo(EditorFrame* Other) const = 0;
			virtual String GetName() const = 0;
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

		namespace Tasks
		{
			class TasksEditor : public EditorFrame
			{
			private:
				Files::TaskSystem* _System;

				Label* ObjectCount;
				Button* Search, * Add, * Remove, * View, * Edit, * SelectAll, * DeSelectAll, * CompleteTask;
				ScrollViewer* ObjectScroll;
				Grid* ObjectView;
				Vector<UI::ComponentViewer*> Objects;

				void FillObjects();

				static LRESULT __stdcall WndProc(HWND, UINT, WPARAM, LPARAM);

			protected:
				void LoadControls() override;

				LRESULT Command(WPARAM wp, LPARAM lp) override;
				LRESULT KeyDown(WPARAM Key) override;
				LRESULT Size() override;
			public:
				TasksEditor(Files::TaskSystem* System);

				bool IsApplyable() const override { return false; }
				bool ConditionSpecific() const override { return true; }
				bool Apply(Files::ArminSessionBase* File, bool PromptErrors = true) override { return true; }
				EditorTypes EditorType() const override { return EDT_Tasks; }
				Vector<void*> CondenseArgs() const override { return _System; }
				bool TestOnCondition(Vector<void*> Args) const override { return Args.Size == 1 && Args[0] == _System; }
				bool EquatableTo(EditorFrame* Other) const override { return dynamic_cast<TasksEditor*>(Other) != nullptr && TestOnCondition(Other->CondenseArgs()); }
				String GetName() const override { return L"Tasks"; }
				WNDPROC ThisProc() const override { return WndProc; }

				void Reset() override;
			};
			class ViewTaskEditor : public EditorFrame
			{
			private:
				Files::Task* Source;

				bool _EditMode, _DummySelect;

				Button* CompleteTask, * EditThis;
				Label* Title, * Instructions, * DueBy, * RequiresAssurance;
				ScrollViewer* AssignedScroll;
				Grid* AssignedView;
				Vector<UI::ComponentViewer*> AssignedObjects;

				static LRESULT __stdcall WndProc(HWND, UINT, WPARAM, LPARAM);

				void FillAssigned();
			protected:
				void LoadControls() override;

				LRESULT Command(WPARAM wp, LPARAM lp) override;
				LRESULT KeyDown(WPARAM Key) override;
				LRESULT Size() override;
				LRESULT Destroy() override;
				LRESULT SpecialCommand(HMENU ID, uint Command, LPARAM Sender) override;
			public:
				ViewTaskEditor(Files::Task* Target, bool EditMode = true);

				bool IsApplyable() const override { return false; }
				bool ConditionSpecific() const override { return true; }
				bool Apply(Files::ArminSessionBase* File, bool PrompErrors = true) override { return true; }
				EditorTypes EditorType() const override { return EDT_ViewTask; }
				Vector<void*> CondenseArgs() const override { return { Source }; }
				bool TestOnCondition(Vector<void*> Args) const override;
				bool EquatableTo(EditorFrame* Other) const override;
				String GetName() const override { return L"View Task"; }
				WNDPROC ThisProc() const override { return WndProc; }

				void Reset() override;
			};
			class EditTaskEditor : public EditorFrame
			{
			private:
				Files::Task* Target;
				DateTime DueByD;

				TextBox* Title, * Instructions;
				Label* DueBy;
				CheckableButton* RequiresAssurance;
				Button* DueBySelect, * ModifyAssigned, * ViewAssigned;
				ScrollViewer* AssignedUsersScroll;
				Grid* AssignedUsersView;
				Vector<UI::ComponentViewer*> AssignedUsers;

				bool _Multiselect = false;
			protected:
				void LoadControls() override;
				void FillAssigned();

				LRESULT Command(WPARAM wp, LPARAM lp) override;
				LRESULT KeyDown(WPARAM Key) override;
				LRESULT Size() override;

				static LRESULT __stdcall WndProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp);
			public:
				EditTaskEditor(Files::Task* Target);

				bool IsApplyable() const override { return true; }
				bool ConditionSpecific() const override { return true; }
				bool Apply(Files::ArminSessionBase* File, bool PrompErrors = true) override;
				EditorTypes EditorType() const override { return EDT_EditTask; }
				Vector<void*> CondenseArgs() const override { return { Target }; }
				bool TestOnCondition(Vector<void*> Args) const override;
				bool EquatableTo(EditorFrame* Other) const override { return false; }
				String GetName() const override { return L"Edit Task"; }
				WNDPROC ThisProc() const override { return WndProc; }

				void Reset() override;
			};
			class AddTaskEditor : public EditorFrame
			{
			private:
				TextBox* Title, * Instructions;
				ScrollViewer* AssignedScroll;
				Grid* AssignedView;
				Vector<UI::ComponentViewer*> AssignedTo;
				CheckableButton* RequiresAssurance;
				Label* DueBy;
				Button* DueBySelect; //ID: 4
				Button* ModifyAssigned, * Multiselect;

				DateTime DueByD;
				bool _IsMultiselect;

				static LRESULT __stdcall WndProc(HWND, UINT, WPARAM, LPARAM);

			protected:
				void LoadControls() override;

				LRESULT Command(WPARAM wp, LPARAM lp) override;
				LRESULT KeyDown(WPARAM Key) override;
				LRESULT Size() override;
			public:
				AddTaskEditor();

				bool IsApplyable() const override { return true; }
				bool ConditionSpecific() const override { return false; }
				bool Apply(Files::ArminSessionBase* File, bool PromptErrors = true) override;
				EditorTypes EditorType() const override { return EDT_AddTask; }
				Vector<void*> CondenseArgs() const override { return {}; }
				bool TestOnCondition(Vector<void*> Args) const override;
				bool EquatableTo(EditorFrame* Other) const override { return false; }
				String GetName() const override { return L"Add Task"; }
				WNDPROC ThisProc() const override { return WndProc; }

				void Reset() override;
			};

			class CompletedTasksEditor : public EditorFrame
			{
			private:
				Files::TaskSystem* _System;

				Label* ObjectCount;
				TextBox* Criteria;
				Button* GoToTasks, * View, *SelectAll, *DeSelectAll;
				ScrollViewer* ObjectScroll;
				Grid* ObjectView;
				Vector<UI::ComponentViewer*> Objects;

				void FillObjects();

				static LRESULT __stdcall WndProc(HWND, UINT, WPARAM, LPARAM);

			protected:
				void LoadControls() override;

				LRESULT Command(WPARAM wp, LPARAM lp) override;
				LRESULT KeyDown(WPARAM Key) override;
				LRESULT Size() override;
			public:
				CompletedTasksEditor(Files::TaskSystem* System);

				bool IsApplyable() const override { return false; }
				bool ConditionSpecific() const override { return true; }
				bool Apply(Files::ArminSessionBase* File, bool PromptErrors = true) override { return true; }
				EditorTypes EditorType() const override { return EDT_CompletedTasks; }
				Vector<void*> CondenseArgs() const override { return Vector<void*>(); }
				bool TestOnCondition(Vector<void*> Args) const override { return Args.Size == 1 && Args[0] == _System; }
				bool EquatableTo(EditorFrame* Other) const override { return dynamic_cast<CompletedTasksEditor*>(Other) != nullptr && TestOnCondition(Other->CondenseArgs()); }
				String GetName() const override { return L"Completed Tasks"; }
				WNDPROC ThisProc() const override { return WndProc; }

				void Reset() override;
			};
			class CompleteTaskEditor : public EditorFrame
			{
			private:
				Files::Task* _Target;

				DateTime DateCompletedD;
				Label* DateCompleted;
				Button* DateCompletedSelect;

				UI::ComponentViewer* AssuredBy, * TargetTask;
				TextBox* ActionReport;
				
				Vector<Files::Component*> CompletedByD;
				Label* CompletedBy;
				Button* CompletedBySelect, *ViewCompletedBy;

				Vector<Files::Component*> OperRefD;
				Label* OperRef;
				Button* OperRefSelect, *ViewRefs;

				static LRESULT __stdcall WndProc(HWND, UINT, WPARAM, LPARAM);
			protected:
				void LoadControls() override;

				LRESULT Command(WPARAM wp, LPARAM lp) override;
				LRESULT KeyDown(WPARAM Key) override;
				LRESULT Size() override;
			public:
				CompleteTaskEditor(Files::Task* Target);

				bool IsApplyable() const override { return true; }
				bool ConditionSpecific() const override { return true; }
				bool Apply(Files::ArminSessionBase* File, bool PromptErrors = true) override;
				EditorTypes EditorType() const override { return EDT_CompleteTask; }
				Vector<void*> CondenseArgs() const override { return { _Target }; }
				bool TestOnCondition(Vector<void*> Args) const override;
				bool EquatableTo(EditorFrame* Other) const override { return false; }
				String GetName() const override { return L"Complete Task"; }
				WNDPROC ThisProc() const override { return WndProc; }

				void Reset() override;
			};
			class ViewCompletedTaskEditor : public EditorFrame
			{
			private:
				Files::CompletedTask* Target;

				Label* DateCompleted;
				UI::ComponentViewer* AssuredBy, * Task;
				Label* CompletedBy, * References;
				Button* ViewCompleted, * ViewReferences;
				Label* ActionsDone;

				static LRESULT __stdcall WndProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp);
			protected:
				void LoadControls() override;

				LRESULT Command(WPARAM wp, LPARAM lp) override;
				LRESULT KeyDown(WPARAM Key) override;
				LRESULT Size() override;
				
			public:
				ViewCompletedTaskEditor(Files::CompletedTask* Target);

				bool IsApplyable() const override { return false; }
				bool ConditionSpecific() const override { return true; }
				bool Apply(Files::ArminSessionBase* File, bool PrompErrors = true) override { return true; }
				EditorTypes EditorType() const override { return EDT_ViewCompletedTask; }
				Vector<void*> CondenseArgs() const override { return { Target }; }
				bool TestOnCondition(Vector<void*> Args) const override;
				bool EquatableTo(EditorFrame* Other) const override;
				String GetName() const override { return L"View Completed Task"; }
				WNDPROC ThisProc() const override { return WndProc; }

				void Reset() override;
			};
		}

		namespace Users
		{
			class JobPositionsEditor : public EditorFrame
			{
			private:
				Files::UserSystem* _System;

				Label* ObjectCount;
				Button* Add, * Remove, * View, * Edit, * SelectAll, * DeSelectAll, * Search;
				ScrollViewer* ObjectScroll;
				Grid* ObjectView;
				Vector<UI::ComponentViewer*> Objects;

				void FillObjects();

				static LRESULT __stdcall WndProc(HWND, UINT, WPARAM, LPARAM);

			protected:
				void LoadControls() override;

				LRESULT Command(WPARAM wp, LPARAM lp) override;
				LRESULT KeyDown(WPARAM Key) override;
				LRESULT Size() override;
			public:
				JobPositionsEditor(Files::UserSystem* System);

				EditorTypes EditorType() const override { return EDT_JobPositions; }
				bool IsApplyable() const override { return false; }
				bool ConditionSpecific() const override { return true; }
				bool Apply(Files::ArminSessionBase* File, bool PromptErrors = true) override { return true; }
				Vector<void*> CondenseArgs() const override { return _System; }
				bool TestOnCondition(Vector<void*> Args) const override { return Args.Size != 0 && Args[0] == _System; }
				bool EquatableTo(EditorFrame* Other) const override { return dynamic_cast<JobPositionsEditor*>(Other) != nullptr && TestOnCondition(Other->CondenseArgs()); }
				String GetName() const override { return L"Job Positions"; }
				WNDPROC ThisProc() const override { return WndProc; }

				void Reset() override;
			};
			class AddJobPositionEditor : public EditorFrame
			{
			private:
				TextBox* Title, * Description;

				static LRESULT __stdcall WndProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp);

			protected:
				void LoadControls() override;

				LRESULT Command(WPARAM wp, LPARAM lp) override;
				LRESULT KeyDown(WPARAM Key) override;
				LRESULT Size() override;
				LRESULT Destroy() override;
			public:
				AddJobPositionEditor();

				EditorTypes EditorType() const override { return EDT_AddJobPosition; }
				bool IsApplyable() const override { return true; }
				bool ConditionSpecific() const override { return false; }
				bool Apply(Files::ArminSessionBase* File, bool PromptErrors = true) override;
				Vector<void*> CondenseArgs() const override { return Vector<void*>(); }
				bool TestOnCondition(Vector<void*> Args) const override;
				bool EquatableTo(EditorFrame* Other) const override { return false; }
				String GetName() const override { return L"Add Job Position"; }
				WNDPROC ThisProc() const override { return WndProc; }

				void Reset() override;
			};

			class UsersEditor : public EditorFrame
			{
			private:
				Files::UserSystem* _System;

				Label* UserCount;
				UI::ComponentViewer* CurrentUser;
				Button* Search, * SignOut, * Add, * Remove, * Edit, * View, *SelectAll, *DeSelectAll;
				Grid* ObjectView;
				ScrollViewer* ObjectScroll;
				Vector<UI::ComponentViewer*> Objects;

				void FillObjects();

				static LRESULT __stdcall WndProc(HWND, UINT, WPARAM, LPARAM);

			protected:
				void LoadControls() override;

				LRESULT Command(WPARAM wp, LPARAM lp) override;
				LRESULT KeyDown(WPARAM Key) override;
				LRESULT Size() override;
			public:
				UsersEditor(Files::UserSystem* System);

				EditorTypes EditorType() const override { return EDT_Users; }
				bool IsApplyable() const override { return false; }
				bool ConditionSpecific() const override { return true; }
				bool Apply(Files::ArminSessionBase* DestFile, bool PropmtErrors = true) override { return true; }
				Vector<void*> CondenseArgs() const override { return _System; }
				bool TestOnCondition(Vector<void*> Args) const override { return Args.Size != 0 && Args[0] == _System; }
				bool EquatableTo(EditorFrame* Other) const override { return dynamic_cast<UsersEditor*>(Other) != nullptr && TestOnCondition(Other->CondenseArgs()); }
				String GetName() const override { return L"Users"; }
				WNDPROC ThisProc() const override { return WndProc; }

				void Reset() override;
			};
			class ViewUserEditor : public EditorFrame
			{
			private:
				Label* Username, * Password, * FirstName, * MiddleName, * LastName, * UserType;
				ScrollViewer* PositionScroll;
				Grid* PositionView;
				Button* Change, * Timecards, *Homepage;

				Vector<UI::ComponentViewer*> Positions;

				Files::User* Current;

				static LRESULT __stdcall WndProc(HWND, UINT, WPARAM, LPARAM);

			protected:
				void LoadControls() override;

				LRESULT Command(WPARAM wp, LPARAM lp) override;
				LRESULT KeyDown(WPARAM Key) override;
				LRESULT Size() override;
				LRESULT Destroy() override;
				LRESULT SpecialCommand(HMENU ID, uint Command, LPARAM Sender) override;
			public:
				ViewUserEditor(Files::User* Target);
				~ViewUserEditor();

				EditorTypes EditorType() const override { return EDT_ViewUser; }
				bool IsApplyable() const override { return false; }
				bool ConditionSpecific() const override { return true; }
				bool Apply(Files::ArminSessionBase* DestFile, bool PropmtErrors = true) override { return true; }
				Vector<void*> CondenseArgs() const override { return { Current }; }
				bool TestOnCondition(Vector<void*> Args) const override;
				bool EquatableTo(EditorFrame* Other) const override;
				String GetName() const override { return L"View User"; }
				WNDPROC ThisProc() const override { return WndProc; }

				void Reset() override;
			};
			class EditUserEditor : public EditorFrame
			{
			private:
				TextBox* Username, * Password, * FirstName, * MiddleName, * LastName;
				ComboBox* UserType;
				ScrollViewer* PositionScroll;
				Grid* PositionView;
				Button* ModifyPositions, * ViewPosition;
				Vector<UI::ComponentViewer*> Positions;

				bool _Multiselect;
				Files::User* Current;

				static LRESULT __stdcall WndProc(HWND, UINT, WPARAM, LPARAM);

			protected:
				void LoadControls() override;

				LRESULT Command(WPARAM wp, LPARAM lp) override;
				LRESULT KeyDown(WPARAM Key) override;
				LRESULT Size() override;

			public:
				EditUserEditor(Files::User* Target);

				EditorTypes EditorType() const override { return EDT_EditUser; }
				bool IsApplyable() const override { return true; }
				bool ConditionSpecific() const override { return true; }
				bool Apply(Files::ArminSessionBase* File, bool PrmoptErrors = true) override;
				Vector<void*> CondenseArgs() const override { return { Current }; }
				bool TestOnCondition(Vector<void*> Args) const override;
				bool EquatableTo(EditorFrame* Other) const override;
				String GetName() const override { return L"Edit User"; }
				WNDPROC ThisProc() const override { return WndProc; }

				void Reset() override;
			};
			class CreateUserEditor : public EditorFrame
			{
			private:
				static LRESULT __stdcall WndProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp);

				TextBox* Username, * Password, * FirstName, * MiddleName, * LastName;
				Button* ModifyPositions, * ViewPosition;

				ComboBox* UserType;
				ScrollViewer* PositionsScroll;
				Grid* PositionsView;
				Vector<UI::ComponentViewer*> Positions;

			protected:
				void LoadControls() override;

				LRESULT Command(WPARAM wp, LPARAM lp) override;
				LRESULT KeyDown(WPARAM Key) override;
				LRESULT Size() override;

			public:
				CreateUserEditor();

				EditorTypes EditorType() const override { return EDT_CreateUser; }
				bool IsApplyable() const override { return true; }
				bool ConditionSpecific() const override { return false; }
				bool Apply(Files::ArminSessionBase* File, bool PromptErrors = true) override;
				Vector<void*> CondenseArgs() const override { return Vector<void*>(); }
				bool TestOnCondition(Vector<void*> Args) const override { return false; }
				bool EquatableTo(EditorFrame* Other) const override { return false; }
				String GetName() const override { return L"Create User"; }
				WNDPROC ThisProc() const override { return WndProc; }

				void Reset() override;
			};

			class UserHomepageEditor : public EditorFrame
			{
			private:
				Files::User* _Target;

				UI::ComponentViewer* CurrentUser;
				ScrollViewer* ObjectScroll;
				Grid* ObjectView;
				Vector<UI::ComponentViewer*> Objects;
				Button* SignOut, * Lock, * GoToTasks, * Timecard;
				Button* CompleteTask, * View, * Edit, *SelectAll, *DeSelectAll;

				static LRESULT __stdcall WndProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp);
			protected:
				void LoadControls() override;
				void FillObjects();

				LRESULT Size() override;
				LRESULT KeyDown(WPARAM wp) override;
				LRESULT Command(WPARAM wp, LPARAM lp) override;
			public:
				UserHomepageEditor(Files::User* Target);

				EditorTypes EditorType() const override { return EDT_UserHomepage; }
				bool IsApplyable() const override { return false; }
				bool ConditionSpecific() const override { return true; }
				bool Apply(Files::ArminSessionBase* DestFile, bool PropmtErrors = true) override { return true; }
				Vector<void*> CondenseArgs() const override { return { _Target }; }
				bool TestOnCondition(Vector<void*> Args) const override;
				bool EquatableTo(EditorFrame* Other) const override;
				String GetName() const override { return L"Homepage"; }
				WNDPROC ThisProc() const override { return WndProc; }

				void Reset() override;
			};
			class TimecardsEditor : public EditorFrame
			{
			private:
				Files::User* _TargetUser;
				
				Button* NextMonth, * LastMonth;
				Label* CurrentMonth;
				Vector<CalendarGrid*> Grids;
				int _ThisMonth, _ThisYear;

				void SetupMonth();
				void FillRecords();

				static LRESULT __stdcall WndProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp);
			protected:
				void LoadControls() override;

				LRESULT Size() override;
				LRESULT KeyDown(WPARAM wp) override;
				LRESULT Command(WPARAM wp, LPARAM lp) override;
			public:
				TimecardsEditor(Files::User* Target);

				EditorTypes EditorType() const override { return EDT_Timecards; }
				bool IsApplyable() const override { return false; }
				bool ConditionSpecific() const override { return true; }
				bool Apply(Files::ArminSessionBase* DestFile, bool PropmtErrors = true) override { return true; }
				Vector<void*> CondenseArgs() const override;
				bool TestOnCondition(Vector<void*> Args) const override;
				bool EquatableTo(EditorFrame* Other) const override;
				String GetName() const override { return L"Timecards"; }
				WNDPROC ThisProc() const override { return WndProc; }

				void Reset() override;
			};

			class UserSearch : public EditorFrame
			{
			private:
				Files::UserSystem* _System;

				void RunSearch();

				TextBox* Username, * FirstName, * LastName;
				CheckableButton* Admin, * Assurance, * Standard;
				Button* Search, * SaveResult, * DuplicateResult, * View, * Edit, * SelectAll, * DeSelectAll;

				ScrollViewer* ObjectScroll;
				Grid* ObjectView;
				Vector<UI::ComponentViewer*> Objects;

				static LRESULT __stdcall WndProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp);
			protected:
				void LoadControls() override;

				LRESULT Size() override;
				LRESULT KeyDown(WPARAM wp) override;
				LRESULT Command(WPARAM wp, LPARAM lp) override;
			public:
				UserSearch(Files::UserSystem* System);

				EditorTypes EditorType() const override { return EDT_UserSearch; }
				bool IsApplyable() const override { return false; }
				bool ConditionSpecific() const override { return true; }
				bool Apply(Files::ArminSessionBase* File, bool PromptErrors = true) override { return true; }
				Vector<void*> CondenseArgs() const override { return _System; }
				bool TestOnCondition(Vector<void*> Args) const override { return Args.Size == 1 && Args[0] == _System; }
				bool EquatableTo(EditorFrame* Other) const override { return dynamic_cast<UserSearch*>(Other) != nullptr && TestOnCondition(Other->CondenseArgs()); }
				String GetName() const override { return L"User Search"; }
				WNDPROC ThisProc() const override { return WndProc; }

				void Reset() override;
			};
		}

		namespace Inventory
		{
			class InventoryEditor : public EditorFrame
			{
			private:
				Files::InventorySystem* _System;

				Label* InventorySize = nullptr;
				Button* ImportCSV = nullptr, *InvSearch = nullptr, * Add = nullptr, * Remove = nullptr, * Edit = nullptr, * View = nullptr, *SelectAll = nullptr, *DeSelectAll = nullptr;
				Grid* ObjectView = nullptr;
				ScrollViewer* ObjectScroll = nullptr;
				Vector<UI::ComponentViewer*> Objects;

				static LRESULT __stdcall WndProc(HWND, UINT, WPARAM, LPARAM);
			protected:
				void LoadControls() override;
				void FillObjects();

				LRESULT Command(WPARAM wp, LPARAM lp) override;
				LRESULT KeyDown(WPARAM Key) override;
				LRESULT Size() override;
			public:
				InventoryEditor(Files::InventorySystem* System);

				EditorTypes EditorType() const override { return EDT_Inventory; }
				bool IsApplyable() const override { return false; }
				bool ConditionSpecific() const override { return true; }
				bool Apply(Files::ArminSessionBase* File, bool PromptErrors = true) override { return true; }
				Vector<void*> CondenseArgs() const override { return _System; }
				bool TestOnCondition(Vector<void*> Args) const override { return Args.Size != 0 && Args[0] == _System; }
				bool EquatableTo(EditorFrame* Other) const override;
				String GetName() const override { return L"Inventory"; }
				WNDPROC ThisProc() const override { return WndProc; }

				void Reset() override;
			};
			class OperationInventoryEditor : public EditorFrame
			{
			private:
				Files::InventorySystem* _System;

				Label* InventorySize;
				Button* Add, * Remove, * View, * Edit, * Search, * SelectAll, * DeSelectAll;
				Grid* ObjectView;
				ScrollViewer* ObjectScroll;
				Vector<UI::ComponentViewer*> Objects;

				void FillObjects();

				static LRESULT __stdcall WndProc(HWND, UINT, WPARAM, LPARAM);
			protected:
				void LoadControls() override;

				LRESULT Command(WPARAM wp, LPARAM lp) override;
				LRESULT KeyDown(WPARAM Key) override;
				LRESULT Size() override;
			public:
				OperationInventoryEditor(Files::InventorySystem* System);

				EditorTypes EditorType() const override { return EDT_OperationInventory; }
				bool IsApplyable() const override { return false; }
				bool ConditionSpecific() const override { return true; }
				bool Apply(Files::ArminSessionBase* DestFile, bool PropmtErrors = true) override { return true; }
				Vector<void*> CondenseArgs() const override { return _System; }
				bool TestOnCondition(Vector<void*> Args) const override { return Args.Size != 0 && Args[0] == _System; }
				bool EquatableTo(EditorFrame* Other) const override { return dynamic_cast<OperationInventoryEditor*>(Other) != nullptr && TestOnCondition(Other->CondenseArgs()); }
				String GetName() const override { return L"Operation Inventory"; }
				WNDPROC ThisProc() const override { return WndProc; }

				void Reset() override;
			};
			class AddInventoryItemEditor : public EditorFrame
			{
			private:
				TextBox* SerialNumber, * Description, * Group;
				CheckableButton* IsInPossession;
				Button* SelectGroup, *SelectImage;
				UI::ComponentViewer* TargetImage;

				static LRESULT __stdcall WndProc(HWND, UINT, WPARAM, LPARAM);
			protected:
				void LoadControls() override;

				LRESULT Command(WPARAM wp, LPARAM lp) override;
				LRESULT KeyDown(WPARAM Key) override;
				LRESULT Size() override;
				LRESULT Destroy() override;
			public:
				AddInventoryItemEditor();

				EditorTypes EditorType() const override { return EDT_AddInventoryItem; }
				bool IsApplyable() const override { return true; }
				bool ConditionSpecific() const override { return false; }
				bool Apply(Files::ArminSessionBase* DestFile, bool PromptErrors = true) override;
				Vector<void*> CondenseArgs() const override { return Vector<void*>(); }
				bool TestOnCondition(Vector<void*> Args) const override;
				bool EquatableTo(EditorFrame* Other) const override { return false; }
				String GetName() const override { return L"Add Inventory Item"; }
				WNDPROC ThisProc() const override { return WndProc; }

				void Reset() override;
			};
			class AddOperationInventoryItemEditor : public EditorFrame
			{
			private:
				TextBox* SerialNumber, * Description, * Group;
				CheckableButton* WorkingOrder, * PartialWorkingOrder, * Broken, * UnderRepair, * NotInPossession;
				Button* SelectGroup, * SelectImage;
				UI::ComponentViewer* TargetImage;

				static LRESULT __stdcall WndProc(HWND, UINT, WPARAM, LPARAM);
			protected:
				void LoadControls() override;

				LRESULT Command(WPARAM wp, LPARAM lp) override;
				LRESULT KeyDown(WPARAM Key) override;
				LRESULT Size() override;
				LRESULT Destroy() override;
			public:
				AddOperationInventoryItemEditor();

				EditorTypes EditorType() const override { return EDT_AddOperationInventoryItem; }
				bool IsApplyable() const override { return true; }
				bool ConditionSpecific() const override { return false; }
				bool Apply(Files::ArminSessionBase* DestFile, bool PromptErrors = true) override;
				Vector<void*> CondenseArgs() const override { return Vector<void*>(); }
				bool TestOnCondition(Vector<void*> Args) const override;
				bool EquatableTo(EditorFrame* Other) const override { return false; }
				String GetName() const override { return L"Add Operation Inventory Item"; }
				WNDPROC ThisProc() const override { return WndProc; }

				void Reset() override;
			};

			class InventorySearchEditor : public EditorFrame
			{
			private:
				Files::InventorySystem* _System = nullptr;

				Button* Run = nullptr, *SaveResult = nullptr;
				CheckableButton* OperationMode = nullptr, *StandardMode = nullptr;

				ScrollViewer* ObjectScroll = nullptr;
				Grid* ObjectView = nullptr;
				Button* View = nullptr, * Edit = nullptr, * DuplicateResult = nullptr;
				Vector<UI::ComponentViewer*> Objects;

				TextBox* SerialNumber = nullptr, * Group = nullptr;
				Button* SelectGroups = nullptr;
				CheckableButton* WorkingOrder = nullptr, * PartialWorkingOrder = nullptr, * Broken = nullptr, * UnderRepair = nullptr, * NotInPossession = nullptr;

				bool _Mode = true; //True for Inventory Search, false for OperationInventorySearch
				void RunSearch();
				void SwitchModes();

				static LRESULT __stdcall WndProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp);
			protected:
				void LoadControls() override;

				LRESULT Command(WPARAM wp, LPARAM lp) override;
				LRESULT KeyDown(WPARAM wp) override;
				LRESULT Size() override;
				
			public:
				InventorySearchEditor(Files::InventorySystem* System, bool Mode = false);

				EditorTypes EditorType() const override { return EDT_InventorySearch; }
				bool IsApplyable() const override { return false; }
				bool ConditionSpecific() const override { return true; }
				bool Apply(Files::ArminSessionBase* File, bool PropmtErrors = true) override { return true; }
				Vector<void*> CondenseArgs() const override { return { (void*)_Mode }; }
				bool TestOnCondition(Vector<void*> Args) const override;
				bool EquatableTo(EditorFrame* Other) const override { return dynamic_cast<InventorySearchEditor*>(Other) != nullptr; }
				String GetName() const override { return L"Inventory Search"; }
				WNDPROC ThisProc() const override { return WndProc; }

				void Reset() override;
			};
		}

		namespace RefGroups
		{
			class AddReferenceGroupEditor : public EditorFrame
			{
			private:
				TextBox* Name;
				ScrollViewer* ObjectScroll;
				Grid* ObjectView;
				Vector<UI::ComponentViewer*> Objects;
				Button* Modify, * Remove, * View, * Edit;

				bool _Multiselect;

				static LRESULT __stdcall WndProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp);

			protected:
				void LoadControls() override;

				LRESULT Size() override;
				LRESULT KeyDown(WPARAM wp) override;
				LRESULT Command(WPARAM wp, LPARAM lp) override;
			public:
				AddReferenceGroupEditor();

				EditorTypes EditorType() const override { return EDT_AddReferenceGroup; }
				bool IsApplyable() const override { return true; }
				bool ConditionSpecific() const override { return false; }
				bool Apply(Files::ArminSessionBase* DestFile, bool PromptErrors = true) override;
				Vector<void*> CondenseArgs() const override { return Vector<void*>(); }
				bool TestOnCondition(Vector<void*> Args) const override { return false; }
				bool EquatableTo(EditorFrame* Other) const override { return false; }
				String GetName() const override { return L"Add Refrence Group"; }
				WNDPROC ThisProc() const override { return WndProc; }

				void Reset() override;
			};
			class ViewEditReferenceGroupEditor : public EditorFrame
			{
			private:
				Files::RefrenceGroup* _Target;
				bool _EditMode = false;

				TextBox* TitleEd;
				Label* TitleVi;
				Button* Modify, * Remove, * View, * Edit;
				ScrollViewer* ObjectScroll;
				Grid* ObjectView;
				Vector<UI::ComponentViewer*> Objects;

				bool _Multiselect = true;

				static LRESULT __stdcall WndProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp);
			protected:
				void LoadControls() override;

				LRESULT Size() override;
				LRESULT KeyDown(WPARAM wp) override;
				LRESULT Command(WPARAM wp, LPARAM lp);
			public:
				ViewEditReferenceGroupEditor(Files::RefrenceGroup* Target, bool EditMode);

				EditorTypes EditorType() const override { return EDT_ViewEditReferenceGroup; }
				bool IsApplyable() const override { return _EditMode; }
				bool ConditionSpecific() const override { return true; }
				bool Apply(Files::ArminSessionBase* DestFile, bool PromptErrors = true) override;
				Vector<void*> CondenseArgs() const override { return { _Target, (void*)_EditMode }; }
				bool TestOnCondition(Vector<void*> Args) const override { return Args.Size == 2 && Args[0] == static_cast<void*>(_Target) && Args[1] == (void*)_EditMode; }
				bool EquatableTo(EditorFrame* Other) const override;
				String GetName() const override { return _EditMode ? L"Edit Refrence Group" : L"View Refrence Group"; }
				WNDPROC ThisProc() const override { return WndProc; }

				void Reset() override;
			};
			class ReferenceGroupsEditor : public EditorFrame
			{
			private:
				Files::ArminSessionBase* _Source;

				Label* ObjectCount;
				Button* Add, * Remove, * Edit, * View, *SelectAll, *DeSelectAll, *Search;
				ScrollViewer* ObjectScroll;
				Grid* ObjectView;
				Vector<UI::ComponentViewer*> Objects;

				void FillObjects();

				static LRESULT __stdcall WndProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp);

			protected:
				void LoadControls() override;

				LRESULT Size() override;
				LRESULT KeyDown(WPARAM wp) override;
				LRESULT Command(WPARAM wp, LPARAM lp) override;
			public:
				ReferenceGroupsEditor(Files::ArminSessionBase* Source);

				EditorTypes EditorType() const override { return EDT_ReferenceGroups; }
				bool IsApplyable() const override { return false; }
				bool ConditionSpecific() const override { return false; }
				bool Apply(Files::ArminSessionBase* DestFile, bool PromptErrors = true) override { return true; }
				Vector<void*> CondenseArgs() const override { return Vector<void*>(); }
				bool TestOnCondition(Vector<void*> Args) const override { return true; }
				bool EquatableTo(EditorFrame* Other) const override { return Other && dynamic_cast<ReferenceGroupsEditor*>(Other) == this; }
				String GetName() const override { return L"Refrence Groups"; }
				WNDPROC ThisProc() const override { return WndProc; }

				void Reset() override;
			};
		}

		namespace Resources
		{
			class ViewImageEditor : public EditorFrame
			{
			private:
				HGLOBAL _Target;
				ImageViewer* Img;

				static LRESULT __stdcall WndProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp);
			protected:
				void LoadControls() override;

				LRESULT Command(WPARAM wp, LPARAM lp) override;
				LRESULT KeyDown(WPARAM Key) override;
				LRESULT Size() override;
			public:
				ViewImageEditor(Files::Image* Target);
				~ViewImageEditor();

				EditorTypes EditorType() const override { return EDT_ViewImage; }
				bool IsApplyable() const override { return false; }
				bool ConditionSpecific() const override { return true; }
				bool Apply(Files::ArminSessionBase* File, bool PromptErrors = true) override { return true; }
				Vector<void*> CondenseArgs() const override { return _Target; }
				bool TestOnCondition(Vector<void*> Args) const override;
				bool EquatableTo(EditorFrame* Other) const override;
				String GetName() const override { return L"View Image"; }
				WNDPROC ThisProc() const override { return WndProc; }

				void Reset() override;
			};

			class ImagesEditor : public EditorFrame
			{
			private:
				Files::ResourceSystem* _System;

				Button *Add, * Remove, * View, *Edit, *SelectAll, *DeSelectAll, *Compress;
				ScrollViewer* ObjectScroll;
				Grid* ObjectView;
				Vector<UI::ComponentViewer*> Objects;

				static LRESULT __stdcall WndProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp);

				void FillObjects();
			protected:
				void LoadControls() override;

				LRESULT Size() override;
				LRESULT KeyDown(WPARAM wp) override;
				LRESULT Command(WPARAM wp, LPARAM lp) override;
			public:
				ImagesEditor(Files::ResourceSystem* TargetSys = nullptr);
				
				EditorTypes EditorType() const override { return EDT_Images; }
				bool IsApplyable() const override { return false; }
				bool ConditionSpecific() const override { return true; }
				bool Apply(Files::ArminSessionBase* File, bool PromptErrors = true) override { return true; }
				Vector<void*> CondenseArgs() const override { return _System; }
				bool TestOnCondition(Vector<void*> Other) const override;
				bool EquatableTo(EditorFrame* Other) const override;
				String GetName() const override { return L"Images"; }
				WNDPROC ThisProc() const override { return WndProc; }

				void Reset() override;
			};
		}

		namespace Misc
		{
			class BasicEditorEditor : public EditorFrame
			{
			private:
				UI::ComponentViewer* Target, * CV1, * CV2;
				Label* LB1, * LB2;
				TextBox* TB1, * TB2, * TB3, * TB4, * TB5; //Text Boxes
				CheckableButton* CB1, * CB2, * CB3; //Checkable Buttons
				ComboBox* DD1, * DD2; //Drop downs
				Button* B1, * B2, * B3; //Button
				ScrollViewer* SV1; //Scroll Viewer
				Grid* G1; //Grid for Scroll Viewer
				Label* L1, * L2; //Labels
				Vector<ControlBase*> GridList; //For Scroll Viewer
				Vector<Files::Component*> ComponentList;

				/*
				* BasicEditor & BasicViewer
				*
				* Since the data for each component is diffrent, the basic viewer & editor have 'slot' controls, that can be used to display/edit data about an object.
				*
				* Not every slot must be used, and only specific slots will be used/loaded according to which component is targeted.
				*/

				Files::Component* ThisTarget; //The source and modify location of the current editor.
				bool _DummySelect;

				static LRESULT __stdcall WndProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp);

				void ProcessCommand(WPARAM wp);

			protected:
				void LoadControls() override;

				LRESULT Command(WPARAM wp, LPARAM lp) override;
				LRESULT KeyDown(WPARAM Key) override;
				LRESULT Size() override;
				LRESULT Destroy() override;

			public:
				BasicEditorEditor(Files::Component* Target);

				EditorTypes EditorType() const override { return EDT_BasicEditor; }
				bool IsApplyable() const override { return true; }
				bool ConditionSpecific() const override { return true; }
				bool Apply(Files::ArminSessionBase* File, bool PromptErrors = true) override;
				Vector<void*> CondenseArgs() const override { return { ThisTarget }; }
				bool TestOnCondition(Vector<void*> Args) const override;
				bool EquatableTo(EditorFrame* Other) const override { return false; }
				String GetName() const override;
				WNDPROC ThisProc() const override { return WndProc; }

				void Reset() override;
			};
			class BasicViewerEditor : public EditorFrame
			{
			private:
				UI::ComponentViewer* Target, * CV1;
				Vector<Label*> Data;
				Vector<Label*> ScrollData;
				Grid* ScrollGrid;
				ScrollViewer* Scroll;

				Files::Component* ThisTarget;
				bool _DummySelect = false;

				static LRESULT __stdcall WndProc(HWND, UINT, WPARAM, LPARAM);

			protected:
				void LoadControls() override;

				LRESULT Command(WPARAM wp, LPARAM lp) override;
				LRESULT KeyDown(WPARAM Key) override;
				LRESULT Size() override;
				LRESULT Destroy() override;

			public:
				BasicViewerEditor(Files::Component* Target);

				EditorTypes EditorType() const override { return EDT_BasicViewer; }
				bool IsApplyable() const override { return false; }
				bool ConditionSpecific() const override { return true; }
				bool Apply(Files::ArminSessionBase* File, bool PromptErrors = true) override { return true; }
				Vector<void*> CondenseArgs() const override { return { ThisTarget }; }
				bool TestOnCondition(Vector<void*> Args) const override;
				bool EquatableTo(EditorFrame* Other) const override;
				String GetName() const override;
				WNDPROC ThisProc() const override { return WndProc; }

				void Reset() override;
			};
			class QuickSearchEditor : public EditorFrame
			{
			private:
				int Filter = Files::ComponentTypes::CT_All;

				TextBox* SearchCriteria;
				Button* Remove, * Add, * View, * Edit, * SaveResults, * DuplicateResults, * _RunSearch, * SelectAll, * ClearSelection; //Remove and Add do not actually remove the components from the file. They only add or remove to the search result. DuplicateResults opens the current components in ComponentListRenderer. SaveResulsts creates a new RefrenceGroup object that is stored in the project.
				CheckableButton* Users, * Tasks, * CompletedTasks, * InventoryItems, * JobPositions, * OperationInventoryItems, * RefrenceGroups, *Images;
				ScrollViewer* ObjectScroll, * TypesScroll;
				Grid* ObjectView, * TypesView;
				Vector<UI::ComponentViewer*> Objects;

				bool _Multiselect = true;

				static LRESULT __stdcall WndProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp);

			protected:
				void LoadControls() override;

				LRESULT Command(WPARAM wp, LPARAM lp) override;
				LRESULT KeyDown(WPARAM Key) override;
				LRESULT Size() override;

			public:
				QuickSearchEditor(int Filter = Files::ComponentTypes::CT_All);

				EditorTypes EditorType() const override { return EDT_QuickSearch; }
				bool IsApplyable() const override { return false; }
				bool ConditionSpecific() const override { return false; }
				bool Apply(Files::ArminSessionBase* File, bool PromptErrors = true) override { return true; }
				Vector<void*> CondenseArgs() const override { return Vector<void*>(); }
				bool TestOnCondition(Vector<void*> Args) const override { return true; }
				bool EquatableTo(EditorFrame* Other) const override { return Other && dynamic_cast<QuickSearchEditor*>(Other) != nullptr; }
				String GetName() const override { return L"Quick Search"; }
				WNDPROC ThisProc() const override { return WndProc; }

				void RunSearch();
				void Reset() override;
			};
			
			class ProjectSettingsEditor : public EditorFrame
			{
			private:
				Files::ArminSessionBase* Target;

				static LRESULT __stdcall WndProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp);
				
			protected:
				void LoadControls() override;

				LRESULT Size() override;
				LRESULT KeyDown(WPARAM wp) override;
				LRESULT Command(WPARAM wp, LPARAM lp) override;

			public:
				ProjectSettingsEditor(Files::ArminSessionBase* File = nullptr);

				EditorTypes EditorType() const override { return EDT_ProjectSettings; }
				bool IsApplyable() const override { return true; }
				bool ConditionSpecific() const override { return true; }
				bool Apply(Files::ArminSessionBase* File, bool PromptErrors = true) override;
				Vector<void*> CondenseArgs() const override { return Target; }
				bool TestOnCondition(Vector<void*> Args) const override { return Args.Size != 0 && Args[0] == (void*)Target; }
				bool EquatableTo(EditorFrame* Other) const override;
				String GetName() const override { return L"Project Settings"; }
				WNDPROC ThisProc() const override { return WndProc; }

				void Reset() override;
			};
			class SettingsEditor : public EditorFrame
			{
			private:
				CheckableButton* LoadLast;
				TextBox* LastLoaded;
				Button* LastLoadedSelect;

				ScrollViewer* RecentScroll;
				Grid* RecentView;
				Vector<CheckableButton*> Recents;
				Button* Remove;

				static LRESULT __stdcall WndProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp);

			protected:
				void LoadControls() override;

				LRESULT Size() override;
				LRESULT KeyDown(WPARAM wp) override;
				LRESULT Command(WPARAM wp, LPARAM lp) override;

			public:
				SettingsEditor();

				EditorTypes EditorType() const override { return EDT_Settings; }
				bool IsApplyable() const override { return false; }
				bool ConditionSpecific() const override { return false; }
				bool Apply(Files::ArminSessionBase* File, bool PromptErrors = true) override { return true; }
				Vector<void*> CondenseArgs() const override { return Vector<void*>(); }
				bool TestOnCondition(Vector<void*> Args) const override { return true; }
				bool EquatableTo(EditorFrame* Other) const override { return true; }
				String GetName() const override { return L"Settings"; }
				WNDPROC ThisProc() const override { return WndProc; }

				void Reset() override;
			};
		}
	}
}