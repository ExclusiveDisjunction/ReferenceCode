#include "CommandWindow.h"

#include <sstream>

CommandWindow::CommandWindow(HINSTANCE ins, FlowFieldGen* Host)
{
	if (!ThisAtom)
		InitAtom(ins);

	if (!ThisAtom) //Still not loaded, error
		throw std::logic_error("The atom could not be loaded.");

	Wnd = CreateWindowEx(0l, MAKEINTATOM(CommandWindow::ThisAtom), L"Flow: Commands", WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, 400, 500, NULL, NULL, ins, this);

	if (!Host)
		throw std::logic_error("The command window could not be resolved.");

	this->Host = Host;
}
CommandWindow::~CommandWindow()
{
	if (Wnd)
		DestroyWindow(Wnd);
}

ATOM CommandWindow::ThisAtom = NULL;
void CommandWindow::InitAtom(HINSTANCE ins)
{
	if (ThisAtom)
		return;

	WNDCLASS wn = { 0 };
	wn.lpfnWndProc = CommandWindow::WndProc;
	wn.lpszClassName = L"CommandWindow";
	wn.hCursor = LoadCursor(NULL, IDC_ARROW);
	wn.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wn.hInstance = ins;
	wn.cbClsExtra = 0;
	wn.cbWndExtra = sizeof(CommandWindow*);
	wn.hbrBackground = (HBRUSH)(COLOR_WINDOW + 3);
	wn.lpszMenuName = NULL;
	wn.style = CS_HREDRAW | CS_VREDRAW;

	ThisAtom = RegisterClass(&wn);
}
LRESULT __stdcall CommandWindow::WndProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp)
{
	if (Message == WM_CREATE)
	{
		LPCREATESTRUCT pcs = (LPCREATESTRUCT)lp;
		CommandWindow* Data = (CommandWindow*)pcs->lpCreateParams;

		SetWindowLongPtr(Window, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(Data));

		return 1;
	}
	else
	{
		LONG_PTR UserData = GetWindowLongPtr(Window, GWLP_USERDATA);
		CommandWindow* Wnd = reinterpret_cast<CommandWindow*>(UserData);

		if (!Wnd)
			return DefWindowProc(Window, Message, wp, lp);

		switch (Message)
		{
		case WM_DESTROY:
		{
			Wnd->Wnd = NULL;
			SetWindowLongPtr(Window, GWLP_USERDATA, 0);
			delete Wnd;
			PostQuitMessage(0);
			return 1;
		}
		case WM_PAINT:
			Wnd->Paint();
			return 0;
		case WM_COMMAND:
			Wnd->Command(wp, lp);
			return 0;
		case WM_CLOSE:
			DestroyWindow(Wnd->Wnd);
			return 0;

		default:
			return DefWindowProc(Window, Message, wp, lp);
		}
	}
}

void CommandWindow::Paint()
{
	PAINTSTRUCT ps;
	HDC dc = BeginPaint(Wnd, &ps);

	HBRUSH bk = CreateSolidBrush(RGB(0x33, 0x33, 0x33));
	FillRect(dc, &ps.rcPaint, bk);

	DeleteObject(bk);
	EndPaint(Wnd, &ps);
}
void CommandWindow::Command(WPARAM wp, LPARAM lp)
{
	switch (wp)
	{
	case 1: //PerlinSubmit
	{
		int X = 0, Y = 0;

		{
			String XStr = PerlinWidth->GetText(), YStr = PerlinHeight->GetText();

			std::wstringstream XSStream(XStr), YSStream(YStr);
			XSStream >> X;
			YSStream >> Y;
		}

		if (X < 0 || Y < 0)
		{
			MessageBox(Wnd, TEXT("The X and Y values must be greater than zero"), TEXT("Error"), MB_ICONERROR | MB_OK);
			break;
		}

		if (Host)
		{
			Host->FillGrid(X, Y);
			Host->DisplayGrid(true);
		}

		EnableWindow(*PerlinSettings, FALSE);
		EnableWindow(*PtsSettings, TRUE);
		break;
	}
	case 4: //PtsSubmit
	{
		int PtsCnt = 0;

		{
			String Raw = PtsCount->GetText();
			std::wstringstream SStream(Raw);
			SStream >> PtsCnt;
		}

		if (PtsCnt < 0 || PtsCnt > 400)
		{
			MessageBox(Wnd, TEXT("The points is out of bounds (0, 400)"), TEXT("Error"), MB_ICONERROR | MB_OK);
			break;
		}

		if (Host)
		{
			Host->FillParticles(PtsCnt);
			Host->DisplayParticles(true);
		}
	
		EnableWindow(*PtsSettings, FALSE);
		EnableWindow(*BeginRender, TRUE);
		break;
	}

	case 7: //StartRender
	{
		if (!Host)
			break;

		Host->DisplayGrid(false);
		Host->DisplayParticles(false);
		Host->AnimateParticles();

		EnableWindow(*BeginRender, FALSE);
		break;
	}
	case 12: //Minimize
	{
		::ShowWindow(Wnd, SW_MINIMIZE);
		break;
	}
	case 10: //Reset All
		PerlinWidth->SetText(TEXT("0"));
		PerlinHeight->SetText(TEXT("0"));
		PtsCount->SetText(TEXT("0"));
		Host->ClearAll();

		EnableWindow(*PerlinSettings, TRUE);
		EnableWindow(*PtsSettings, FALSE);
		EnableWindow(*BeginRender, FALSE);
		break;
	case 11: //Quit
		SendMessage(Wnd, WM_CLOSE, 0, 0);
		break;
	default:
		return;
	}
}

