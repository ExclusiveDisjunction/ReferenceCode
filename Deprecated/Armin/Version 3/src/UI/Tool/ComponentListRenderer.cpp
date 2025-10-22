#include "ComponentListRenderer.h"

#include "..\..\Ins.h"
#include "..\..\UserRegistry.h"
#include "..\..\UICommon.h"
#include "..\..\Files\Components.h"
#include "..\..\Editors\EditorRegistry.h"

namespace Armin::UI::Tool
{
	using namespace Files;
	using namespace Editors;

	ComponentListRenderer::ComponentListRenderer(const Vector<Component*>& Source, HINSTANCE ins, String WindowTitle, bool CanEdit)
	{
		_Source = Source;
		_CanEdit = CanEdit;
		if (!_ThisAtom)
			InitBase(ins);

		_Base = CreateWindowExW(0l, MAKEINTATOM(_ThisAtom), static_cast<LPCWSTR>(WindowTitle), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 550, 600, NULL, NULL, ins, NULL);
		SetWindowLongPtrW(_Base, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

		ShowWindow(_Base, SW_NORMAL);
		UpdateWindow(_Base);

		LoadControls();
	}
	ComponentListRenderer::ComponentListRenderer(const ReferenceList& Source, HINSTANCE ins, String WindowTitle, bool CanEdit) : ComponentListRenderer(Source.operator Vector<ComponentReference*>(), ins, WindowTitle, CanEdit) {}
	ComponentListRenderer::ComponentListRenderer(const Vector<ComponentReference*>& Source, HINSTANCE ins, String WindowTitle, bool CanEdit) : ComponentListRenderer(ComponentReference::Convert<Component>(Source), ins, WindowTitle, CanEdit) {}
	ComponentListRenderer::~ComponentListRenderer()
	{
		SetWindowLongPtrW(_Base, GWLP_USERDATA, 0);
		DestroyWindow(_Base);

		if (Objects)
			delete Objects;

		Destroy();
	}

	void ComponentListRenderer::ChangeSource(const Vector<Component*>& Source)
	{
		_Source = Source;

		FillObjects();
	}
	void ComponentListRenderer::ChangeSource(const Vector<ComponentReference*>& Source)
	{
		ChangeSource(ComponentReference::Convert<Component>(Source));
	}

	ATOM ComponentListRenderer::_ThisAtom = ATOM();
	void ComponentListRenderer::InitBase(HINSTANCE ins)
	{
		WNDCLASSW wn = { 0 };
		wn.lpfnWndProc = ComponentListRenderer::WndProc;
		wn.hInstance = ins;
		wn.hCursor = LoadCursorW(NULL, IDC_ARROW);
		wn.hbrBackground = NULL;
		wn.lpszMenuName = NULL;
		wn.lpszClassName = L"SearchResultClass";
		wn.hIcon = ArminIcon;
		wn.cbClsExtra = 0;
		wn.cbWndExtra = sizeof(ComponentListRenderer*);
		wn.style = CS_HREDRAW | CS_VREDRAW;

		_ThisAtom = RegisterClassW(&wn);
	}
	LRESULT __stdcall ComponentListRenderer::WndProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp)
	{
		ComponentListRenderer* Item = reinterpret_cast<ComponentListRenderer*>(GetWindowLongPtrW(Window, GWLP_USERDATA));
		if (!Item)
			return DefWindowProcW(Window, Message, wp, lp);

		switch (Message)
		{
		case WM_PAINT:
			return Item->Paint();
		case WM_DESTROY:
		{
			LRESULT Return = Item->Destroy();
			delete Item;
			return Return;
		}
		case WM_COMMAND:
			return Item->Command(wp, lp);
		case WM_SIZE:
			return Item->Size();
		default:
			return DefWindowProcW(Window, Message, wp, lp);
		}
	}

