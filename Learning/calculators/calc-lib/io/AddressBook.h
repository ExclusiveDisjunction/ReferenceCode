#pragma once

#include <iostream>

#include "Component.h"
#include "Reference.h"

namespace Mason::IO
{
	class AddressBook;
	class AddressBookIterator;

	class AddressBookRecord
	{
	private:
		AddressBookRecord* Next = nullptr, * Last = nullptr; //Used for transverse ordering
		AddressBookRecord* SNext = nullptr, * SPrevious = nullptr, * SParent = nullptr; //Used for tree based ordering
		AddressBookRecord* SFirst, * SLast = nullptr;

		AddressBook* _Parent;
	public:
		AddressBookRecord() = delete;
		AddressBookRecord(AddressBook* Parent, Address ID);
		AddressBookRecord(AddressBook* Parent, Address ID, Sector* BelongsTo, std::streampos Begin, std::streampos End, Component* Socket = nullptr, Reference* Reference = nullptr);
		AddressBookRecord(const AddressBookRecord& Obj) = delete;
		AddressBookRecord(const AddressBookRecord&& Obj) = delete;
		~AddressBookRecord();

		friend AddressBook;
		friend AddressBookIterator;
		friend class ReferenceListCore;

		AddressBookRecord& operator=(const AddressBookRecord& Obj) = delete;
		AddressBookRecord& operator=(const AddressBookRecord&& Obj) = delete;

		std::streampos Begin = 0;
		std::streampos End = 0;

		Address ID;

		Sector* BelongsTo = nullptr;
		Component* Handle = nullptr;
		Reference* Ref = nullptr;

		bool _Loaded = false;
	};

	enum ABRIType
	{
		Tranverse, //Uses ABR::Next and ABR::Last in tranverse fashion.
		ParentNode, //The node provided is the parent, and ABR::SNext and ABR::SPrevious are used in tree fashion.
		SiblingNode // The node provided is a sibling node, and ABR::SNext and ABR::SPrevious are used in tree fashion.
	};

	class AddressBookIterator
	{
	private:
		AddressBookRecord* Current = nullptr;
		ABRIType Behavior = ABRIType::Tranverse;

	public:
		using iterator_category = std::bidirectional_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = AddressBookRecord*;
		using pointer = value_type*;
		using reference = value_type&;

		AddressBookIterator(AddressBookRecord* Socket, ABRIType Behavior);

		reference operator*();
		pointer operator->();

		AddressBookIterator& operator++(int);
		AddressBookIterator& operator++();

		AddressBookIterator& operator--(int);
		AddressBookIterator& operator--();

		friend bool operator==(const AddressBookIterator& A, const AddressBookIterator& B);
		friend bool operator!=(const AddressBookIterator& A, const AddressBookIterator& B);
	};

	class AddressBook
	{
	private:
		AddressBookRecord* First = nullptr, * Last = nullptr;
		unsigned int _Size = 0;

		FileInstance* _Parent = nullptr;

	public:
		AddressBook() = delete;
		AddressBook(FileInstance* Parent);
		AddressBook(const AddressBook& Obj) = delete;
		AddressBook(const AddressBook&& Obj) = delete;
		~AddressBook();

		friend FileInstance;
		friend class ReferenceListCore;

		AddressBook& operator=(const AddressBook& Obj) = delete;
		AddressBook& operator=(const AddressBook&& Obj) = delete;

		AddressBookRecord* operator[](unsigned int Index) const;
		AddressBookRecord* ItemAt(unsigned int Index) const;
		unsigned int const& Size = _Size;
		
		AddressBookRecord* Append(unsigned int TypeID, Sector* BelongsTo, std::streampos Begin = std::streampos(), std::streampos End = std::streampos(), Component* Sock = nullptr, Reference* Ref = nullptr);
		bool Pop(AddressBookRecord* const& Entry);
		bool Remove(unsigned int Index);
		bool Remove(AddressBookRecord* const& Entry);
		void Clear();

		unsigned int IndexOf(const AddressBookRecord*& Obj) const;
		
		AddressBookIterator begin();
		AddressBookIterator end();
		const AddressBookIterator begin() const;
		const AddressBookIterator end() const;
	};
}