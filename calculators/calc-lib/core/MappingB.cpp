#include "MappingB.h"

#include "Str.h"
#include "VectorB.h"

namespace Core
{
	String MappingB::ToString() const
	{
		if (Size == 0)
			return L"-M(){}";

		String KeyName = (String)First->Key->TypeName(), ValueName = (String)First->Value->TypeName();
		String Return = L"-M(" + KeyName + L',' + ValueName + L"){";

		Return += First->Key->ToString() + L',' + First->Value->ToString();
		for (seg Current = First->Next; Current != nullptr; Current = Current->Next)
			Return += L',' + Current->Key->ToString() + L',' + Current->Value->ToString();

		Return += L"}";
		return Return;
	}
	String MappingB::ToUIString() const
	{
		if (Size == 0)
			return L"{ Collection is Empty. }";

		String Return = L"{ ";

		Return += First->Key->ToUIString() + L" : " + First->Value->ToUIString();
		for (seg Current = First->Next; Current != nullptr; Current = Current->Next)
			Return += L", " + Current->Key->ToUIString() + L" : " + Current->Value->ToUIString();

		Return += L" }";
		return Return;
	}
	String MappingB::TypeName() const { return L"MAPPING"; }

	BasicObject* MappingB::DefaultValue() const { return new MappingB(); }
	BasicObject* MappingB::Clone() const
	{
		MappingB* Return = new MappingB();
		for (seg Current = First; Current != nullptr; Current = Current->Next)
		{
			BasicObject* NewK = Current->Key->Clone(), * NewV = Current->Value->Clone();
			Return->Add(NewK, NewV);
		}

		return Return;
	}
	bool MappingB::OverrideFrom(BasicObject* Obj)
	{
		MappingB* FirstConv = dynamic_cast<MappingB*>(Obj);
		if (FirstConv)
		{
			Clear();
			MappingB* Cloned = dynamic_cast<MappingB*>(FirstConv->Clone());
			if (Cloned)
			{
				First = Cloned->First;
				Last = Cloned->Last;
				_Size = Cloned->Size;

				Cloned->First = Cloned->Last = nullptr;
				Cloned->_Size = 0;
				delete Cloned;
				return true;
			}
			return false;
		}

		String* SecondConv = dynamic_cast<String*>(Obj);
		if (SecondConv)
		{
			this->FillFromString(*SecondConv);
			return true;
		}

		return false;
	}

	void MappingB::FillFromString(const String& Obj)
	{
		if (!Obj.BeginsWith(L"-M("))
			return;

		String UObj = Obj;
		UObj.RemoveAt(0); //-
		UObj.RemoveAt(0); //M
		UObj.RemoveAt(0); //(

		unsigned i = 0;
		for (i = 0; i < UObj.Length(); i++)
			if (UObj[i] == L')')
				break;

		String TypeStringRaw = UObj.SubString(0, i - 1);
		StringList TypeStrings;
		if (TypeStringRaw == String())
			TypeStrings = { L"STRING", L"STRING" };
		else
			TypeStrings = TypeStringRaw.Split(',');

		if (TypeStrings.Size == 1)
			TypeStrings.Add(L"STRING");
		if (TypeStrings[0] == String())
			TypeStrings[0] = L"STRING";
		if (TypeStrings[1] == String())
			TypeStrings[1] = L"STRING";

		if (UObj[UObj.Length() - 1] != L'}')
			return;

		String Splined = UObj.SubString(i + 2, UObj.Length() - 2);
		if (Splined == String())
			return;
		StringList List = Splined.Split(L',');

		VectorIterator<String> Iterator = List.begin(), End = List.end();
		i = 0;
		MappingPair<BasicObject*, BasicObject*> Temp;
		for (i = 0; Iterator != End; i++, Iterator++)
		{
			if (i % 2) //Odd, value item
			{
				Temp.Value = TypeRegistry::Retrive(TypeStrings[1]);
				Temp.Value->FillFromString(*Iterator);
				Add(Temp);
			}
			else //Even, key item
			{
				Temp.Key = TypeRegistry::Retrive(TypeStrings[0]);
				Temp.Key->FillFromString(*Iterator);
			}
		}

	}

	bool MappingB::HasModifyer() const { return true; }
	BasicObjectModifyer* MappingB::ConstructModifyer() const { return nullptr; }
}