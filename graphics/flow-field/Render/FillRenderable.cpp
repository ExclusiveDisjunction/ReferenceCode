#include "FillRenderable.h"

FillRenderable::FillRenderable(Renderer* Target, COLORREF Ref) : Renderable(Target)
{
	this->Color = Ref;
	this->Brush = nullptr;
}

void FillRenderable::OnRender(ID2D1RenderTarget* Target, D2D1_SIZE_F Size)
{
	if (!Brush)
		ConstructResources(Target);

	D2D1_RECT_F FillArea = D2D1_RECT_F(0, 0, Size.width, Size.height);

	Target->FillRectangle(FillArea, Brush);
}
bool FillRenderable::ConstructResources(ID2D1RenderTarget* Target)
{
	if (Brush)
		SafeRelease(&Brush);

	float red = (float)GetRValue(Color) / 255.0f,
		green = (float)GetGValue(Color) / 255.0f,
		blue = (float)GetBValue(Color) / 255.0f;

	HRESULT hr = Target->CreateSolidColorBrush(D2D1::ColorF(red, green, blue), &Brush);

	return SUCCEEDED(hr);
}
void FillRenderable::ReleaseResources()
{
	SafeRelease(&Brush);
}