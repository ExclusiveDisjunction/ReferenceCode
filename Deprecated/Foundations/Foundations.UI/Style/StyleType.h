#pragma once

#include "../UICommon.h"
#include <string>

namespace Core::UI
{
	/// <summary>
	/// Represents a specific type used by the style and UI system.
	/// </summary>
	class CORE_API StyleType
	{
	public:
		virtual ~StyleType();

		virtual void Dump() = 0; //Cleans out data
		virtual std::string TypeName() const = 0;
		virtual std::string ToString() const = 0;
		virtual StyleType* Blank() const = 0;
		virtual StyleType* Clone() const = 0;
	};
}