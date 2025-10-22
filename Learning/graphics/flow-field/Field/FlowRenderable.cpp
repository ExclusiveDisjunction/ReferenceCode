#include "FlowRenderable.h"

FlowRenderable::FlowRenderable(FlowFieldGen* Parent, Renderer* Render, std::string Name) : Renderable(Render)
{
	if (!Parent || !Render)
		throw std::logic_error("The parent and the renderer must both be provided.");

	this->FlowParent = Parent;
	this->Name = Name;
}
FlowRenderable::~FlowRenderable()
{
	
}

Matrix& FlowRenderable::RequestMatrix()
{
	if (!FlowParent)
		throw std::exception();

	return FlowParent->AngleMat;
}
void FlowRenderable::ReturnMatrix()
{
	
}

std::vector<Particle>& FlowRenderable::RequestParticles()
{
	if (!FlowParent)
		throw std::exception();

	return FlowParent->Particles;
}
void FlowRenderable::ReturnParticles()
{
	
}

std::string FlowRenderable::GetName() const
{
	return Name;
}

void FlowRenderable::Detach(bool PopFromParent)
{
	if (!IsAttached())
		return;

	if (PopFromParent && FlowParent)
	{
		std::vector<FlowRenderable*>& Ren = FlowParent->Renderables;
		std::string Name = this->Name;
		auto Iter = std::find_if(
			Ren.begin(),
			Ren.end(),
			[Name](FlowRenderable*& Obj) -> bool
			{
				return Obj->GetName() == Name;
			}
		);

		if (Iter != Ren.end())
			Ren.erase(Iter);
	}

	Renderable::Detach(PopFromParent);
}