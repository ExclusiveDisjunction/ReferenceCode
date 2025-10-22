#include "Renderable.h"

#include <vector>

namespace Core::Render
{
	Renderable::Renderable(Renderer* Target, bool Enabled)
	{
		if (!Target)
			throw std::exception();

		this->Target = Target;
		this->Enabled = Enabled;
		this->Attached = true;

		this->Target->Renderables.push_back(this);
	}
	Renderable::~Renderable()
	{
		if (Attached)
			Detach();
	}

	void Renderable::Detach(bool PopFromParent)
	{
		ReleaseResources();

		if (Target)
		{
			if (PopFromParent)
			{
				auto Iter = std::find(Target->Renderables.begin(), Target->Renderables.end(), this);
				if (Iter != Target->Renderables.end())
					Target->Renderables.erase(Iter);
			}

			Target = nullptr;
		}

		Attached = false;
	}

	bool Renderable::IsEnabled() const
	{
		return Enabled;
	}
	void Renderable::IsEnabled(bool New)
	{
		this->Enabled = New;
	}
	void Renderable::ToggleEnabled()
	{
		Enabled = !Enabled;
	}

	bool Renderable::IsAttached() const
	{
		return Attached;
	}
}