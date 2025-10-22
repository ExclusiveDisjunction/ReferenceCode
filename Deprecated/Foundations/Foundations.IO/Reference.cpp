#include "Reference.h"

namespace Core::IO
{
	Reference::Reference(ReferenceCore* Target)
	{
		TargetCore = Target;
		if (TargetCore)
			TargetCore->Increment();
	}
	Reference::Reference(const Reference& Obj) noexcept
	{
		TargetCore = Obj.TargetCore;
		if (TargetCore)
			TargetCore->Increment();
	}
	Reference::Reference(Reference&& Obj) noexcept
	{
		TargetCore = std::exchange(Obj.TargetCore, nullptr);
	}
	Reference::~Reference() noexcept
	{
		if (TargetCore)
			TargetCore->Decrement();
	}

	Reference& Reference::operator=(const Reference& Obj) noexcept
	{
		if (TargetCore)
			TargetCore->Decrement();

		TargetCore = Obj.TargetCore;
		if (TargetCore)
			TargetCore->Increment();

		return *this;
	}
	Reference& Reference::operator=(Reference&& Obj) noexcept
	{
		if (TargetCore)
			TargetCore->Decrement();

		TargetCore = std::exchange(Obj.TargetCore, nullptr);

		return *this;
	}

	Element& Reference::Retrive() const
	{
		if (!IsValid())
			throw std::logic_error("ERROR: Element cannot be accessed.");

		return *(TargetCore->Target);
	}
	bool Reference::IsValid() const noexcept
	{
		return TargetCore && TargetCore->ElementIsValid && TargetCore->Target;
	}

	Element& Reference::operator*() const
	{
		try
		{
			return Retrive();
		}
		catch (std::logic_error& e)
		{
			throw e;
		}
	}
	Element* Reference::operator->() const
	{
		if (!IsValid())
			throw std::logic_error("ERROR: Element cannot be accessed.");

		return TargetCore->Target;
	}
}