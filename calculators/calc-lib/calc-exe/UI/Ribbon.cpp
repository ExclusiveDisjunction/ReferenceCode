#include "Main.h"

#include "Editors\Window\SideHost.h"
#include "Editors\Editor.h"
#include "Editors\EditorRegistry.h"
//#include "..\IO\ProjectIO.h"

namespace Mason::UI
{
	using namespace Core;
	using namespace Controls;
	using namespace IO;
	using namespace Editors;

	void Main::SetRibbonStatus()
	{
		FileInstance* Current = LoadedProject;
		
		HideRibbonGrids();
		ShowWindow(*FileGrid, SW_SHOW);
	}

	void Main::HideRibbonGrids()
	{
		if (FileGrid)
			ShowWindow(*FileGrid, SW_HIDE);
		if (EditGrid)
			ShowWindow(*EditGrid, SW_HIDE);
		if (WindowGrid)
			ShowWindow(*WindowGrid, SW_HIDE);
		if (HelpGrid)
			ShowWindow(*HelpGrid, SW_HIDE);
	}

	void Main::LoadRibbon()
	{
		RECT WndRect;
		GetClientRect(RibbonBase, &WndRect);
		HINSTANCE ins = reinterpret_cast<HINSTANCE>(GetWindowLongPtrW(_Base, GWLP_HINSTANCE));

		{
			int XCoord = 5;
			int YCoord = 3;
			int Width = 140;
			int Height = 33;

			TextStyleSheet TextStyle;
			TextStyle.FontSize = 16;
			TextStyle.FontFamily = TitleFontName;
			TextStyle.Foreground = FontColor;
			TextStyle.Bold = false;
			TextStyle.CharSpacing = 1;

			StyleSheet Style;
			Style.Background = Grey2;
			Style.BorderBrush = Accent3;
			Style.BorderThickness = 4;
			Style.Radius = 0;

			int OrigX = XCoord;

			FileMsCmd = new Button(XCoord, YCoord, Width, Height, L"File", RibbonBase, (HMENU)1, ins, Style, TextStyle);
			XCoord += 5 + Width;

			EditMsCmd = new Button(XCoord, YCoord, Width, Height, L"Edit", RibbonBase, (HMENU)3, ins, Style, TextStyle);
			XCoord += 5 + Width;

			WindowMsCmd = new Button(XCoord, YCoord, Width, Height, L"Window", RibbonBase, (HMENU)7, ins, Style, TextStyle);
			XCoord += 5 + Width;

			HelpMsCmd = new Button(XCoord, YCoord, Width, Height, L"Help", RibbonBase, (HMENU)8, ins, Style, TextStyle);

			XCoord = OrigX;
			YCoord += 5 + Height;
			Style.Background = Grey2;
			Style.BorderThickness = 0;

			Width = WndRect.right - (XCoord * 2);
			Height = WndRect.bottom - 3 - YCoord;

			FileGrid = new Grid(XCoord, YCoord, Width, Height, RibbonBase, ins, Style);
			EditGrid = new Grid(XCoord, YCoord, Width, Height, RibbonBase, ins, Style);
			WindowGrid = new Grid(XCoord, YCoord, Width, Height, RibbonBase, ins, Style);
			HelpGrid = new Grid(XCoord, YCoord, Width, Height, RibbonBase, ins, Style);

			HideRibbonGrids();
			//ShowWindow(*FileGrid, SW_SHOW);
		}

		TextStyleSheet TextStyle;
		TextStyle.FontSize = 13;
		TextStyle.FontFamily = StandardFontName;
		TextStyle.Foreground = FontColor;
		TextStyle.Bold = false;

		StyleSheet Style;
		Style.BorderBrush = Accent3;
		Style.BorderThickness = 3;
		Style.Background = Grey2;
		Style.Radius = 20;

		{
			RECT ThisRect = RECT();
			GetClientRect(FileGrid, &ThisRect);

			int XCoord = 5;
			int YCoord = 3;
			int Width = 100;
			int Height = ThisRect.bottom - (YCoord * 2);

			OpenCmd = new Button(XCoord, YCoord, Width, Height, L"Open", FileGrid, (HMENU)10, ins, Style, TextStyle);
			XCoord += 3 + Width;

			NewCmd = new Button(XCoord, YCoord, Width, Height, L"New", FileGrid, (HMENU)11, ins, Style, TextStyle);
			XCoord += 3 + Width;

			CloseCmd = new Button(XCoord, YCoord, Width, Height, L"Close", FileGrid, (HMENU)12, ins, Style, TextStyle);
			XCoord += 12 + Width;

			SaveCmd = new Button(XCoord, YCoord, Width, Height, L"Save", FileGrid, (HMENU)13, ins, Style, TextStyle);
			XCoord += 3 + Width;

			SaveAsCmd = new Button(XCoord, YCoord, Width, Height, L"Save As", FileGrid, (HMENU)14, ins, Style, TextStyle);
			XCoord += 3 + Width;

			ApplyCmd = new Button(XCoord, YCoord, Width, Height, L"Apply", FileGrid, (HMENU)15, ins, Style, TextStyle);
			XCoord += 3 + Width;

			ApplyAllCmd = new Button(XCoord, YCoord, Width, Height, L"Apply All", FileGrid, (HMENU)16, ins, Style, TextStyle);
		}
	}
	void Main::ResizeRibbon()
	{
		RECT WndRect;
		GetClientRect(RibbonBase, &WndRect);

		{
			int XCoord = 5;
			int YCoord = 3;
			int Width = 140;
			int Height = 33;

			int OrigX = XCoord;

			FileMsCmd->Move(XCoord, YCoord, Width, Height);
			XCoord += 5 + Width;

			EditMsCmd->Move(XCoord, YCoord, Width, Height);
			XCoord += 5 + Width;

			WindowMsCmd->Move(XCoord, YCoord, Width, Height);
			XCoord += 5 + Width;

			HelpMsCmd->Move(XCoord, YCoord, Width, Height);

			XCoord = OrigX;
			YCoord += 5 + Height;

			Width = WndRect.right - (XCoord * 2);
			Height = WndRect.bottom - 3 - YCoord;

			FileGrid->Move(XCoord, YCoord, Width, Height);
			EditGrid->Move(XCoord, YCoord, Width, Height);
			WindowGrid->Move(XCoord, YCoord, Width, Height);
			HelpGrid->Move(XCoord, YCoord, Width, Height);
		}
	}
	void Main::ProcessRibbon(WPARAM wp, LPARAM lp)
	{
		HINSTANCE ins = reinterpret_cast<HINSTANCE>(GetWindowLongPtrW(_Base, GWLP_HINSTANCE));

		switch (wp)
		{
		case 1: //File		
			HideRibbonGrids();
			ShowWindow(*FileGrid, SW_SHOW);
			break;
		case 2: //Unused
			break;
		case 3: //Edit
			HideRibbonGrids();
			ShowWindow(*EditGrid, SW_SHOW);
			break;
		case 7: //Window
			HideRibbonGrids();
			ShowWindow(*WindowGrid, SW_SHOW);
			break;
		case 8: //Help
			HideRibbonGrids();
			ShowWindow(*HelpGrid, SW_SHOW);
			break;
		case 9: //Unused
			break;

			//File
		case 10: //Open
		case 11: //New
		case 12: //Close
		case 13: //Save
		case 14: //SaveAs
		case 15: //Apply
		case 16: //Apply All
		case 17: //Unused
		case 18: //Unused	
		case 19: //Unused
			break;

			//Unused (OLD: Project)
		case 20: //Import			
		case 21: //Unused
		case 22: //Unused
		case 23: //Unused
		case 24: //Unused		
		case 25: //Unused
		case 26: //Unused
		case 27: //Unused
		case 28: //Unused
		case 29: //Unused
			break;

			//Edit
		case 30: //Settings
		case 31: //ProjectSettings
		case 32: //RefrenceGroups
		case 33: //QuickSearch
		case 34: //Unused
		case 35: //Unused
		case 36: //Unused
		case 37: //Unused
		case 38: //Images
		case 39: //CreateImage	
			break;

			//Tasks
		case 40: //Tasks
		case 41: //AddTask
		case 42: //Completed Tasks
		case 43: //Unused
		case 44: //Unused
		case 45: //Unused
		case 46: //Unused
		case 47: //Unused
		case 48: //Unused
		case 49: //Unused
			break;

			//Inventory
		case 50: //Inventory
		case 51: //AddInvItem
		case 52: //Oper inventory
		case 53: //Add oper inventory item
		case 54: //Inventory Search
		case 55: //Unused
		case 56: //Unused
		case 57: //Unused
		case 58: //Unused
		case 59: //Unused
			break;

			//User
		case 60: //Sign In
		case 61: //Sign Out
		case 62: //Lock
		case 63: //CurrentUser
		case 64: //Users
		case 65: //Add User
		case 66: //UserSearch
		case 67: //JobPositions
		case 68: //Timecard
		case 69: //Timeclock
			break;

			//Window
		case 70: //Maxmimize
		case 71: //Minimize
		case 72: //Restore
		case 73:
		case 74: //Quit
		case 75: //Unused
		case 76: //Unused
		case 77: //Unused
		case 78: //Unused
		case 79: //Unused
			break;

			//Help
		case 80: //About
		case 81: //Unused
		case 82: //Unused
		case 83: //Unused
		case 84: //Unused
		case 85: //Unused
		case 86: //Unused
		case 87: //Unused
		case 88: //Unused
		case 89: //Unused
			break; 
		}
	}
}