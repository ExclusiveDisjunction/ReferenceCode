#include "AddressBook.h"

namespace Mason::IO
{
	AddressBookRecord::AddressBookRecord(AddressBook* Parent, Address ID)
	{
		_Parent = Parent;
		Next = Last = nullptr;

		this->ID = ID;
	}
	AddressBookRecord::AddressBookRecord(AddressBook* Parent, Address ID, Sector* BelongsTo, std::streampos Begin, std::streampos End, Component* Socket, Reference* Reference)
	{
		_Parent = Parent;
		Next = Last = nullptr;

		this->ID = ID;
		this->Begin = Begin;
		this->End = End;
		this->Handle = Socket;
		this->Ref = Reference;
		this->BelongsTo = BelongsTo;
	}
	AddressBookRecord::~AddressBookRecord()
	{
		if (_Parent != nullptr)
			_Parent->Pop(this);

		if (SParent)
		{
			if (this == SParent->SFirst)
				SParent->SFirst = this->SNext;
			if (this == SParent->SLast)
				SParent->SLast = this->SPrevious;

			AddressBookRecord* Next = SNext, * Last = SPrevious;
			if (Next)
				Next->SPrevious = Last;
			if (Last)
				Last->SNext = Next;

			SParent = nullptr;
		}

		delete Handle;
		Handle = nullptr;

		delete Ref;
		Ref = nullptr;

		Begin = End = std::streampos();
		ID = Address();
	}
}