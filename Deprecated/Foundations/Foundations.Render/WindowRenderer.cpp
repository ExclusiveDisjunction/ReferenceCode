#include "WindowRenderer.h"

#include <iostream>

namespace Core::Render
{
	WindowRenderer::WindowRenderer(HINSTANCE ins, HWND Parent, int X, int Y, int Width, int Height)
	{
		if (!ThisAtom && !InitAtom(ins))
			throw std::logic_error("The ATOM for this class could not be made.");

		Wnd = CreateWindowEx(0l, MAKEINTATOM(ThisAtom), TEXT(""), WS_CHILD | WS_VISIBLE, X, Y, Width, Height, Parent, NULL, ins, this);
		if (!Wnd)
			throw std::logic_error("The window could not be constructed.");

		UpdateWindow(Wnd);
	}
	WindowRenderer::WindowRenderer(HINSTANCE ins, const TCHAR* Title, int X, int Y, int Width, int Height)
	{
		if (!ThisAtom && !InitAtom(ins))
			throw std::logic_error("The ATOM for this class could not be made.");

		Wnd = CreateWindowEx(0l, MAKEINTATOM(ThisAtom), Title, WS_OVERLAPPEDWINDOW, X, Y, Width, Height, NULL, NULL, ins, this);
		if (!Wnd)
			throw std::logic_error("The window could not be constructed.");

		ShowWindow(Wnd, SW_SHOWNORMAL);
		UpdateWindow(Wnd);
	}
	WindowRenderer::~WindowRenderer()
	{
		if (Wnd)
		{
			SetWindowLongPtr(Wnd, GWLP_USERDATA, 0l);
			DestroyWindow(Wnd);
		}
	}

	ATOM WindowRenderer::ThisAtom = NULL;
	bool WindowRenderer::InitAtom(HINSTANCE ins)
	{
		if (ThisAtom)
			return true;

		WNDCLASSEX wn = { 0 };
		wn.cbSize = sizeof(WNDCLASSEX);
		wn.lpszClassName = TEXT("RENDERWINDOW");
		wn.cbClsExtra = 0;
		wn.cbWndExtra = sizeof(WindowRenderer*);
		wn.hbrBackground = (HBRUSH)COLOR_WINDOW;
		wn.hCursor = LoadCursor(NULL, IDC_ARROW);
		wn.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wn.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
		wn.hInstance = ins;
		wn.lpfnWndProc = WindowRenderer::WndProc;
		wn.lpszMenuName = NULL;
		wn.style = CS_HREDRAW | CS_VREDRAW;

		ThisAtom = RegisterClassEx(&wn);
		return ThisAtom != NULL;
	}
	LRESULT __stdcall WindowRenderer::WndProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp)
	{
		if (Message == WM_CREATE)
		{
			LPCREATESTRUCT lpc = (LPCREATESTRUCT)lp;
			SetWindowLongPtr(Window, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(lpc->lpCreateParams));
			return 1;
		}
		else
		{
			WindowRenderer* Target = reinterpret_cast<WindowRenderer*>(GetWindowLongPtr(Window, GWLP_USERDATA));
			if (!Target)
				return DefWindowProc(Window, Message, wp, lp);

			switch (Message)
			{
			case WM_DESTROY:
			{
				Target->Wnd = NULL;
				delete Target;

				return 1;
			}
			case WM_PAINT:
				Target->OnPaint();
				ValidateRect(Window, NULL);
				return 0;

			case WM_DISPLAYCHANGE:
				InvalidateRect(Window, NULL, false);
				return 0;
			case WM_SIZE:
			{
				UINT width = LOWORD(lp),
					height = HIWORD(lp);
				Target->OnSize(width, height);
				return 0;
			}
			default:
				return DefWindowProc(Window, Message, wp, lp);
			}
		}
	}

	void WindowRenderer::OnPaint()
	{
		OnRender();
	}
	void WindowRenderer::OnSize(UINT Width, UINT Height)
	{
		if (Target)
		{
			D2D1_SIZE_U Size = { Width, Height };
			Target->Resize(Size);
			for (std::pair<void* const, std::function<void(void*, D2D1_SIZE_U)>>& Event : OnSizeEvents)
				Event.second(Event.first, Size);
		}		
	}

	HRESULT WindowRenderer::CreateDependentResources(ID2D1Factory* Factory)
	{
		HRESULT hr = S_OK;
		if (!Target)
		{
			RECT rc;
			GetClientRect(Wnd, &rc);

			D2D1_SIZE_U size = { static_cast<UINT32>(rc.right - rc.left), static_cast<UINT32>(rc.bottom - rc.top) };

			hr = Factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(Wnd, size), &Target);
			if (FAILED(hr))
				return hr;
		}

		return hr;
	}
	void WindowRenderer::DiscardDependentResources()
	{
		Renderer::DiscardDependentResources();
		SafeRelease(&Target);
	}

	void WindowRenderer::RequestRedraw()
	{
		InvalidateRect(Wnd, nullptr, true);
	}

	void WindowRenderer::RegisterOnSizeEvent(void* Obj, const std::function<void(void*, D2D1_SIZE_U)>& Callable)
	{
		OnSizeEvents.try_emplace(Obj, Callable);
	}
	void WindowRenderer::DeRegisterOnSizeEvent(void* Obj)
	{
		auto Result = OnSizeEvents.find(Obj);
		if (Result == OnSizeEvents.end())
			return;

		OnSizeEvents.erase(Result);
	}
}