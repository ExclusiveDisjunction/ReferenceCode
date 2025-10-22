#include "GraphControl.h"

using namespace Mason::UI;
using namespace Mason::UI::Controls;

namespace Mason::Graph
{
	GraphControl::GraphControl(RECT Position, HWND Parent, HINSTANCE ins)
	{
		if (!_ThisAtom)
			InitBase(ins);

		_Base = CreateWindowExW(0l, MAKEINTATOM(_ThisAtom), nullptr, WS_CHILD | WS_VISIBLE, Position.left, Position.right, Position.top, Position.bottom, Parent, nullptr, ins, nullptr);
		SetWindowLongPtrW(_Base, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
	}
	GraphControl::GraphControl(RECT Position, ControlBase* Parent, HINSTANCE ins) : GraphControl(Position, !Parent ? nullptr : *Parent, ins) {}
	GraphControl::~GraphControl()
	{

	}

	ATOM GraphControl::_ThisAtom = ATOM();
	void GraphControl::InitBase(HINSTANCE ins)
	{
		WNDCLASSW wn = { 0 };
		wn.style = CS_HREDRAW | CS_VREDRAW;
		wn.lpfnWndProc = WndProc;
		wn.lpszClassName = L"GraphControlBase";
		wn.hCursor = MasonCursor;
		wn.hInstance = ins;
		wn.cbClsExtra = 0;
		wn.cbWndExtra = sizeof(GraphControl*);
		wn.hIcon = nullptr;
		wn.lpszMenuName = nullptr;
		wn.hbrBackground = nullptr;

		_ThisAtom = RegisterClassW(&wn);
	}
	LRESULT GraphControl::WndProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp)
	{
		void* Temp = reinterpret_cast<void*>(GetWindowLongPtrW(Window, GWLP_USERDATA));
		if (!Temp)
			return DefWindowProc(Window, Message, wp, lp);

		GraphControl* Obj = static_cast<GraphControl*>(Temp);

		switch (Message)
		{
		case WM_DESTROY:
		{
			delete Obj;
			return 0;
		}
		case WM_PAINT:
			return Obj->Paint();
		case WM_SIZE:
			return Obj->Size();
		case WM_LBUTTONDOWN:
			return Obj->MouseDown(wp, lp);
		case WM_LBUTTONUP:
			return Obj->MouseUp(wp, lp);
		case WM_MOUSEMOVE:
			return Obj->MouseMove(wp, lp);
		case WM_MOUSEWHEEL:
			return Obj->MouseWheel(wp);
		case WM_KEYDOWN:
			return Obj->KeyDown(wp);
		case WM_KEYUP:
			return Obj->KeyUp(wp);
		default:
			return ControlBase::CommonWndProc(Window, Message, wp, lp);
		}
	}

	void GraphControl::Move(RECT Position)
	{
		MoveWindow(_Base, Position.left, Position.top, Position.right, Position.bottom, true);
	}
}