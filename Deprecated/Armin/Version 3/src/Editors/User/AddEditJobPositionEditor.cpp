#include "..\EditorFrame.h"

#include "..\EditorRegistry.h"
#include "..\..\Files\ArminSessions.h"

namespace Armin::Editors::Users
{
	using namespace Files;

	AddEditJobPositionEditor::AddEditJobPositionEditor(Files::JobPosition* Target)
	{
		if (Target)
			this->Target = Target;
		else
			this->Target = nullptr;
	}

	LRESULT __stdcall AddEditJobPositionEditor::WndProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp)
	{
		AddEditJobPositionEditor* This = reinterpret_cast<AddEditJobPositionEditor*>(GetWindowLongPtrW(Window, GWLP_USERDATA));
		if (!This)
			return DefWindowProcW(Window, Message, wp, lp);

		return EditorProc(This, Window, Message, wp, lp);
	}

	void AddEditJobPositionEditor::LoadControls()
	{
		if (_Loaded)
			return;

		_Loaded = true;

		RECT WndRect;
		GetClientRect(_Base, &WndRect);
		HINSTANCE ins = reinterpret_cast<HINSTANCE>(GetWindowLongPtrW(_Base, GWLP_HINSTANCE));

		int BaseYCoord = this->BaseYCoord;
		AaColor BaseBk = EditorGrey;

		LoadUpperButtons(WndRect, ins);

		{
			int XCoord = 10;
			int YCoord = BaseYCoord;
			int Height = 30;
			int Width = 140;

			TextStyleSheet TextStyle;
			TextStyle.FontSize = 13;
			TextStyle.FontFamily = StandardFontName;
			TextStyle.Foreground = FontColor;
			TextStyle.Alignment = TA_RightAlignment;
			
			StyleSheet Style;
			Style.BaseBackground = BaseBk;
			Style.Background = Grey3;
			Style.BorderBrush = Accent4;
			Style.BorderThickness = 3;
			Style.Radius = 20;

			MiscControls.Add(new Label(XCoord, YCoord, Width, Height, _Base, ins, L"Title:", BaseBk, TextStyle, false));
			YCoord += 10 + Height;

			MiscControls.Add(new Label(XCoord, YCoord, Width, Height, _Base, ins, L"Description:", BaseBk, TextStyle, false));

			YCoord = BaseYCoord;
			XCoord += 5 + Width;
			Width = WndRect.right - (10 + XCoord);
			TextStyle.Alignment = TA_LeftAlignment;

			Title = new TextBox(XCoord, YCoord, Width, Height, _Base, ins, Target ? Target->Title() : String(), Style, TextStyle);

			YCoord += 10 + Height;
			Height = WndRect.bottom - 10 - YCoord;

			Description = new TextBox(XCoord, YCoord, Width, Height, _Base, ins, Target ? Target->Description : String(), Style, TextStyle, true);
		}
	}

	LRESULT AddEditJobPositionEditor::Command(WPARAM wp, LPARAM lp)
	{
		return 0;
	}
	LRESULT AddEditJobPositionEditor::KeyDown(WPARAM wp)
	{
		return 0;
	}
	LRESULT AddEditJobPositionEditor::Size()
	{
		if (!_Loaded)
			return 0;

		RECT WndRect;
		GetClientRect(_Base, &WndRect);

		int BaseYCoord = this->BaseYCoord;

		MoveUpperButtons(WndRect);

		{
			int XCoord = 10;
			int YCoord = BaseYCoord;
			int Height = 30;
			int Width = 140;

			MiscControls[0]->Move(XCoord, YCoord, Width, Height);
			YCoord += 10 + Height;

			MiscControls[1]->Move(XCoord, YCoord, Width, Height);

			YCoord = BaseYCoord;
			XCoord += 5 + Width;
			Width = WndRect.right - (10 + XCoord);

			Title->Move(XCoord, YCoord, Width, Height);

			YCoord += 10 + Height;
			Height = WndRect.bottom - 10 - YCoord;

			Description->Move(XCoord, YCoord, Width, Height);
		}

		return 0;
	}
	LRESULT AddEditJobPositionEditor::Destroy()
	{
		return 0;
	}

	void AddEditJobPositionEditor::Reset()
	{
		Title->SetText(String());
		Description->SetText(String());
	}
	bool AddEditJobPositionEditor::Apply(ProjectBase* File, bool PromptErrors)
	{
		UserSystem* ConvFile = dynamic_cast<UserSystem*>(File);
		JobPositionList* Positions = ConvFile ? ConvFile->Positions : nullptr;

		if (!ConvFile || !Positions)
		{
			if (PromptErrors)
				MessageBoxW(GetAncestor(_Base, GA_ROOT), L"The currently loaded project does not support Job Positions.\n\nPlease load a project that does, and try again.", L"Add Job Position:", MB_OK | MB_ICONERROR);
			return false;
		}

		String Title = this->Title->GetText();
		String Description = this->Description->GetText();

		if (Title == L"" || Title.Contains(L'~') || Description.Contains(L'~'))
		{
			if (PromptErrors)
				MessageBoxW(GetAncestor(_Base, GA_ROOT), L"Please ensure that the title has a value, and that the title and description do not contain any tilde ('~'). ", L"Add Job Position", MB_OK | MB_ICONERROR);
			return false;
		}

		JobPosition* New = Target ? Target : new JobPosition(File, Positions);
		New->Title(Title);
		New->Description = Description;

		AppState |= APS_HasEdit;
		EditorRegistry::ResetEditorOfType(EDT_JobPositions);
		return true;
	}
	bool AddEditJobPositionEditor::TestOnCondition(Vector<void*> Args) const
	{
		if (Target)
			return Args.Size != 0 && Args[0] == Target;
		return false;
	}
	bool AddEditJobPositionEditor::EquatableTo(EditorFrame* Obj) const
	{
		if (Target)
			return dynamic_cast<AddEditJobPositionEditor*>(Obj) != nullptr && Obj->TestOnCondition(CondenseArgs());
		return false;
	}
}