void CommandWindow::LoadControls()
{
	if (!Wnd)
		return;

	HINSTANCE ins = reinterpret_cast<HINSTANCE>(GetWindowLongPtr(Wnd, GWLP_HINSTANCE));
	RECT WndRect;
	GetClientRect(Wnd, &WndRect);

	int X = 10, Y = 10;

	StyleSheet Style;
	AaColor BaseBk = 0xFF333333;
	Style.Background = 0xFF444444;
	Style.BorderBrush = 0x00FFFFFF;
	Style.BorderThickness = 0;
	Style.Radius = 0;

	StyleSheet BttnStyle = Style;
	BttnStyle.BorderBrush = 0xFF702963;
	BttnStyle.Background = 0x555555;
	BttnStyle.BorderThickness = 4;
	BttnStyle.Radius = 10;

	TextStyleSheet TextStyle;
	TextStyle.FontFamily = TEXT("Segoe");
	TextStyle.FontSize = 17;
	TextStyle.Foreground = 0xFFFFFFFF;

	TitleLbl = new Label(X, Y, WndRect.right - X - 10, 30, Wnd, ins, String(TEXT("Flow")), BaseBk, TextStyle, false);
	Y += 15 + 30;
	TextStyle.FontSize = 13;

	PerlinSettings = new Grid(X, Y, WndRect.right - X - 10, 150, Wnd, ins, Style);
	{
		RECT PerlinRc;
		GetClientRect(PerlinSettings, &PerlinRc);

		int pX = 5, pY = 5;
		int Width = 100;
		int Height = 27;

		TextStyle.Italic = true;
		PerlinLbl = new Label(pX, pY, PerlinRc.right - pX - 5, Height, PerlinSettings, ins, TEXT("Perlin Settings"), Style.Background, TextStyle, false);
		TextStyle.Italic = false;
		int pYTemp = pY += 7 + Height;

		//Column one, labels
		MiscControls.push_back(new Label(pX, pY, Width, Height, PerlinSettings, ins, TEXT("Width: "), Style.Background, TextStyle, false));
		pY += 5 + Height;
		MiscControls.push_back(new Label(pX, pY, Width, Height, PerlinSettings, ins, TEXT("Height:"), Style.Background, TextStyle, false));
		pY += 5 + Height;

		pY = pYTemp;
		pX += Width + 5;
		Width = PerlinRc.right - pX - 5;

		//Column two, textboxes
		PerlinWidth = new TextBox(pX, pY, Width, Height, PerlinSettings, ins, TEXT("0"), BttnStyle, TextStyle);
		pY += Height + 5;
		
		PerlinHeight = new TextBox(pX, pY, Width, Height, PerlinSettings, ins, TEXT("0"), BttnStyle, TextStyle);
		pY += Height + 7;

		pX = 5;
		Width = PerlinRc.right - 5 - 5;
		PerlinSubmit = new Button(pX, pY, Width, Height, TEXT("Submit"), PerlinSettings, (HMENU)1, ins, BttnStyle, TextStyle);
		pY += Height + 5;

		//Resize parent window
		{
			RECT WndLoc;
			GetWindowRect(PerlinSettings, &WndLoc);
			MapWindowPoints(NULL, Wnd, (LPPOINT)&WndLoc, 2);
			MoveWindow(*PerlinSettings, WndLoc.left, WndLoc.top, PerlinRc.right, pY, true);

			Y += 10 + pY; //Updates the outer Y.
		}
	}

	//Points
	PtsSettings = new Grid(X, Y, WndRect.right - X - 10, 120, Wnd, ins, Style);
	{
		RECT PtsRc;
		GetClientRect(PerlinSettings, &PtsRc);

		int pX = 5, pY = 5;
		int Width = 100;
		int Height = 27;

		TextStyle.Italic = true;
		PtsLbl = new Label(pX, pY, PtsRc.right - pX - 5, Height, PtsSettings, ins, TEXT("Point Settings"), Style.Background, TextStyle, false);
		TextStyle.Italic = false;
		pY += 7 + Height;

		MiscControls.push_back(new Label(pX, pY, Width, Height, PtsSettings, ins, TEXT("# Of Points: "), Style.Background, TextStyle, false));

		//Since pY was never incremented it never has to reset
		pX += Width + 5;
		Width = PtsRc.right - pX - 5;

		PtsCount = new TextBox(pX, pY, Width, Height, PtsSettings, ins, TEXT("0"), BttnStyle, TextStyle);
		pY += Height + 7;

		pX = 5;
		Width = PtsRc.right - 5 - 5;
		PtsSubmit = new Button(pX, pY, Width, Height, TEXT("Submit"), PtsSettings, (HMENU)4, ins, BttnStyle, TextStyle);
		pY += Height + 5;

		//Resize parent window
		{
			RECT WndLoc;
			GetWindowRect(PtsSettings, &WndLoc);
			MapWindowPoints(NULL, Wnd, (LPPOINT)&WndLoc, 2);
			MoveWindow(*PtsSettings, WndLoc.left, WndLoc.top, PtsRc.right, pY, true);

			Y += 10 + pY; //Updates the outer Y.
		}
	}

	//Commands
	Commands = new Grid(X, Y, WndRect.right - X - 10, 120, Wnd, ins, Style);
	{
		RECT CmdRc;
		GetClientRect(PerlinSettings, &CmdRc);

		int pX = 5, pY = 5;
		int Width = ((CmdRc.right - 5 - 5) - 10) / 3;
		int Height = 27;

		TextStyle.Italic = true;
		CommandsLbl = new Label(pX, pY, CmdRc.right - pX - 5, Height, Commands, ins, TEXT("Commands"), Style.Background, TextStyle, false);
		TextStyle.Italic = false;
		pY += 7 + Height;

		BeginRender = new Button(pX, pY, Width * 3 + 10, Height, TEXT("Start Calculations"), Commands, (HMENU)7, ins, BttnStyle, TextStyle);
		pX = 5;
		pY += 5 + Height;

		ClearAll = new Button(pX, pY, Width, Height, TEXT("Reset"), Commands, (HMENU)10, ins, BttnStyle, TextStyle);
		pX += 5 + Width;

		Minimize = new Button(pX, pY, Width, Height, TEXT("Minimize"), Commands, (HMENU)12, ins, BttnStyle, TextStyle);
		pX += 5 + Width;

		BttnStyle.BorderBrush = 0xFFFF0000;
		Quit = new Button(pX, pY, Width, Height, TEXT("Quit"), Commands, (HMENU)11, ins, BttnStyle, TextStyle);

		pY += Height + 5;
		//Resize parent window
		{
			RECT WndLoc;
			GetWindowRect(Commands, &WndLoc);
			MapWindowPoints(NULL, Wnd, (LPPOINT)&WndLoc, 2);
			MoveWindow(*Commands, WndLoc.left, WndLoc.top, CmdRc.right, pY, true);

			Y += 10 + pY; //Updates the outer Y.
		}
	}

	SendMessage(Wnd, WM_COMMAND, (WPARAM)10, 0); //Sends a message to reset the window to natural state.
}
void CommandWindow::ShowWindow(int ShowCommand)
{
	if (!Wnd)
		return;

	UpdateWindow(Wnd);
	::ShowWindow(Wnd, ShowCommand);
}