	void ComponentListRenderer::LoadControls()
	{
		RECT WndRect;
		GetClientRect(_Base, &WndRect);
		HINSTANCE ins = reinterpret_cast<HINSTANCE>(GetWindowLongPtrW(_Base, GWLP_HINSTANCE));

		_Loaded = true;

		{
			int Spacing = 10;

			int Width = 40;
			int Height = 40;
			int XCoord = 10;
			int YCoord = (WndRect.bottom / 2) - (Height + Spacing + (Height / 2));

			TextStyleSheet TextStyle;
			TextStyle.FontFamily = StandardFontName;
			TextStyle.FontSize = 15;
			TextStyle.Foreground = FontColor;

			StyleSheet Style;
			Style.Background = Style.BaseBackground = Background2;
			Style.BorderBrush = Accent4;
			Style.BorderThickness = 3;
			Style.Radius = 20;

			View = new Button(XCoord, YCoord, Width, Height, L"VI", _Base, (HMENU)1, ins, Style, TextStyle);
			YCoord += Height + Spacing;

			Edit = new Button(XCoord, YCoord, Width, Height, L"ED", _Base, (HMENU)2, ins, Style, TextStyle);
			EnableWindow(*Edit, _CanEdit);
			YCoord += Height + Spacing;

			SaveResult = new Button(XCoord, YCoord, Width, Height, L"SR", _Base, (HMENU)3, ins, Style, TextStyle);
			YCoord += Height + Spacing;

			XCoord += Spacing + Width;

			YCoord = 20;
			Width = WndRect.right - 20 - XCoord;
			Height = WndRect.bottom - 20 - YCoord;

			Style.Background = Grey1;
			Style.BorderThickness = 0;

			ObjectScroll = new ScrollViewer(XCoord, YCoord, Width, Height, _Base, ins, Style);

			GetClientRect(ObjectScroll, &WndRect);
			XCoord = 0;
			YCoord = 0;
			Width = WndRect.right - XCoord;
			Height = WndRect.bottom - YCoord;

			ObjectView = new Grid(XCoord, YCoord, Width, Height, ObjectScroll, ins, Style);
			ObjectScroll->SetViewer(ObjectView);

			Objects = new ComponentViewerList(ObjectView, ObjectScroll);
		}

		FillObjects();
	}
	void ComponentListRenderer::ResizeObjects()
	{
		Objects->ReSizeList();
	}
	void ComponentListRenderer::FillObjects()
	{
		Objects->GenerateList(_Source, NULL, MultiselectMode, _CanEdit);
	}

	LRESULT ComponentListRenderer::Paint()
	{
		PAINTSTRUCT ps;
		HDC Dc = BeginPaint(_Base, &ps);

		RECT WndRect;
		GetClientRect(_Base, &WndRect);

		HBRUSH Bk = CreateSolidBrush(Background2);
		FillRect(Dc, &WndRect, Bk);

		DeleteObject(Bk);
		EndPaint(_Base, &ps);
		return 0;
	}
	LRESULT ComponentListRenderer::Destroy()
	{
		return 0;
	}
	LRESULT ComponentListRenderer::Command(WPARAM wp, LPARAM lp)
	{
		switch (wp)
		{
		case 1: //View
		case 2: //Edit
			Objects->OpenSelectedForEditView(wp == 2 && _CanEdit);
			break;
		case 3: //SaveResult
			Objects->SaveSelectedAsGroup();
			break;
		}

		return 0;
	}
	LRESULT ComponentListRenderer::Size()
	{
		RECT WndRect;
		GetClientRect(_Base, &WndRect);

		if (!_Loaded)
			return 0;

		{
			int Spacing = 10;

			int Width = 40;
			int Height = 40;
			int XCoord = 10;
			int YCoord = (WndRect.bottom / 2) - (Height + Spacing + (Height / 2));

			View->Move(XCoord, YCoord, Width, Height);
			YCoord += Height + Spacing;

			Edit->Move(XCoord, YCoord, Width, Height);
			YCoord += Height + Spacing;

			SaveResult->Move(XCoord, YCoord, Width, Height);
			YCoord += Height + Spacing;

			XCoord += Spacing + Width;

			YCoord = 20;
			Width = WndRect.right - 20 - XCoord;
			Height = WndRect.bottom - 20 - YCoord;

			ObjectScroll->Move(XCoord, YCoord, Width, Height);

			GetClientRect(ObjectScroll, &WndRect);
			XCoord = 0;
			YCoord = 0;
			Width = WndRect.right - XCoord;
			Height = WndRect.bottom - YCoord;

			ObjectView->Move(XCoord, YCoord, Width, Height);
		}

		ResizeObjects();

		return 0;
	}
}