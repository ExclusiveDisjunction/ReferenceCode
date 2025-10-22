#pragma once

#include "StyleType.h"
#include "StyleSchematic.h"
#include "StyleSheet.h"

#include <string>
#include <map>

namespace Core::UI
{
	/// <summary>
	/// Represents the final union of a stylesheet and a styleschematic. Call Style::Join to preform the union.
	/// </summary>
	class CORE_API Style
	{
	private:
		std::map<std::string, StyleType*> Data;
	public:
		Style() noexcept;
		Style(const Style& Obj) noexcept;
		~Style();

		/// <summary>
		/// Joins the StyleSheet and the StyleSchematic. If something is in the StyleSheet but not in the StyleSchematic, it is ignored. If something is in the StyleSchematic, but not in the StyleSheet, the default value is used.
		/// </summary>
		/// <param name="UserDef">The style sheet.</param>
		/// <param name="Schema">The style schematic.</param>
		/// <returns></returns>
		static Style Join(const StyleSheet& UserDef, const StyleSchematic& Schema);

		Style& operator=(const Style& Obj) noexcept;
		const StyleType const& operator[](const std::string& Name) const noexcept;
	};
}