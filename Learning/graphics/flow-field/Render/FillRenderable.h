#pragma once

#include "Renderable.h"

class FillRenderable : public Renderable
{
private:
	COLORREF Color;

	ID2D1SolidColorBrush* Brush = nullptr;
public:
	FillRenderable(Renderer* Target, COLORREF Ref = 0x000000);

	void OnRender(ID2D1RenderTarget* Target, D2D1_SIZE_F Size) override;
	bool ConstructResources(ID2D1RenderTarget* Target) override;
	void ReleaseResources() override;
};