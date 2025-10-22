#include "StyleSheet.h"

using namespace Core;
using namespace Core::UI;

StyleSheet::StyleSheet() 
{

}
StyleSheet::StyleSheet(const std::map<std::string, StyleType*>& InValues)
{
	this->Values = InValues;
}
StyleSheet::StyleSheet(const StyleSheet& Obj) noexcept
{
	for (const std::pair<const std::string, StyleType*>& Val : Obj.Values)
		Values.emplace(std::make_pair(Val.first, Val.second ? Val.second->Clone() : nullptr)); //Recall that types cannot be simply copied.
}
StyleSheet::~StyleSheet()
{
	for (std::pair<const std::string, StyleType*>& Val : Values)
	{
		if (Val.second)
			delete Val.second;
		Val.second = nullptr;
	}
}

StyleSheet& StyleSheet::operator=(const StyleSheet& Obj)
{
	if (Values.size()) // Empty first
	{
		for (std::pair<const std::string, StyleType*>& Val : Values)
		{
			if (Val.second)
				delete Val.second;

			Val.second = nullptr;
		}
		Values.clear();
	}

	for (const std::pair<const std::string, StyleType*>& OtherVal : Obj.Values)
		Values.emplace(std::make_pair(OtherVal.first, OtherVal.second ? OtherVal.second->Clone() : nullptr));

	return *this;
}
StyleSheet& StyleSheet::operator=(const std::map<std::string, StyleType*>& Obj)
{
	if (Values.size()) // Empty first
	{
		for (std::pair<const std::string, StyleType*>& Val : Values)
		{
			if (Val.second)
				delete Val.second;

			Val.second = nullptr;
		}
		Values.clear();
	}

	for (const std::pair<const std::string, StyleType*>& OtherVal : Obj)
		Values.emplace(std::make_pair(OtherVal.first, OtherVal.second ? OtherVal.second->Clone() : nullptr));

	return *this;
}
StyleType& StyleSheet::operator[](const std::string& Name) const
{
	auto Target = Values.find(Name);
	if (Target == Values.end())
		throw std::logic_error("That property name was not found in the styles sheet.");

	return *(*Target).second;
}

void StyleSheet::AppendProperty(const std::string& Name, StyleType* Value)
{
	auto Result = Values.emplace(Name, Value);
	if (!Result.second) //The emplace function's return pair contains a bool that specifies if it actually happened. In this instance, we will update. 
	{
		//Grab the iterator
		auto Iterator = Result.first;
		if (Iterator == Values.end()) //Error!
			throw std::logic_error("The emplace returned the end index.");

		*(*Iterator).second = *Value; //Update the value at second.
	}
}
bool StyleSheet::RemoveProperty(const std::string& Name)
{
	return Values.erase(Name) == 1;
}