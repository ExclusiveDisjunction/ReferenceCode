#include "FlowRenderable.h"

PointsDispRenderable::PointsDispRenderable(FlowFieldGen* Target, Renderer* Render) : FlowRenderable(Target, Render, "PointsDispRenderable")
{

}
PointsDispRenderable::~PointsDispRenderable()
{

}

void PointsDispRenderable::OnRender(ID2D1RenderTarget* Target, D2D1_SIZE_F Size)
{
	if (!Target || !FlowParent)
		return;

	if (!DotColor || !VecColor)
	{
		bool Result = ConstructResources(Target);
		if (!Result)
			return;
	}

	std::vector<Particle>& Particles = RequestParticles();

	for (Particle& Part : Particles)
	{
		D2D1_POINT_2F Base = D2D1::Point2F((float)Part.getCurrentPosition().x, (float)Part.getCurrentPosition().y);
		
		float r = 0.17f;
		D2D1_ELLIPSE El = D2D1::Ellipse(Base, r, r);
		Target->FillEllipse(El, DotColor);
	}

	ReturnParticles();
}
bool PointsDispRenderable::ConstructResources(ID2D1RenderTarget* Target)
{
	if (!Target)
		return false;

	ReleaseResources();

	HRESULT hr1 = Target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &DotColor);
	HRESULT hr2 = Target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Orange), &VecColor);
	return SUCCEEDED(hr1) && SUCCEEDED(hr2);
}
void PointsDispRenderable::ReleaseResources()
{

}