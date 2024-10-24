#pragma once

#include "RenderBase.h"

class Renderer;
class Renderable;
class RenderWindow;

class Renderer
{
public:
	Renderer(RenderWindow* Target);
	Renderer(const Renderer& Obj) = delete;
	Renderer(Renderer&& Obj) = delete;
	~Renderer();

	Renderer& operator=(const Renderer& Obj) = delete;
	Renderer& operator=(Renderer&& Obj) = delete;

	void Init();
	D2D1_SIZE_F GetSize() const;
	void RequestRedraw();

	void RegisterOnSizeEvent(void* Obj, std::function<void(void*, D2D1_SIZE_U)> Callable);
	void DeRegisterOnSizeEvent(void* Obj);

	D2D1_MATRIX_3X2_F GetTransform() const;
	void SetTransform(const D2D1_MATRIX_3X2_F& New);

	ID2D1Factory* GetFactory() { return Factory; }

	friend Renderable;
	friend RenderWindow;

private:
	RenderWindow* Target = nullptr;
	ID2D1Factory* Factory = nullptr;
	ID2D1HwndRenderTarget* RenderTarget = nullptr;
	D2D1_MATRIX_3X2_F Transform = D2D1::Matrix3x2F::Identity();

	std::vector<Renderable*> Renderables;
	std::map<void*, std::function<void(void*, D2D1_SIZE_U)>> OnSizeEvents;

	HRESULT CreateDevIndpResx();
	HRESULT CreateDevDepResx();
	void DiscardDevResx();

	HRESULT OnRender();
	void OnResize(UINT Width, UINT Height);
};