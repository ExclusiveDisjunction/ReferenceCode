#pragma once

#include "FlowFieldGen.h"
#include "..\Render\Renderable.h"

class FlowRenderable : public Renderable
{
protected:
	FlowFieldGen* FlowParent = nullptr;
	std::string Name;

	Matrix& RequestMatrix();
	void ReturnMatrix();

	std::vector<Particle>& RequestParticles();
	void ReturnParticles();

	Matrix& GetAngleMatrix() 
	{ 
		if (!FlowParent)
			throw std::exception();
		return FlowParent->GetAngleMatrix(); 
	}
	std::vector<Particle>& GetParticles()
	{
		if (!FlowParent)
			throw std::exception();

		return FlowParent->GetParticles();
	}
	std::vector<std::vector<D2D1_POINT_2F>>& GetCalculations()
	{
		if (!FlowParent)
			throw std::exception();

		return FlowParent->GetCalculations();
	}

public:
	FlowRenderable(FlowFieldGen* Parent, Renderer* Render, std::string Name);
	FlowRenderable(const FlowRenderable& Obj) = delete;
	FlowRenderable(FlowRenderable&& Obj) = delete;
	~FlowRenderable();

	FlowRenderable& operator=(const FlowRenderable& Obj) = delete;
	FlowRenderable& operator=(FlowRenderable&& Obj) = delete;

	std::string GetName() const;

	void Detach(bool PopFromParent) override;
};

class GridRenderable : public FlowRenderable
{
private:
	ID2D1SolidColorBrush* PointBrush = nullptr;
	ID2D1SolidColorBrush* LineColor = nullptr;
public:
	GridRenderable(FlowFieldGen* Parent, Renderer* Render);
	GridRenderable(const GridRenderable& Obj) = delete;
	GridRenderable(GridRenderable&& Obj) = delete;
	~GridRenderable();

	GridRenderable& operator=(const GridRenderable& Obj) = delete;
	GridRenderable& operator=(GridRenderable&& Obj) = delete;

	void OnRender(ID2D1RenderTarget* Target, D2D1_SIZE_F Size) override;
	bool ConstructResources(ID2D1RenderTarget* Target) override;
	void ReleaseResources() override;
};
class PointsDispRenderable : public FlowRenderable
{
private:
	ID2D1SolidColorBrush* DotColor;
	ID2D1SolidColorBrush* VecColor;
public:
	PointsDispRenderable(FlowFieldGen* Parent, Renderer* Render);
	PointsDispRenderable(const PointsDispRenderable& Obj) = delete;
	PointsDispRenderable(PointsDispRenderable&& Obj) = delete;
	~PointsDispRenderable();

	PointsDispRenderable& operator=(const PointsDispRenderable& Obj) = delete;
	PointsDispRenderable& operator=(PointsDispRenderable&& Obj) = delete;

	void OnRender(ID2D1RenderTarget* Target, D2D1_SIZE_F Size) override;
	bool ConstructResources(ID2D1RenderTarget* Target) override;
	void ReleaseResources() override;
};
class FieldRenderable : public FlowRenderable
{
private:
	std::vector<ID2D1SolidColorBrush*> LineColors;
public:
	FieldRenderable(FlowFieldGen* Parent, Renderer* Render);
	FieldRenderable(const FieldRenderable& Obj) = delete;
	FieldRenderable(FieldRenderable&& Obj) = delete;
	~FieldRenderable();

	FieldRenderable& operator=(const FieldRenderable& Obj) = delete;
	FieldRenderable& operator=(FieldRenderable&& Obj) = delete;

	void OnRender(ID2D1RenderTarget* Target, D2D1_SIZE_F Size) override;
	bool ConstructResources(ID2D1RenderTarget* Target) override;
	void ReleaseResources() override;
};