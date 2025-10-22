#include "..\EditorFrame.h"

#include "Sort.h"
#include "UI\StyleButton.h"
#include "..\EditorRegistry.h"
#include "..\..\UserRegistry.h"
#include "..\..\Files\ArminSessions.h"
#include "..\..\Files\SearchCriteria.h"

namespace Armin::Editors::Users
{
	using namespace Files;
	using namespace UI;

	UsersEditor::UsersEditor(UserSystem* System)
	{
		if (!System)
			_System = dynamic_cast<UserSystem*>(LoadedProject);
		else
			_System = System;
	}
	UsersEditor::~UsersEditor()
	{
		if (Objects)
			delete Objects;
	}

	LRESULT __stdcall UsersEditor::WndProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp)
	{
		UsersEditor* This = reinterpret_cast<UsersEditor*>(GetWindowLongPtrW(Window, GWLP_USERDATA));
		if (!This)
			return DefWindowProcW(Window, Message, wp, lp);

		return EditorFrame::EditorProc(This, Window, Message, wp, lp);
	}

	void UsersEditor::LoadControls()
	{
		int BaseXCoord = 10;
		int BaseYCoord = this->BaseYCoord;
		AaColor BaseBk = EditorGrey;

		RECT WndRect;
		GetClientRect(_Base, &WndRect);
		HINSTANCE ins = reinterpret_cast<HINSTANCE>(GetWindowLongPtrW(_Base, GWLP_HINSTANCE));

		_Loaded = true;

		LoadUpperButtons(WndRect, ins);

		{
			int XCoord = 10;
			int YCoord = BaseYCoord;
			int Width = 130;
			int Height = 27;

			TextStyleSheet TextStyle;
			TextStyle.Foreground = FontColor;
			TextStyle.FontFamily = StandardFontName;
			TextStyle.FontSize = 13;
			TextStyle.Alignment = TA_RightAlignment;
			TextStyle.Bold = true;

			StyleSheet Style;
			Style.Background = Grey3;
			Style.BaseBackground = BaseBk;
			Style.BorderBrush = Accent4;
			Style.BorderThickness = 3;
			Style.Radius = 20;

			int ResetY = 0;

			{
				MiscControls.Add(new Label(XCoord, YCoord, Width, Height, _Base, ins, L"User Size:", BaseBk, TextStyle, false));

				TextStyle.Bold = false;
				TextStyle.Alignment = TA_LeftAlignment;
				UserCount = new Label(XCoord + 5 + Width, YCoord, (WndRect.right) - (10 + (XCoord + 5 + Width)), Height, _Base, ins, L"0", BaseBk, TextStyle, false);
				YCoord += 10 + Height;

				TextStyle.Bold = true;
				TextStyle.Alignment = TA_RightAlignment;
				MiscControls.Add(new Label(XCoord, YCoord, Width, Height, _Base, ins, L"Current User:", BaseBk, TextStyle, false));
				
				TextStyle.Bold = false;
				TextStyle.Alignment = TA_LeftAlignment;
				CurrentUser = new ComponentViewer(XCoord + 5 + Width, YCoord, (WndRect.right) - (10 + (XCoord + 5 + Width)), Height, _Base, ins, nullptr, Armin::CurrentUser, false, nullptr);

				YCoord += 10 + Height;
				Width = (WndRect.right - (10 + XCoord + 10)) / 2;				

				Search = new StyleButton(XCoord, YCoord, Width, Height, L"User Search", _Base, (HMENU)4, ins, Style, TextStyle, RECT{ 0, 0, 0, 5 });
				XCoord += Width + 10;

				SignOut = new StyleButton(XCoord, YCoord, Width, Height, L"Sign Out", _Base, (HMENU)5, ins, Style, TextStyle, RECT{ 0, 0, 0, 5 });
				XCoord -= Width + 10;
				ResetY = YCoord += 10 + Height;
			}

			int ButtonSize = 40;

			{
				XCoord = WndRect.right - (ButtonSize + 10);

				int OldWidth = Width, OldHeight = Height;
				Width = Height = ButtonSize;

				Add = new Button(XCoord, YCoord, Width, Height, L"+", _Base, (HMENU)6, ins, Style, TextStyle);
				EnableWindow(*Add, (AppState & APS_HasAdminUser));
				YCoord += 5 + ButtonSize;

				Remove = new Button(XCoord, YCoord, Width, Height, L"-", _Base, (HMENU)7, ins, Style, TextStyle);
				EnableWindow(*Remove, (AppState & APS_HasAdminUser));
				YCoord += 10 + ButtonSize;

				View = new Button(XCoord, YCoord, Width, Height, L"VI", _Base, (HMENU)8, ins, Style, TextStyle);
				YCoord += 5 + ButtonSize;

				Edit = new Button(XCoord, YCoord, Width, Height, L"ED", _Base, (HMENU)9, ins, Style, TextStyle);
				EnableWindow(*Edit, (AppState & APS_HasAdminUser));
				YCoord += 10 + ButtonSize;

				SelectAll = new Button(XCoord, YCoord, Width, Height, L"SA", _Base, (HMENU)10, ins, Style, TextStyle);
				YCoord += 5 + ButtonSize;

				DeSelectAll = new Button(XCoord, YCoord, Width, Height, L"DA", _Base, (HMENU)11, ins, Style, TextStyle);

				Width = OldWidth;
				Height = OldHeight;
			}

			YCoord = ResetY;
			XCoord = 10;
			Width = WndRect.right - (10 + XCoord + ButtonSize + 10);
			Height = WndRect.bottom - (10 + YCoord);

			ObjectScroll = new ScrollViewer(XCoord, YCoord, Width, Height, _Base, ins, Style);
			ObjectView = new Grid(0, 0, 910, 32, ObjectScroll, ins, Style);
			ObjectScroll->SetViewer(ObjectView);

			if (Objects)
				delete Objects;
			Objects = new ComponentViewerList(ObjectView, ObjectScroll);

			FillObjects();
		}
	}
	void UsersEditor::FillObjects()
	{
		UserSet* Users = !_System ? nullptr : _System->Users;
		if (!Users)
			return;

		Vector<User*> Items = *Users;
		QuickSort(Items, [](Files::User*& One, Files::User*& Two) { return static_cast<wstring>(One->Username) < static_cast<wstring>(Two->Username); });

		UserCount->SetText(Items.Size);
		Objects->GenerateList(Items, NULL, true, true);
	}

	LRESULT UsersEditor::Command(WPARAM wp, LPARAM lp)
	{
		switch (wp)
		{
		case 4: //Search
			EditorRegistry::OpenEditor(new UserSearchEditor(_System), nullptr);
			break;
		case 5: //SignOut
			UserSignOut();
			break;
		case 6: //Add
			if (!Armin::CurrentUser || !(AppState & APS_HasAdminUser))
			{
				MessageBoxW(_Base, L"You do not have the proper User Level Access to add a user.", L"Add User:", MB_OK | MB_ICONERROR);
				break;
			}

			EditorRegistry::OpenEditor(new CreateEditUserEditor(nullptr), nullptr);
			break;
		case 7://Remove
		{
			if (!Armin::CurrentUser || !(AppState & APS_HasAdminUser))
			{
				MessageBoxW(_Base, L"You do not have the proper User Level Access to remove a user.", L"Remove User:", MB_OK | MB_ICONERROR);
				break;
			}

			Vector<ComponentViewer*> Selected;
			Vector<Component*> Targets = Objects->RetriveFromList(Selected);

			if (Targets.Size == 0)
			{
				MessageBoxW(GetAncestor(_Base, GA_ROOT), L"Please select at least one user to remove.", L"Remove User:", MB_OK | MB_ICONERROR);
				break;
			}

			int Result = MessageBoxW(GetAncestor(_Base, GA_ROOT), L"The following users will be remove permanantly.\n\nDo you want to continue?", L"Remove Users:", MB_YESNOCANCEL | MB_ICONWARNING);
			if (Result != IDYES)
				break;

			for (uint i = 0; i < Targets.Size; i++)
			{
				Component* Temp = Targets[i];
				User* Conv = dynamic_cast<User*>(Temp);
				if (!Conv)
					continue;

				delete Conv;

				ComponentViewer* Current = Selected[i];
				delete Current;
			}

			Objects->ReSizeList();
			AppState |= APS_HasEdit;
			break;
		}
		case 8: //View
		case 9: //Edit
			Objects->OpenSelectedForEditView(wp == 9);
			break;
		case 10: //SelectAll
		case 11: //DeSelectAll
			for (ComponentViewer* Obj = Objects->Item(0); Obj != nullptr; Obj = Obj->Next)
				Obj->CheckState(wp == 10);
			break;
		}

		return 0;
	}
	LRESULT UsersEditor::KeyDown(WPARAM key)
	{
		switch (key)
		{
		case VK_RETURN:
			FillObjects();
			return 0;
		case VK_DELETE:
			if ((AppState & APS_HasAdminUser))
				Command(7, 0);
			break;
		case 'N':
			if ((GetKeyState(VK_SHIFT) & 0x8000) && (GetKeyState(VK_CONTROL) & 0x8000) && (AppState & APS_HasAdminUser))
				Command(6, 0);
			break;
		case 'V':
		case 'E':
			if (GetKeyState(VK_CONTROL) & 0x8000)
			Objects->OpenSelectedForEditView(key == 'E' && (AppState & APS_HasAdminUser));
			break;
		default:
			return SendMessageW(GetParent(_Base), WM_KEYDOWN, key, 0);
		}

		return 0;
	}
	LRESULT UsersEditor::Size()
	{
		int BaseXCoord = 10;
		int BaseYCoord = this->BaseYCoord;

		RECT WndRect;
		GetClientRect(_Base, &WndRect);

		if (!_Loaded)
			return 0;

		MoveUpperButtons(WndRect);

		{
			int XCoord = 10;
			int YCoord = BaseYCoord;
			int Width = 130;
			int Height = 27;

			int ResetY = 0;

			{
				MiscControls[0]->Move(XCoord, YCoord, Width, Height);

				UserCount->Move(XCoord + 5 + Width, YCoord, (WndRect.right) - (10 + (XCoord + 5 + Width)), Height);
				YCoord += 10 + Height;

				MiscControls[1]->Move(XCoord, YCoord, Width, Height);

				CurrentUser->Move(XCoord + 5 + Width, YCoord, (WndRect.right) - (10 + (XCoord + 5 + Width)), Height);

				YCoord += 10 + Height;
				Width = (WndRect.right - (10 + XCoord + 10)) / 2;

				Search->Move(XCoord, YCoord, Width, Height);
				XCoord += Width + 10;

				SignOut->Move(XCoord, YCoord, Width, Height);
				XCoord -= Width + 10;
				ResetY = YCoord += 10 + Height;
			}

			int ButtonSize = 40;

			{
				XCoord = WndRect.right - (ButtonSize + 10);

				int OldWidth = Width, OldHeight = Height;
				Width = Height = ButtonSize;

				Add->Move(XCoord, YCoord, Width, Height);
				YCoord += 5 + ButtonSize;

				Remove->Move(XCoord, YCoord, Width, Height);
				YCoord += 10 + ButtonSize;

				View->Move(XCoord, YCoord, Width, Height);
				YCoord += 5 + ButtonSize;

				Edit->Move(XCoord, YCoord, Width, Height);
				YCoord += 10 + ButtonSize;

				SelectAll->Move(XCoord, YCoord, Width, Height);
				YCoord += 5 + ButtonSize;

				DeSelectAll->Move(XCoord, YCoord, Width, Height);

				Width = OldWidth;
				Height = OldHeight;
			}

			YCoord = ResetY;
			XCoord = 10;
			Width = WndRect.right - (10 + XCoord + ButtonSize + 10);
			Height = WndRect.bottom - (10 + YCoord);

			ObjectScroll->Move(XCoord, YCoord, Width, Height);
			Objects->ReSizeList();
		}

		return 0;
	}

	void UsersEditor::Reset()
	{
		FillObjects();
	}
}