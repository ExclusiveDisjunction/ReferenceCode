#include "Reference.h"

#include "Element.h"

namespace Core::IO
{
	ReferenceCore::ReferenceCore(Element* Target) noexcept
	{
		Bind(Target);
	}
	ReferenceCore::~ReferenceCore() noexcept
	{
		Unbind();
	}

	void ReferenceCore::Increment() noexcept
	{
		RefCount++;
	}
	void ReferenceCore::Decrement() noexcept
	{
		RefCount--;

		if (RefCount <= 0) //No longer needed.
			delete this;
	}

	void ReferenceCore::Bind(Element* New) noexcept
	{
		if (Target)
			Unbind();

		if (New)
		{
			if (New->RefCore)
				New->RefCore->Unbind();

			Target = New;
			Target->RefCore = this;
			ElementIsValid = true;
		}
		else
		{
			Target = nullptr;
			ElementIsValid = false;
		}

		RefCount = 0;
	}
	void ReferenceCore::Unbind() noexcept
	{
		if (!Target)
			return;

		Target->RefCore = nullptr;
		Target = nullptr;
		RefCount = 0;
		ElementIsValid = false;
	}
}