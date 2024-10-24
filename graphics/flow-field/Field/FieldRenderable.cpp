#include "FlowRenderable.h"

FieldRenderable::FieldRenderable(FlowFieldGen* Parent, Renderer* Render) : FlowRenderable(Parent, Render, "FieldRenderable")
{

}
FieldRenderable::~FieldRenderable()
{

}

void FieldRenderable::OnRender(ID2D1RenderTarget* Target, D2D1_SIZE_F Size)
{
	if (!Target || !FlowParent)
		return;

	std::vector<std::vector<D2D1_POINT_2F>>& Lines = GetCalculations();
	if (!LineColors.size() || Lines.size() != LineColors.size())
	{
		bool Result = ConstructResources(Target);
		if (!Result)
			return;
	}

	auto LineI = Lines.begin(), LineEnd = Lines.end();
	auto ColorI = LineColors.begin(), ColorEnd = LineColors.end();
	for (; LineI != LineEnd && ColorI != ColorEnd; LineI++, ColorI++)
	{
		std::vector<D2D1_POINT_2F>& Line = *LineI;
		ID2D1SolidColorBrush*& Color = *ColorI;

		if (Line.size() < 2)
			continue; //The drawer requires a two element array minimum.

		auto beg = Line.begin();
		auto end = Line.end();
		
		for (; beg != end && (beg + 1) != end; beg++)
		{
			D2D1_POINT_2F& Curr = *beg, & Next = *(beg + 1);
			Target->DrawLine(Curr, Next, Color, 0.1f);
		}
	}
}
bool FieldRenderable::ConstructResources(ID2D1RenderTarget* Target)
{
	if (!Target || !FlowParent)
		return false;

	ReleaseResources();

	if (GetParticles().size() != GetCalculations().size())
		return false;

	std::vector<Particle>& Particles = GetParticles();
	LineColors.resize(Particles.size());
	auto ColorI = LineColors.begin(), ColorEnd = LineColors.end();

	for (Particle& Part : Particles)
	{
		HRESULT hr = Target->CreateSolidColorBrush(Part.getColor(), &(*ColorI));
		if (FAILED(hr))
			return false;

		ColorI++;
		if (ColorI == ColorEnd)
			break;
	}

	return true;
}
void FieldRenderable::ReleaseResources()
{
	for (ID2D1SolidColorBrush*& Brush : LineColors)
		SafeRelease(&Brush);

	LineColors.clear();
}