#include "StyleSchematic.h"

using namespace Core;
using namespace Core::UI;

StyleSchematic::StyleSchematic()
{

}
StyleSchematic::StyleSchematic(const std::vector<StyleSchematicElement*>& Schemas)
{
	this->Properties = Schemas;

	//Scan for duplicates. If one is found, push it to a duplicates vector to be removed later.
	std::vector<StyleSchematicElement*> Duplicates;
	std::vector<StyleSchematicElement*>::iterator beg1, beg2, end1, end2;
	int i, j;
	for (beg1 = Properties.begin(), end1 = Properties.end(), i = 0; beg1 != end1; beg1++, i++)
	{
		for (beg2 = Properties.begin(), end2 = Properties.end(), j = 0; beg2 != end2; beg2++, j++)
		{
			if (i == j) //Same index
				continue;

			if ((*beg1)->GetName() == (*beg2)->GetName())
				Duplicates.push_back(*beg1);
		}
	}

	//Now that all duplicates are found, erase them.
	for (auto curr = Duplicates.begin(), end = Duplicates.end(); curr != end; curr++)
	{
		delete* curr;
		Properties.erase(curr);
	}
}
StyleSchematic::StyleSchematic(const StyleSchematic& Obj) noexcept
{
	//Copy over the properties from the other object by going trough each one and adding one to the current item.
	Properties.resize(Obj.Properties.size());
	for (StyleSchematicElement* ToClone : Obj.Properties)
		Properties.push_back(new StyleSchematicElement(*ToClone));
}
StyleSchematic::~StyleSchematic()
{
	for (StyleSchematicElement* Item : Properties)
		if (Item)
			delete Item;

	Properties.clear();
}

StyleSchematic& StyleSchematic::operator=(const StyleSchematic& Obj)
{
	if (Properties.size())
	{
		for (StyleSchematicElement* Item : Properties)
			if (Item)
				delete Item;
		Properties.clear();
	}

	//Copy over the properties from the other object by going trough each one and adding one to the current item.
	Properties.resize(Obj.Properties.size());
	for (StyleSchematicElement* ToClone : Obj.Properties)
		Properties.push_back(new StyleSchematicElement(*ToClone));
}
StyleSchematicElement& StyleSchematic::operator[](const std::string& Name) const
{
	for (StyleSchematicElement* Elem : Properties)
	{
		if (Elem->GetName() == Name)
			return *Elem;
	}

	//At this point, nothing was found.
	throw std::logic_error("The name was not found.");
}

void StyleSchematic::AppendProperty(StyleSchematicElement* Schematic)
{
	//First check to see if that property already exists. If it does, update the values.
	for (StyleSchematicElement* Item : Properties)
	{
		if (Item->GetName() == Schematic->GetName())
		{
			//Match found, override the current one.
			*Item = *Schematic;
			return;
		}
	}

	//Nothing matched, so add it into the properties.
	Properties.push_back(Schematic);
}
void StyleSchematic::AppendManyProperties(const std::vector<StyleSchematicElement*>& Schemas)
{
	//Must go through each item and then go through each of the incomming ones.
	for (StyleSchematicElement* InProp : Schemas)
	{
		bool CurrentWasFound = false;
		for (StyleSchematicElement* CurrProp : Properties)
		{
			if (InProp->GetName() == CurrProp->GetName())
			{
				//Match Found, override.
				*CurrProp = *InProp;
				CurrentWasFound = true;
			
				break; //Exists the loop because at this point, only one of each name can exist in properties.
			}
		}

		//If the element never matched, then it is safe to push it.
		if (!CurrentWasFound)
			Properties.push_back(InProp);
	}	
}
bool StyleSchematic::RemoveProperty(std::string Name)
{
	std::vector<StyleSchematicElement*>::iterator begin, end = Properties.end();
	for (begin = Properties.begin(); begin != end; begin++)
	{
		if ((*begin)->GetName() == Name) //Target found
		{
			delete* begin;
			Properties.erase(begin);
			return true;
		}
	}

	return false;
}