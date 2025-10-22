#include "FlowRenderable.h"

GridRenderable::GridRenderable(FlowFieldGen* Parent, Renderer* Render) : FlowRenderable(Parent, Render, "GridRenderable")
{

}
GridRenderable::~GridRenderable()
{

}

void GridRenderable::OnRender(ID2D1RenderTarget* Target, D2D1_SIZE_F Size)
{
	if (!Target || !FlowParent)
		return;

	if (!PointBrush || !LineColor)
	{
		bool Result = ConstructResources(Target);
		if (!Result)
			return;
	}

	Matrix& Mat = RequestMatrix();
	D2D1_SIZE_F Scale = FlowParent->GetScale(), Bounds = FlowParent->GetBounds();

	if ((int)Bounds.width != Mat.NumCols() || (int)Bounds.height != Mat.NumRows())
		return;

	float ArrowLength = 0.40f;

	Vector Base, Arrow;
	for (int i = 0; i < (int)Bounds.height; i++)
	{
		for (int j = 0; j < (int)Bounds.width; j++)
		{
			//First calculate, then transform

			Base = Vector(float(i), float(j));
			Arrow = Vector::FromAngle(Mat[i][j], ArrowLength) + Base;

			D2D1_POINT_2F WndBase = D2D1::Point2F((float)Base.x, (float)Base.y),
				WndArrow = D2D1::Point2F((float)Arrow.x, (float)Arrow.y);

			Target->DrawLine(WndBase, WndArrow, LineColor, 0.05f);

			float Margin = 0.045f;
			D2D1_RECT_F BaseRect = D2D1::RectF(Base.x - Margin, Base.y - Margin, Base.x + Margin, Base.y + Margin);

			Target->FillRectangle(BaseRect, PointBrush);
		}
	}

	ReturnMatrix();
}
bool GridRenderable::ConstructResources(ID2D1RenderTarget* Target)
{
	if (!Target)
		return false;

	ReleaseResources();

	HRESULT r1 = Target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Violet), &PointBrush);
	HRESULT r2 = Target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::LightBlue), &LineColor);

	return SUCCEEDED(r1) && SUCCEEDED(r2);
}
void GridRenderable::ReleaseResources()
{

}
