#include <Windows.h> //To allow AaColor to output to COLORREF.
#include "SideHost.h"

#include "..\Editor.h"
#include "..\EditorRegistry.h"

namespace Core::Editors
{
	using namespace UI;
	using namespace UI::Controls;	

	SideHost::SideHost(HINSTANCE ins)
	{
		if (!_ThisAtom)
			InitBase(ins);

		_Base = CreateWindowExW(0l, MAKEINTATOM(_ThisAtom), L"Side Host", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1000, 640, NULL, NULL, ins, NULL);
		SetWindowLongPtrW(_Base, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

		ShowWindow(_Base, SW_MAXIMIZE);
		UpdateWindow(_Base);

		LoadControls();

		EditorRegistry::Track(this);
	}
	SideHost::~SideHost()
	{
		EditorRegistry::RemoveTracking(this);

		SetWindowLongPtrW(_Base, GWLP_USERDATA, 0);
		DestroyWindow(_Base);
	}

	ATOM SideHost::_ThisAtom = ATOM();
	void SideHost::InitBase(HINSTANCE ins)
	{
		WNDCLASSW wn = { 0 };
		wn.lpfnWndProc = SideHost::WndProc;
		wn.hInstance = ins;
		wn.hCursor = ProgramCursor;
		wn.hbrBackground = NULL;
		wn.lpszMenuName = NULL;
		wn.lpszClassName = L"SideHost";
		wn.hIcon = ProgramIcon;
		wn.cbClsExtra = 0;
		wn.cbWndExtra = sizeof(SideHost*);
		wn.style = CS_HREDRAW | CS_VREDRAW;

		_ThisAtom = RegisterClassW(&wn);
	}
	LRESULT __stdcall SideHost::WndProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp)
	{
		SideHost* This = reinterpret_cast<SideHost*>(GetWindowLongPtrW(Window, GWLP_USERDATA));
		if (!This)
			return DefWindowProcW(Window, Message, wp, lp);

		switch (Message)
		{
		case WM_PAINT:
			return This->Paint();
		case WM_SIZE:
			return This->Size();
		case WM_COMMAND:
			return This->Command(wp, lp);
		case WM_CLOSE:
			return This->Close();
		case WM_DESTROY:
		{
			SetWindowLongPtrW(Window, GWLP_USERDATA, 0);
			delete This;

			return 0;
		}
		default:
			return DefWindowProc(Window, Message, wp, lp);
		}
	}

	void SideHost::LoadControls()
	{
		if (_Loaded)
			return;

		_Loaded = true;

		HINSTANCE ins = reinterpret_cast<HINSTANCE>(GetWindowLongPtrW(_Base, GWLP_HINSTANCE));
		RECT WndRect;
		GetClientRect(_Base, &WndRect);

		EditorButtonHost::LoadEditorButtonsBase(_Base, ins, 0, 0, WndRect.right, 37, Accent1);
		EditorPlacement = { 20, 37 + 20, 20, 20 };
	}

	LRESULT SideHost::Size()
	{
		if (!_Loaded)
			return 0;

		RECT WndRect;
		GetClientRect(_Base, &WndRect);

		EditorButtonHost::MoveEditorButtonsBase(0, 0, WndRect.right, 37);
		EditorPlacement = { 20, 37 + 20, 20, 20 };

		for (EditorButton* Item : EditorButtons)
		{
			Editor* Current = Item->Current();
			Current->ReSize();
		}

		return 0;
	}
	LRESULT SideHost::Paint()
	{
		PAINTSTRUCT ps;
		HDC Dc = BeginPaint(_Base, &ps);

		RECT WndRect;
		GetClientRect(_Base, &WndRect);

		HBRUSH Bk = CreateSolidBrush(Background1);
		FillRect(Dc, &WndRect, Bk);

		DeleteObject(Bk);
		EndPaint(_Base, &ps);
		return 0;
	}
	LRESULT SideHost::Close()
	{
		Vector<Editor*> CurrentEditors = EditorRegistry::GetEditors(AllEditorsCode, this);
		bool HasApplyable = CurrentEditors.Search([](Editor* Obj) { return Obj->IsApplyable(); });

		if (HasApplyable)
		{
			int Result = MessageBoxW(_Base, L"Would you like to apply all editors first?\n\nIf not, editors will be moved to main window.", L"Side Host:", MB_YESNOCANCEL | MB_ICONQUESTION);

			if (Result == IDYES)
			{
				if (EditorRegistry::CloseEditors(CurrentEditors))
					DestroyWindow(_Base);
				else
					return 0;
			}
			else if (Result == IDNO)
			{
				for (Editor* Frame : CurrentEditors)
					EditorRegistry::MoveEditor(Frame, nullptr, false);
				DestroyWindow(_Base);
			}
			else
				return 0;
		}
		else
		{
			EditorRegistry::CloseEditors(CurrentEditors);
			DestroyWindow(_Base);
		}

		return 0;		
	}
	LRESULT SideHost::Command(WPARAM wp, LPARAM lp)
	{
		return 0;
	}
}