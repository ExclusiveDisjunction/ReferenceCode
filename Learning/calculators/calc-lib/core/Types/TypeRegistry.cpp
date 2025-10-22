#include "TypeRegistry.h"

#include "..\Color.h"
#include "..\Str.h"
#include "..\VectorB.h"
#include "..\DateTime.h"
#include "..\MappingB.h"

namespace Core
{
	void TypeRegistry::Initiate()
	{
		AppendType(new Integer());
		AppendType(new Double());
		AppendType(new Boolean());
		AppendType(new Color());
		AppendType(new VectorB());
		AppendType(new DateTime());
		AppendType(new MappingB());
		AppendType(new String());
	}
	void TypeRegistry::ShutDown()
	{
		for (MappingPair<String, BasicObject*> Item : Objects)
		{
			delete Item.Value;
			Item.Value = nullptr;
		}

		Objects.Clear();
	}

	Mapping<String, BasicObject*> TypeRegistry::Objects;
	TypeRegistry::ErrorType TypeRegistry::LastError = ERR_None;

	bool TypeRegistry::AppendType(BasicObject* Sample)
	{
		if (!Sample)
		{
			LastError = ERR_TypeSignatureNull;
			return false;
		}
		else if (IsRegistered(Sample))
		{
			LastError = ERR_TypeNameNonUnique;
			return false;
		}

		Objects.Add(Sample->TypeName(), Sample);
		return true;
	}
	bool TypeRegistry::PopType(const String& Name)
	{
		unsigned i = 0;
		for (auto Item : Objects)
		{
			if (Item.Key == Name)
			{
				delete Item.Value;
				Item.Value = 0;
				Objects.RemoveAt(i);

				return true;
			}
		}

		LastError = ERR_TypeNameNotFound;
		return false;
	}

	bool TypeRegistry::IsRegistered(BasicObject* Signature)
	{
		if (!Signature)
			return false;

		return IsRegistered(Signature->TypeName());
	}
	bool TypeRegistry::IsRegistered(const String& TypeName)
	{
		for (auto Item : Objects)
		{
			if (Item.Key == TypeName)
				return true;
		}

		return false;
	}

	BasicObject* TypeRegistry::Retrive(const String& Name)
	{
		for (auto Item : Objects)
		{
			if (Item.Key == Name)
				return Item.Value->DefaultValue();
		}

		LastError = ERR_TypeNameNotFound;
		return nullptr;
	}

	BasicObject* TypeRegistry::ConstructFrom(const String& Literal)
	{
		BasicObject* Return = nullptr;
		
		//First, Basic Core Types
		{
			if (Literal == L"TRUE" || Literal == L"FALSE")
				return new Boolean(Literal == L"TRUE");

			long long FirstCase = Literal.ToLong();
			if (String(FirstCase) == Literal)
				return new Integer(FirstCase);

			double SecondCase = Literal.ToDouble();
			if (String(SecondCase) == Literal)
				return new Double(SecondCase);

			else if (Literal.BeginsWith(L"-V(")) //Vector
			{
				Return = new VectorB();
				Return->FillFromString(Literal);
				return Return;
			}

			else if (Literal.BeginsWith(L"-M(")) //Mapping
			{
				Return = new MappingB();
				Return->FillFromString(Literal);
				return Return;
			}
		}

		//Next, Test for AS statement
		{
			//Look for _AS_ statement.

			unsigned length = Literal.Length(), BeginIndex = length + 1;
			for (int i = static_cast<int>(length-1); i >= 0; i--)
			{
				if (Literal[i] == L' ')
				{
					BeginIndex = i;
					break;
				}
			}

			String AsLit = Literal.SubString(BeginIndex + 1, BeginIndex + 4);

			if (BeginIndex != (length + 1) && AsLit == L"_AS_")
			{
				String ValuePart = Literal.SubString(0, BeginIndex);
				String TypePart = Literal.SubString(BeginIndex + 5, length - 1);

				Return = Retrive(TypePart);
				Return->FillFromString(ValuePart);

				return Return;
			}

			//If not, it is simply treated as a string in the next clause. 
		}

		//Finally, if all else fails, then it returns a string.
		{
			return new String(Literal);
		}
	}
}