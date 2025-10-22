#pragma once

#include "..\UICommon.h"
#include "Mapping.h"
#include <vector>

#include "StyleType.h"

namespace Core::UI
{
	class CORE_API Style;

	class CORE_API StyleSchematicElement
	{
	private:
		std::string PropertyName;
		StyleType* Type = nullptr;
		StyleType* DefaultValue = nullptr;
	public:
		StyleSchematicElement(const std::string& Name, StyleType* TypeSignature, StyleType* DefaultValue = nullptr);
		StyleSchematicElement(const StyleSchematicElement& Obj) noexcept;
		~StyleSchematicElement();

		friend Style;

		StyleSchematicElement& operator=(const StyleSchematicElement& Obj) noexcept;

		std::string GetName() const;
		std::string TypeName() const;
		std::string DefaultValueStr() const;
	};
	class CORE_API StyleSchematic
	{
	private:
		std::vector<StyleSchematicElement*> Properties;
	public:
		StyleSchematic();
		StyleSchematic(const std::vector<StyleSchematicElement*>& Schemas);
		StyleSchematic(const StyleSchematic& Obj) noexcept;
		~StyleSchematic();

		friend Style;

		StyleSchematic& operator=(const StyleSchematic& Obj);

		StyleSchematicElement& operator[](const std::string& Name) const;
		
		void AppendProperty(StyleSchematicElement* Schematic);
		void AppendManyProperties(const std::vector<StyleSchematicElement*>& Schemas);
		bool RemoveProperty(std::string Name);
	};

	
}