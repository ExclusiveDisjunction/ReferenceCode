#pragma once

// https://en.wikipedia.org/wiki/Perlin_noise

#include "..\Calc\Matrix.h"
#include "..\Calc\Vector.h"
#include "..\Calc\Particle.h"
#include "..\Calc\Perlin.h"
#include "..\Render\Renderer.h"

#define _USE_MATH_DEFINES

#include <iostream>
#include <math.h>
#include <vector>

class FlowFieldGen;
class FlowRenderable;

class FlowFieldGen
{
private:
	Matrix AngleMat;
	std::vector<Particle> Particles;
	std::vector<std::vector<D2D1_POINT_2F>> Calculations;
	Renderer* Render;

	std::vector<FlowRenderable*> Renderables;

	int XMax = 0, YMax = 0;
	int WndX = 0, WndY = 0;
	float ScaleX = 0, ScaleY = 0;

	float DetermineAngle(const Vector& Pos);

	void ConstructRenderables();
	void ResetRenderables();

	static void OnRendererResize(void* Data, D2D1_SIZE_U NewSize);

	Matrix& GetAngleMatrix() { return AngleMat; }
	std::vector<Particle>& GetParticles() { return Particles; }
	std::vector<std::vector<D2D1_POINT_2F>>& GetCalculations() { return Calculations; }
public:
	FlowFieldGen(Renderer* Render);
	FlowFieldGen(const FlowFieldGen& Obj) = delete;
	FlowFieldGen(FlowFieldGen&& Obj) = delete;
	~FlowFieldGen();

	friend FlowRenderable;

	D2D1_SIZE_F GetBounds() const { return D2D1::SizeF((float)XMax, (float)YMax); } //XMax, YMax
	D2D1_SIZE_F GetWndSize() const { return D2D1::SizeF((float)WndX, (float)WndY);} //WndX, WndY
	D2D1_SIZE_F GetScale() const { return D2D1::SizeF((float)ScaleX, (float)ScaleY); }//ScaleX, ScaleY

	void FillParticles(int ParticleCount);
	void DisplayParticles(bool Visible);
	void AnimateParticles();
	void DisplayCalculations(bool Visible);

	void FillGrid(int XMax, int YMax);
	void DisplayGrid(bool Visible);

	Vector MapToWindow(const Vector& ToTranslate);
	Vector MapFromWindow(const Vector& ToTranslate);

	void RequestRedraw()
	{
		if (Render)
			Render->RequestRedraw();
	}

	void ClearAll();
};