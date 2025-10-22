#include "BasicObject.h"

#include "..\Str.h"

namespace Core
{
	Boolean::Boolean() : Value(0) {}
	Boolean::Boolean(bool Value) : Value(Value) {}
	Boolean::Boolean(const Boolean& Obj) : Value(Obj.Value) {}
	Boolean::Boolean(Boolean&& Obj) noexcept : Value(Obj.Value) {}

	Boolean& Boolean::operator=(const Boolean& Obj) noexcept { this->Value = Obj.Value; return *this; }
	Boolean& Boolean::operator=(Boolean&& Obj) noexcept { this->Value = Obj.Value; return *this; }
	Boolean& Boolean::operator=(bool Value) noexcept { this->Value = Value; return *this; }

	String Boolean::ToString() const { return Value ? L"TRUE" : L"FALSE"; }
	String Boolean::ToUIString() const { return Value ? L"True" : L"False"; }
	String Boolean::TypeName() const { return L"BOOLEAN"; }

	BasicObject* Boolean::DefaultValue() const { return new Boolean(); }
	bool Boolean::OverrideFrom(BasicObject* Obj)
	{
		Boolean* Conv1 = dynamic_cast<Boolean*>(Obj);
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

	void Boolean::FillFromString(const String& Obj) { Value = (Obj == L"TRUE"); }

	bool Boolean::HasModifyer() const { return false; }
	BasicObjectModifyer* Boolean::ConstructModifyer() const { return nullptr; }
}