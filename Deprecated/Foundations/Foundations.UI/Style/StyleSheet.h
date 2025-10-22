#pragma once

#include "StyleType.h"
#include <string>
#include <map>

namespace Core::UI
{
	class CORE_API Style;

	/*
	* NOTE: In the STYLESHEET class ONLY, the values are allowed to be nullptr. This is only the case BECAUSE when merged with the StyleSchematic, the style class will take the DEFAULT value of StyleSchematic for that name.
	*/

	class CORE_API StyleSheet
	{
	private:
		std::map<std::string, StyleType*> Values;
	public:
		StyleSheet();
		StyleSheet(const std::map<std::string, StyleType*>& Values);
		StyleSheet(const StyleSheet& Obj) noexcept;
		~StyleSheet();

		friend Style;

		StyleSheet& operator=(const StyleSheet& Obj);
		StyleSheet& operator=(const std::map<std::string, StyleType*>& NewValues);
		StyleType& operator[](const std::string& Name) const;

		void AppendProperty(const std::string& Name, StyleType* Value = nullptr);
		bool RemoveProperty(const std::string& Name);
	};
}