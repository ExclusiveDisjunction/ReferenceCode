#include "Reference.h"

#include "AddressBook.h"

namespace Mason::IO
{
	Reference::Reference(AddressBookRecord* Record)
	{
		if (!Record)
		{
			_IsValid = false;
			return;
		}

		_Record = Record;
		_Target = !Record->Handle ? nullptr : dynamic_cast<Component*>(Record->Handle);
		_IsValid = _Target != nullptr;
	}
	Reference::Reference(Component* Obj)
	{
		if (!Obj)
		{
			_IsValid = false;
			return;
		}

		_Record = Obj->Record;
		_Target = Obj;
		_IsValid = true;
	}
	Reference::~Reference()
	{
		if (_Record)
			_Record->Ref = nullptr;
	}

	void Reference::DecrementSelf()
	{
		if (_RefCount == 0)
		{
			delete this;
			return;
		}

		_RefCount--;
		if (_RefCount == 0)
			delete this;
	}
	void Reference::IncrementSelf()
	{
		_RefCount++;
	}

	Reference::operator Component* () const
	{
		if (!_IsValid)
			return nullptr;
		
		return _Target;
	}
	Component* Reference::Evaluate() const
	{
		return operator Component * ();
	}

	bool operator==(const Reference& A, const Reference& B)
	{
		return A._Target == B._Target &&
			A._IsValid == B._IsValid &&
			A._Record == B._Record;
	}
	bool operator!=(const Reference& A, const Reference& B)
	{
		return !(A == B);
	}
}