#pragma once

#include <Windows.h>

#include "..\UICommon.h"
#include "..\UI\Controls\ControlBase.h"

#include ".\Core\Vector.h"
#include ".\Math\Function\FunctionBase.h"

namespace Mason::Graph
{
	class GraphControl : public UI::Controls::ControlBase
	{
	private:
		bool IsMouseDown = false, IsZooming = false;

		virtual void ZoomThis(double By) = 0;

		virtual LRESULT Paint() override = 0;
		LRESULT Size() override { return 0; }
		virtual LRESULT MouseDown(WPARAM wp, LPARAM lp) = 0;
		virtual LRESULT MouseMove(WPARAM wp, LPARAM lp) = 0;
		virtual LRESULT MouseUp(WPARAM wp, LPARAM lp) = 0;
		virtual LRESULT MouseWheel(WPARAM wp) = 0;
		virtual LRESULT KeyDown(WPARAM Key) override = 0;
		virtual LRESULT KeyUp(WPARAM Key) = 0;

		static ATOM _ThisAtom;
		static void InitBase(HINSTANCE ins);
		static LRESULT WndProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp);
		
	public:
		GraphControl() = delete;
		GraphControl(RECT Position, HWND Parent, HINSTANCE ins);
		GraphControl(RECT Position, UI::Controls::ControlBase * Parent, HINSTANCE ins);
		GraphControl(const GraphControl& Obj) = delete;
		GraphControl(GraphControl&& Obj) noexcept = delete;
		virtual ~GraphControl();

		GraphControl& operator=(const GraphControl& Obj) = delete;
		GraphControl& operator=(GraphControl&& Obj) noexcept = delete;

		double Zoom = 1.0;

		void Move(RECT Position);
		void Move(int X, int Y, int Width, int Height) { Move({ X, Y, Width, Height }); }
	};
}