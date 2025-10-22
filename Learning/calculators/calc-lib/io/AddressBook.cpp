#include "AddressBook.h"

#include "FileInstance.h"

namespace Mason::IO
{
	AddressBook::AddressBook(FileInstance* File)
	{
		_Parent = File;
	}
	AddressBook::~AddressBook()
	{
		if (_Parent)
			_Parent->Book = nullptr;
		
		Clear();
		_Parent = nullptr;
	}

	AddressBookRecord* AddressBook::operator[](unsigned int Index) const
	{
		if (Index >= Size)
			return nullptr;

		if (Index == 0 && Size >= 1)
			return First;
		else if (Index == (Size - 1) && Size >= 1)
			return Last;

		AddressBookRecord* Current = First;
		for (unsigned int i = 0; i < Index; i++)
			Current = Current->Next;

		return Current;
	}
	AddressBookRecord* AddressBook::ItemAt(unsigned int Index) const
	{
		return operator[](Index);
	}

	AddressBookRecord* AddressBook::Append(unsigned int Type, Sector* BelongsTo, std::streampos Begin, std::streampos End, Component* Sock, Reference * Ref)
	{
		Address ID = _Parent->ConstructAddress(Type);
		AddressBookRecord* New = new AddressBookRecord(this, ID, BelongsTo, Begin, End, Sock, Ref);

		if (Size == 0)
		{
			First = Last = New;
			_Size++;
			return First;
		}
		else if (Size == 1)
		{
			Last = New;
			First->Next = Last;
			Last->Last = First;

			_Size++;
			return Last;
		}

		AddressBookRecord* Prev = Last;

		New->Last = Prev;
		Last->Next = New;

		_Size++;
		Last = New;
		return New;
	}
	bool AddressBook::Pop(AddressBookRecord* const& Obj)
	{
		if (Size == 0)
			return false;
		else if (Size == 1 && Obj == First)
			First = Last = nullptr;
		else if (Size == 2 && (Obj == First || Obj == Last))
		{
			if (Obj == First)
				First = Last;
			else if (Obj == Last)
				Last = First;
			else
				return false;
		}
		else if (Obj == First)
			First = Obj->Next;
		else if (Obj == Last)
			Last = Obj->Last;
		else
		{
			AddressBookRecord* Next = Obj->Next;
			AddressBookRecord* Last = Obj->Last;

			if (Last)
				Last->Next = Next;
			Next->Last = Last;

		}

		Obj->Next = Obj->Last = nullptr;
		Obj->_Parent = nullptr;
		
		_Size--;
		return true;
	}
	bool AddressBook::Remove(AddressBookRecord* const& Obj)
	{
		bool Temp = Pop(Obj);

		delete Obj;
		return Temp;
	}
	bool AddressBook::Remove(unsigned int Index)
	{
		AddressBookRecord* Item = operator[](Index);
		if (!Item)
			return false;

		bool Temp = Pop(Item);
		
		delete Item;
		return Temp;
	}
	void AddressBook::Clear()
	{
		if (Size == 0)
			return;
		else if (Size == 1)
		{
			delete First;
			First = Last = nullptr;
		}
		else if (Size == 2)
		{
			delete First;
			delete Last;
			First = Last = nullptr;
		}
		else
		{
			AddressBookRecord* Current = nullptr;
			for (Current = First; Current != nullptr; Current = Current)
			{
				AddressBookRecord* Next = Current->Next;
				delete Current;
				Current = Next;
			}
		}

		_Size = 0;
	}

	unsigned int AddressBook::IndexOf(const AddressBookRecord*& Obj) const
	{
		unsigned int Return = 0;

		for (AddressBookRecord* Current = First; Current != nullptr; Current = Current->Next, Return++)
		{
			if (Current == Obj)
				return Return;
		}

		return UINT_MAX;
	}

	AddressBookIterator AddressBook::begin() { return !First ? end() : AddressBookIterator(First, Tranverse); }
	AddressBookIterator AddressBook::end() { return AddressBookIterator(!Last ? nullptr : Last->Next, Tranverse); }
	const AddressBookIterator AddressBook::begin() const { return !First ? end() : AddressBookIterator(First, Tranverse); }
	const AddressBookIterator AddressBook::end() const { return AddressBookIterator(!Last ? nullptr : Last->Next, Tranverse); }
}