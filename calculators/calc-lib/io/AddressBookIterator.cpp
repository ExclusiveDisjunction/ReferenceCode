#include "AddressBook.h"

namespace Mason::IO
{
	AddressBookIterator::AddressBookIterator(AddressBookRecord* Socket, ABRIType Behavior)
	{
		if (Behavior == Tranverse)
			Current = Socket;
		else if (Behavior == ParentNode)
		{
			Current = Socket->SFirst;
			Behavior = SiblingNode;
		}
		else if (Behavior == SiblingNode)
			Current = Socket;

		this->Behavior = Behavior;
	}

	AddressBookIterator::reference AddressBookIterator::operator*()
	{
		return Current;
	}
	AddressBookIterator::pointer AddressBookIterator::operator->()
	{
		return &Current;
	}

	AddressBookIterator& AddressBookIterator::operator++(int)
	{
		return operator++();
	}
	AddressBookIterator& AddressBookIterator::operator++()
	{
		if (Behavior == Tranverse)
		{
			if (Current)
				Current = Current->Next;
		}
		else
		{
			if (Current)
				Current = Current->SNext;
		}		
		
		return *this;
	}

	AddressBookIterator& AddressBookIterator::operator--(int)
	{
		return operator--();
	}
	AddressBookIterator& AddressBookIterator::operator--()
	{
		if (Behavior == Tranverse)
		{
			if (Current)
				Current = Current->Last;
		}
		else
		{
			if (Current)
				Current = Current->SPrevious;
		}

		return *this;
	}

	bool operator==(const AddressBookIterator& A, const AddressBookIterator& B)
	{
		return A.Current == B.Current;
	}
	bool operator!=(const AddressBookIterator& A, const AddressBookIterator& B)
	{
		return A.Current != B.Current;
	}
}