#include "EditorButtonHost.h"

#include "Editor.h"
#include "EditorRegistry.h"
#include "EditorButton.h"
#include "..\UI\UICommon.h"

namespace Core::Editors
{	
	using namespace UI;
	using namespace UI::Controls;

	EditorButtonHost::~EditorButtonHost()
	{
		CurrentEdtBttnX = 10;
	}

	void EditorButtonHost::ProcessOpenEditor(Editor* Editor)
	{
		class Editor* OldCurrent = Current;		
		EditorHost::ProcessOpenEditor(Editor);

		LastEditor = OldCurrent;
	}

	void EditorButtonHost::AddEditorButton(Editor* Editor)
	{
		StyleSheet Style;
		Style.Background = Accent1;
		Style.BorderBrush = 0xFFFFFFFF;
		Style.BorderThickness = 3;
		Style.Radius = 20;

		TextStyleSheet TextStyle;
		TextStyle.Foreground = 0xFFFFFFFF;
		TextStyle.FontFamily = StandardFontName;
		TextStyle.FontSize = 12;

		RECT WndRect;
		GetClientRect(this->EditorButtonsBase, &WndRect);

		int XCoord = CurrentEdtBttnX;
		int YCoord = 3;
		int Height = WndRect.bottom - YCoord - 8;
		int Width = 150;

		this->EditorButtonsBase->Move(0, 0, CurrentEdtBttnX + Width + 7, WndRect.bottom);
		EditorButtonsScroll->Reset();

		EditorButtons.Add(new EditorButton(XCoord, YCoord, Width, Height, EditorButtonsBase, reinterpret_cast<HINSTANCE>(GetWindowLongPtrW(*EditorButtonsBase, GWLP_HINSTANCE)), Editor, this, Accent1));
		CurrentEdtBttnX += Width + 7;
	}
	void EditorButtonHost::RemoveEditorButton(Editor* Editor)
	{
		for (uint i = 0; i < EditorButtons.Size; i++)
		{
			EditorButton* Current = EditorButtons[i];
			if (Current->Current() == Editor)
			{
				EditorButtons.Remove(Current);
				delete Current;
				break;
			}
		}

		ResizeEditorButtons();
	}
	void EditorButtonHost::ResizeEditorButtons()
	{
		RECT WndRect;
		GetClientRect(EditorButtonsBase, &WndRect);

		int& XCoord = (CurrentEdtBttnX = 5);
		int YCoord = 3;
		int Width = 150;
		int Height = WndRect.bottom - YCoord - 4;

		EditorButtonsBase->Move(0, 0, (Width + 7) * (EditorButtons.Size + 1), WndRect.bottom);
		EditorButtonsScroll->Reset();

		for (uint i = 0; i < EditorButtons.Size; i++)
		{
			EditorButtons[i]->Move(XCoord, YCoord, Width, Height);
			XCoord += 7 + Width;
		}
	}
	void EditorButtonHost::LoadEditorButtonsBase(HWND Parent, HINSTANCE ins, int XCoord, int YCoord, int Width, int Height, AaColor BaseBk)
	{
		StyleSheet Style;
		Style.Background = BaseBk;
		Style.BorderBrush = 0x00FFFFFF;
		Style.BorderThickness = 0;
		Style.Radius = 20;

		EditorButtonsScroll = new ScrollViewer(XCoord, YCoord, Width, Height, Parent, ins, Style);
		EditorButtonsBase = new Grid(0, 0, Width, 40, EditorButtonsScroll, ins, Style);
		EditorButtonsScroll->SetViewer(EditorButtonsBase);
	}
	void EditorButtonHost::MoveEditorButtonsBase(int XCoord, int YCoord, int Width, int Height)
	{
		MoveWindow(*EditorButtonsScroll, XCoord, YCoord, Width, Height, true);
		ResizeEditorButtons();
	}

	void EditorButtonHost::OpenLastEditor()
	{
		Vector<Editor*> AllEditors = EditorRegistry::GetEditors(AllEditorsCode, this);
		for (Editor*& Obj : AllEditors)
			ShowWindow(*Obj, SW_HIDE);

		if (LastEditor && LastEditor->Host == this)
		{
			ShowWindow(*LastEditor, SW_SHOW);
			Current = LastEditor;
			LastEditor = nullptr;
		}
		else if (EditorButtons.Size != 0)
		{
			Vector<EditorButton*> Temp = EditorButtons;
			Temp.Reverse();

			uint Index = 0;
			Editor* Target = nullptr;

			for (uint i = 0; i < Temp.Size; i++)
			{
				Editor* ThisRun = Temp[i]->Current();
				if (ThisRun->Host == this)
				{
					Target = ThisRun;
					break;
				}
			}

			Current = Target;
			if (Current)
				ShowWindow(*Current, SW_SHOW);
			Temp.Clear();
		}
	}
}