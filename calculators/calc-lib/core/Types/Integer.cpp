#include "BasicObject.h"

#include "..\Str.h"

namespace Core
{
	Integer::Integer() : Value(0) {}
	Integer::Integer(long long Value) : Value(Value) {}
	Integer::Integer(const Integer& Obj) : Value(Obj.Value) {}
	Integer::Integer(Integer&& Obj) noexcept : Value(Obj.Value) {}

	Integer& Integer::operator=(const Integer& Obj) noexcept { this->Value = Obj.Value; return *this; }
	Integer& Integer::operator=(Integer&& Obj) noexcept { this->Value = Obj.Value; return *this; }
	Integer& Integer::operator=(long long Value) noexcept { this->Value = Value; return *this; }

	String Integer::ToString() const { return String(Value); }
	String Integer::ToUIString() const { return String(Value); }
	String Integer::TypeName() const { return L"INTEGER"; }

	BasicObject* Integer::DefaultValue() const { return new Integer(); }
	bool Integer::OverrideFrom(BasicObject* Obj)
	{
		Integer* Conv1 = dynamic_cast<Integer*>(Obj);
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

	void Integer::FillFromString(const String& Obj) { *this = Obj.ToLong(); }

	bool Integer::HasModifyer() const { return false; }
	BasicObjectModifyer* Integer::ConstructModifyer() const { return nullptr; }
}