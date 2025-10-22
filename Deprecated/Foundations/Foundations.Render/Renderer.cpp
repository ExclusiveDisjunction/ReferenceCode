#include "Renderer.h"

#include "Renderable.h"

namespace Core::Render
{
	Renderer::Renderer()
	{

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

		DiscardDependentResources();
		SafeRelease(&Factory);
	}

	bool Renderer::Init()
	{
		HRESULT hr;

		hr = CreateIndependentResources();
		if (FAILED(hr))
			return false;

		hr = CreateDependentResources(Factory);
		if (FAILED(hr))
			return false;

		ID2D1RenderTarget* Target = GetTarget();
		bool Return = true;
		for (Renderable*& Ren : Renderables)
			Return &= Ren->ConstructResources(Target);

		return Return;
	}
	D2D1_SIZE_F Renderer::GetSize() const
	{
		ID2D1RenderTarget* Target = GetTarget();
		if (Target)
			return Target->GetSize();
		else
			return { 0.0f, 0.0f};
	}

	D2D1_MATRIX_3X2_F Renderer::GetTransform() const
	{
		return Transform;
	}
	void Renderer::SetTransform(const D2D1_MATRIX_3X2_F& New)
	{
		this->Transform = New;
	}

	HRESULT Renderer::CreateIndependentResources()
	{
		HRESULT hr = S_OK;
		hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &Factory);

		return hr;
	}

	void Renderer::DiscardDependentResources()
	{
		for (Renderable* Item : Renderables)
			Item->ReleaseResources();
	}

	HRESULT Renderer::OnRender()
	{
		HRESULT hr = S_OK;
		hr = CreateDependentResources(Factory);

		if (SUCCEEDED(hr))
		{
			ID2D1RenderTarget* RenderTarget = GetTarget();

			RenderTarget->BeginDraw();
			RenderTarget->SetTransform(Transform);
			RenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

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
				DiscardDependentResources();
			}
		}

		return hr;
	}
}