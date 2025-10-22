#pragma once

#include "ControlBase.h"
#include "..\..\Calculation\MathVector.h"

namespace Mason::UI::Controls
{
	class TrackedCoord : public ControlBase
	{
	protected:
		Core::Vector<Calculation::Vector2d*> Modify;
		HWND _Base;
		bool _Track;
		COLORREF Border, Bk;

		static LRESULT __stdcall WndProc(HWND, UINT, WPARAM, LPARAM);
		static ATOM _ThisAtom;
		static void InitBase(HINSTANCE ins);

		LRESULT Paint() override;
		LRESULT Click() override;
		LRESULT MouseUp() override;
		LRESULT Timer(WPARAM wp);

		virtual void OnMove(Calculation::Vector2d NewPlace) {}
	public:
		TrackedCoord(Core::Vector<Calculation::Vector2d*> Bind, int Size, HWND Parent, COLORREF Border, COLORREF Bk, HINSTANCE ins);

		Core::Vector<Calculation::Vector2d*> Binding() const { return Modify; }
		void Binding(Core::Vector<Calculation::Vector2d*> New);
		void AddToBind(Calculation::Vector2d* New);

		operator HWND() const { return _Base; }
	};
}