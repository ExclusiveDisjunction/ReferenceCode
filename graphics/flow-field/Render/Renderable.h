#pragma once

#include "Renderer.h"

class Renderable
{
private:
	Renderer* Target = nullptr;
	bool Enabled = true;
	bool Attached = false;

public:
	Renderable(Renderer* Target, bool Enabled = true);
	Renderable(const Renderable& Obj) = delete;
	Renderable(Renderable&& Obj) = delete;
	~Renderable();

	Renderable& operator=(const Renderable& Obj) = delete;
	Renderable& operator=(Renderable&& Obj) = delete;

	virtual void OnRender(ID2D1RenderTarget* Target, D2D1_SIZE_F Size) = 0;
	virtual bool ConstructResources(ID2D1RenderTarget* Target) = 0;
	virtual void ReleaseResources() = 0;
	virtual void Detach(bool PopFromParent = true);

	bool IsEnabled() const;
	void IsEnabled(bool New);
	void ToggleEnabled();

	bool IsAttached() const;
};