#include "StyleSchematic.h"

using namespace Core;
using namespace Core::UI;

StyleSchematicElement::StyleSchematicElement(const std::string& Name, StyleType* TypeSignature, StyleType* DefaultValue = nullptr)
{
	this->PropertyName = Name;

	if (!TypeSignature)
		throw std::logic_error("The type signature cannot be nullptr.");
	if (DefaultValue && DefaultValue->TypeName() != DefaultValue->TypeName())
		throw std::logic_error("When the default value is supplied, it must have the same type as the type signature.");

	this->Type = TypeSignature;
	this->Type->Dump();
	this->DefaultValue = DefaultValue == nullptr ? this->Type->Clone() : DefaultValue;
}
StyleSchematicElement::StyleSchematicElement(const StyleSchematicElement& Obj) noexcept
{
	this->PropertyName = Obj.PropertyName;
	this->Type = Obj.Type->Clone();
	this->DefaultValue = Obj.Type->Clone();
}
StyleSchematicElement::~StyleSchematicElement()
{
	if (this->Type)
	{
		delete this->Type;
		this->Type = nullptr;
	}
	if (this->DefaultValue)
	{
		delete this->DefaultValue;
		this->DefaultValue = nullptr;
	}
}

StyleSchematicElement& StyleSchematicElement::operator=(const StyleSchematicElement& Obj) noexcept
{
	if (this->Type)
		delete this->Type;
	if (this->DefaultValue)
		delete this->DefaultValue;

	this->PropertyName = Obj.PropertyName;
	this->Type = Obj.Type->Clone();
	this->DefaultValue = Obj.Type->Clone();
}

std::string StyleSchematicElement::GetName() const
{
	return PropertyName;
}
std::string StyleSchematicElement::TypeName() const
{
	return !Type ? L"NULL" : Type->TypeName(); 
}
std::string StyleSchematicElement::DefaultValueStr() const
{ 
	return !DefaultValue ? L"NULL" : DefaultValue->ToString(); 
}