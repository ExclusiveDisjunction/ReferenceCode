#include "Style.h"

using namespace Core;
using namespace Core::UI;

Style::Style() noexcept
{

}
Style::Style(const Style& Obj) noexcept
{
	for (const std::pair<const std::string, StyleType*>& OtherVal : Obj.Data)
		Data.emplace(std::make_pair(OtherVal.first, OtherVal.second->Clone()));
}
Style::~Style()
{
	for (std::pair<const std::string, StyleType*>& Val : Data)
	{
		if (Val.second)
			delete Val.second;
		Val.second = nullptr;
	}
	
	Data.clear();
}

Style Style::Join(const StyleSheet& UserDef, const StyleSchematic& Schema)
{
	Style Return;
	//First, we have to fill the new style with all the default values of the schema. This ensures that all values are futfilled by the schema and will show up in the union.
	for (auto SchemaElem : Schema.Properties)
	{
		StyleType* DefVal = SchemaElem->DefaultValue ? SchemaElem->DefaultValue->Clone() : nullptr;
		if (!DefVal)
			throw std::logic_error("The schema cannot be processed if it has a nullptr for the type.");

		Return.Data.emplace(std::make_pair(SchemaElem->GetName(), DefVal));
	}

	//Now that the data has been filled, update the information.
	for (auto UserElem : UserDef.Values)
	{
		const std::string& Name = UserElem.first;
		StyleType* Value = UserElem.second;

		if (Name == "")
			throw std::logic_error("The name provided is empty, and this is not allowed.");
		if (!Value)
			throw std::logic_error("The value provided in the style sheet is nullptr, and this is not allowed.");

		auto Result = Return.Data.find(Name);
		if (Result != Return.Data.end()) //Update the value.
		{
			StyleType* ReturnedPtr = (*Result).second;
			//Determine that the type values match.
			if (Value->TypeName() != ReturnedPtr->TypeName())
				throw std::logic_error("The types \"" + Value->TypeName() + "\" and \"" + ReturnedPtr->TypeName() + "\" are not compatable.");

			//Once checking is complete, override the data.
			*ReturnedPtr = *Value;
		}
	}

	return Return;
}

Style& Style::operator=(const Style& Obj) noexcept
{
	if (Data.size())
	{
		for (std::pair<const std::string, StyleType*>& Val : Data)
		{
			if (Val.second)
				delete Val.second;
			Val.second = nullptr;
		}
		
		Data.clear();
	}

	for (const std::pair<const std::string, StyleType*>& OtherVal : Obj.Data)
		Data.emplace(std::make_pair(OtherVal.first, OtherVal.second->Clone()));

	return *this;
}
const StyleType const& Style::operator[](const std::string& Name) const noexcept
{
	auto Result = Data.find(Name);
	if (Result == Data.end())
		throw std::logic_error("The name was not found!");

	return *(*Result).second;
}
