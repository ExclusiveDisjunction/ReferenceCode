#include "VectorB.h"
#include "Str.h"

namespace Core
{
	String VectorB::ToString() const
	{
		if (Size == 0)
			return L"-V(){}";

		String Name = (String)First->Value->TypeName();
		String Return = L"-V(" + Name + L"){";

		if (Size == 0)
			return (Return + L'}');


		Return += static_cast<BasicObject*>(First->Value)->ToString();
		for (seg Current = First->Next; Current != nullptr; Current = Current->Next)
			Return += L',' + Current->Value->ToString();

		Return += L"}";
		return Return;
	}
	String VectorB::ToUIString() const
	{
		if (Size == 0)
			return L"{ Collection is Empty. }";

		String Return = L"{ ";

		Return += First->Value->ToUIString();
		for (seg Current = First->Next; Current != nullptr; Current = Current->Next)
			Return += L", " + Current->Value->ToUIString();

		Return += L" }";
		return Return;
	}
	String VectorB::TypeName() const { return L"VECTOR"; }

	BasicObject* VectorB::DefaultValue() const { return new VectorB(); }
	BasicObject* VectorB::Clone() const 
	{
		VectorB* Return = new VectorB();
		for (seg Current = First; Current != nullptr; Current = Current->Next)
			Return->Add(Current->Value->Clone());

		return Return;
	}
	bool VectorB::OverrideFrom(BasicObject* Obj)
	{
		VectorB* Conv1 = dynamic_cast<VectorB*>(Obj);
		if (Conv1)
		{
			*this = *Conv1;
			return true;
		}

		String* Conv2 = dynamic_cast<String*>(Obj);

		if (Conv2)
			this->FillFromString(*Conv2);
		else
			return false;

		return true;
	}

	void VectorB::FillFromString(const String& Obj) 
	{
		if (!Obj.BeginsWith(L"-V("))
			return;

		String UObj = Obj; //Removes Const qualifer
		UObj.RemoveAt(0); //-
		UObj.RemoveAt(0); //V
		UObj.RemoveAt(0); //(

		unsigned i = 0;
		for (i = 0; i < UObj.Length(); i++)
			if (UObj[i] == L')')
				break;

		String TypeString = UObj.SubString(0, i - 1);
		if (TypeString == String())
			TypeString = L"STRING";

		if (UObj[UObj.Length() - 1] != L'}')
			return;

		String Splined = UObj.SubString(i + 2, UObj.Length() - 2);
		if (Splined == String())
			return;

		StringList List = Splined.Split(L',');

		//Assumes that BasicObject* is a pointer to a class that inherits from BasicObject.
		for (String Item : List)
		{
			BasicObject* Template = TypeRegistry::Retrive(TypeString);
			Template->FillFromString(Item);
			Add(Template);
		}
	}

	bool VectorB::HasModifyer() const
	{ 
		return true; 
	}
	BasicObjectModifyer* VectorB::ConstructModifyer() const
	{
		return nullptr;
	}
}