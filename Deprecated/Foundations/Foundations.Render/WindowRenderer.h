#pragma once

#include "Renderer.h"

namespace Core::Render
{
	//After the reform of Foundantions.UI, this will be redone to conform to the control system.
	class RENDER_API WindowRenderer : public Renderer
	{
	private:
		ID2D1HwndRenderTarget* Target = nullptr;
		HWND Wnd = NULL;

		std::map<void*, std::function<void(void*, D2D1_SIZE_U)>> OnSizeEvents;

		static ATOM ThisAtom;
		static bool InitAtom(HINSTANCE ins);
		static LRESULT __stdcall WndProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp);
	protected:
		void OnPaint();
		void OnSize(UINT Width, UINT Height);

		HRESULT CreateDependentResources(ID2D1Factory* Factory) override;
		void DiscardDependentResources() override;

		ID2D1RenderTarget* GetTarget() const override { return Target; }
	public:
		WindowRenderer(HINSTANCE ins, HWND Parent, int X, int Y, int Width, int Height);
		WindowRenderer(HINSTANCE ins, const TCHAR* Title, int X, int Y, int Width, int Height);
		~WindowRenderer();

		void RequestRedraw() override;

		void RegisterOnSizeEvent(void* Obj, const std::function<void(void*, D2D1_SIZE_U)>& Callable);
		void DeRegisterOnSizeEvent(void* Obj);
	};
}