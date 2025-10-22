#pragma once

#include "RenderCommon.h"

namespace Core::Render
{
	class RENDER_API Renderer;
	class RENDER_API Renderable;

	class RENDER_API Renderer
	{
	private:
		ID2D1Factory* Factory = nullptr;
		std::vector<Renderable*> Renderables;
		D2D1_MATRIX_3X2_F Transform = D2D1::Matrix3x2F::Identity();

		HRESULT CreateIndependentResources();
	protected:
		//Call this when render is needed.
		HRESULT OnRender();

		virtual HRESULT CreateDependentResources(ID2D1Factory* Factory) = 0;
		virtual void DiscardDependentResources();

		virtual ID2D1RenderTarget* GetTarget() const = 0;
	public:
		Renderer();
		Renderer(const Renderer& Obj) = delete;
		Renderer(Renderer&& Obj) = delete;
		~Renderer();

		friend Renderable;

		Renderer& operator=(const Renderer& Obj) = delete;
		Renderer& operator=(Renderer&& Obj) = delete;

		bool Init();
		D2D1_SIZE_F GetSize() const;
		virtual void RequestRedraw() = 0;

		D2D1_MATRIX_3X2_F GetTransform() const;
		void SetTransform(const D2D1_MATRIX_3X2_F& New);
	};
}