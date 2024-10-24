#include "Renderer.h"

#include "Renderable.h"
#include "..\RenderWindow.h"

Renderer::Renderer(RenderWindow* Window)
{
	this->Target = Window;
}
Renderer::~Renderer()
{
	for (auto curr = Renderables.begin(), end = Renderables.end(); curr != end; curr++)
	{
		Renderable*& Ren = *curr;
		Ren->Detach(false);
		delete Ren;
		Ren = nullptr;
	}
	Renderables.clear();

	SafeRelease(&RenderTarget);
	SafeRelease(&Factory);
}

void Renderer::Init()
{
	HRESULT hr;

	hr = CreateDevIndpResx();
	hr = !SUCCEEDED(hr) ? hr : (!this->Target ? -1 : S_OK);

	if (SUCCEEDED(hr))
	{
		HWND Target = this->Target->GetHWND();

		if (Target)
		{
			double dpi = (double)GetDpiForWindow(Target);

			SetWindowPos(Target, NULL, NULL, NULL, static_cast<int>(ceil(640.0f * dpi / 96.0f)), static_cast<int>(ceil(480.0f * dpi / 96.0f)), SWP_NOMOVE);
			ShowWindow(Target, SW_SHOWNORMAL);
			UpdateWindow(Target);
		}
	}
}
D2D1_SIZE_F Renderer::GetSize() const
{
	if (RenderTarget)
		return RenderTarget->GetSize();
	else
		return D2D1_SIZE_F();
}
void Renderer::RequestRedraw()
{
	if (Target)
		InvalidateRect(Target->GetHWND(), nullptr, true);
}

void Renderer::RegisterOnSizeEvent(void* Obj, std::function<void(void*, D2D1_SIZE_U)> Callable)
{
	OnSizeEvents.try_emplace(Obj, Callable);
}
void Renderer::DeRegisterOnSizeEvent(void* Obj)
{
	auto Result = OnSizeEvents.find(Obj);
	if (Result == OnSizeEvents.end())
		return;

	OnSizeEvents.erase(Result);
}

D2D1_MATRIX_3X2_F Renderer::GetTransform() const
{
	return Transform;
}
void Renderer::SetTransform(const D2D1_MATRIX_3X2_F& New)
{
	this->Transform = New;
}

HRESULT Renderer::CreateDevIndpResx()
{
	HRESULT hr = S_OK;
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &Factory);

	return hr;
}
HRESULT Renderer::CreateDevDepResx()
{
	HRESULT hr = S_OK;
	if (!RenderTarget)
	{
		RECT rc;
		GetClientRect(Target->GetHWND(), &rc);

		D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

		hr = Factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(this->Target->GetHWND(), size), &RenderTarget);

		if (FAILED(hr))
			return hr;

		for (Renderable* Item : Renderables)
			Item->ConstructResources(RenderTarget);
	}

	return hr;
}

void Renderer::DiscardDevResx()
{
	for (Renderable* Item : Renderables)
		Item->ReleaseResources();

	SafeRelease(&RenderTarget);
}

HRESULT Renderer::OnRender()
{
	HRESULT hr = S_OK;
	hr = CreateDevDepResx();

	if (SUCCEEDED(hr))
	{
		RenderTarget->BeginDraw();
		RenderTarget->SetTransform(Transform);
		RenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

#if defined(DEBUG) || defined(_DEBUG)
		OutputDebugStringA("Render Started.\n");
#endif

		D2D1_SIZE_F rtSize = RenderTarget->GetSize();

		for (Renderable* Item : Renderables)
		{
			if (Item && Item->IsEnabled())
				Item->OnRender(RenderTarget, rtSize);
		}

		hr = RenderTarget->EndDraw();

		if (hr == D2DERR_RECREATE_TARGET)
		{
			hr = S_OK;
			DiscardDevResx();
		}
	}

	return hr;
}
void Renderer::OnResize(UINT Width, UINT Height)
{
	if (RenderTarget)
	{
		D2D1_SIZE_U Size = D2D1::SizeU(Width, Height);
		RenderTarget->Resize(Size);
		for (std::pair<void * const, std::function<void(void*, D2D1_SIZE_U)>>& Event : OnSizeEvents)
			Event.second(Event.first, Size);
	}
}