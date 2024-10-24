#include "BasicObject.h"

#include "..\Str.h"

namespace Core
{
	Double::Double() : Value(0) {}
	Double::Double(double Value) : Value(Value) {}
	Double::Double(const Double& Obj) : Value(Obj.Value) {}
	Double::Double(Double&& Obj) noexcept : Value(Obj.Value) {}

	Double& Double::operator=(const Double& Obj) noexcept { this->Value = Obj.Value; return *this; }
	Double& Double::operator=(Double&& Obj) noexcept { this->Value = Obj.Value; return *this; }
	Double& Double::operator=(double Value) noexcept { this->Value = Value; return *this; }

	String Double::ToString() const { return String(Value); }
	String Double::ToUIString() const { return String(Value); }
	String Double::TypeName() const { return L"DOUBLE"; }

	BasicObject* Double::DefaultValue() const { return new Double(); }
	bool Double::OverrideFrom(BasicObject* Obj)
	{
		Double* Conv1 = dynamic_cast<Double*>(Obj);
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

	void Double::FillFromString(const String& Obj) { *this = Obj.ToDouble(); }

	bool Double::HasModifyer() const { return false; }
	BasicObjectModifyer* Double::ConstructModifyer() const { return nullptr; }
}