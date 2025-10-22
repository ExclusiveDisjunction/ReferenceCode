#include "ElementList.h"
#include <iterator>

namespace Core::IO
{
	ElementList::ElementList() noexcept
	{

	}
	ElementList::ElementList(Element* Host) noexcept
	{
		setHost(Host);
	}
	ElementList::~ElementList()
	{
		clear();
		if (Host)
			Host->Children = nullptr;
	}

	bool ElementList::CanBeTakenIn(Element* New) const
	{
		//An element can be taken in if:
		// 1) It exists
		/*
		* An element can be taken in if:
		* 1) It exists
		* 2) It is either unclaimed (parent == nullptr) OR the parent of the Element is the host of this list.
		*/
		return New && (New->Parent == nullptr || New->Parent == Host) && !Contains(*New);
	}
	bool ElementList::Contains(Element& Obj) const noexcept
	{
		for (Element* Curr = First; Curr != nullptr; Curr = Curr->Next)
		{
			if (Curr == &Obj)
				return true;
		}

		return false;
	}

	ElementList::iterator ElementList::operator[](size_t i) noexcept
	{
		return ItemAt(i);
	}
	ElementList::iterator ElementList::ItemAt(size_t i) noexcept
	{
		if (i >= _size)
			return end();

		return begin() + i;
	}
	ElementList::const_iterator ElementList::operator[](size_t i) const noexcept
	{
		return ItemAt(i);
	}
	ElementList::const_iterator ElementList::ItemAt(size_t i) const noexcept
	{
		if (i >= _size)
			return end();

		return begin() + i;
	}

	Element* ElementList::getHost() const noexcept
	{
		return Host;
	}
	void ElementList::setHost(Element* newHost)
	{
		if (Host)
			Host->Children = nullptr;

		if (newHost)
		{
			if (!(newHost->State & ElementState::ES_CanHaveChildren))
				throw std::logic_error("ERROR: The elected host does not support children.");

			if (newHost->Children)
				delete newHost->Children;

			newHost->Children = this;
		}
		
		this->Host = newHost;
	}

	void ElementList::resize(std::size_t newSize) noexcept
	{
		if (newSize == _size)
			return;

		clear();
		_size = newSize;

		if (newSize == 0)
			return;

		Element* current = nullptr;
		for (size_t i = 0; i < newSize; i++)
		{
			Element* temp = current;
			current = new Element(Host);

			if (!First)
				First = current;

			if (temp)
				temp->Next = current;
			current->Last = temp;
		}
		Last = current;
	}
	void ElementList::clear() noexcept
	{
		_size = 0;

		if (!First || !Last)
			return;
		else if (First == Last)
		{
			First->Next = First->Last = First->Parent = nullptr;
			delete First;
			First = Last = nullptr;
			return;
		}

		Element* current = First;
		while (current)
		{
			Element* temp = current->Next;

			current->Next = current->Last = current->Parent = nullptr;
			delete current;

			current = temp;
		}

		First = Last = nullptr;
	}

	void ElementList::push_back(Element* New)
	{
		if (!CanBeTakenIn(New))
			throw std::logic_error("ERROR: 'New' is nullptr, 'New' is in this list, OR 'New' is owned by another parent.");

		if (_size == 0)
		{
			First = Last = New;
			First->Last = nullptr;
			First->Next = nullptr;
		}
		else if (_size == 1)
		{
			First->Next = New;
			Last = New;
			Last->Next = nullptr;
			Last->Last = First;
		}
		else
		{
			Last->Next = New;
			New->Last = Last;
			Last = New;
		}

		New->Parent = Host;
		_size++;
	}
	void ElementList::insert(Element* New, ElementList::iterator after)
	{
		if (after == end() || after.getParentList() != this || !CanBeTakenIn(New))
			throw std::logic_error("ERROR: 'after' is ATE OR 'after' is not part of this list.");
		if (!CanBeTakenIn(New))
			throw std::logic_error("ERROR: 'New' is nullptr, 'New' is currently in this list, OR 'New' is owned by another parent.");

		Element* target = &(*after);
		if (target == Last) //Covers if the _size == 1 && if after is the last element.
		{
			Last->Next = New;
			New->Last = Last;
			New->Next = nullptr;

			Last = New;
		}
		else
		{
			if (!target)
				return;

			Element* targetsNext = target->Next;
			
			target->Next = New;
			New->Last = target;

			if (targetsNext)
				targetsNext->Last = New;
			New->Next = targetsNext;
		}

		New->Parent = Host;
		_size++;
	}
	void ElementList::pop_back() noexcept
	{
		if (_size == 0)
			return;
		else if (_size == 1)
		{
			First->Next = First->Last = First->Parent = nullptr;
			delete First;
			First = Last = nullptr;

			_size = 0;
		}
		else
		{
			Element* Prev = Last->Last;
			Prev->Next = nullptr;
			
			delete Last;
			Last = nullptr;
			_size--;
		}
	}
	bool ElementList::erase(ElementList::iterator At) noexcept
	{
		bool Result = detach(At);
		if (Result)
			delete &(*At);
		
		return Result;
	}
	bool ElementList::detach(ElementList::iterator At) noexcept
	{
		if (At == end() || At.getParentList() != this)
			return false;

		Element* Ptr = &(*At);
		Element* Prev = Ptr->Last,
			* Next = Ptr->Next;

		if (Prev)
			Prev->Next = Next;
		if (Next)
			Next->Last = Prev;

		Ptr->Next = Ptr->Last = Ptr->Parent = nullptr;
		_size--;

		if (_size == 0)
			First = Last = nullptr;
		if (_size == 1)
			First = Last;

		return true;
	}

	ElementList::iterator ElementList::begin() noexcept
	{
		return _size == 0 ? end() : ElementList::iterator(First, this);
	}
	ElementList::iterator ElementList::end() noexcept
	{
		return ElementList::iterator(nullptr, this);
	}
	ElementList::const_iterator ElementList::begin() const noexcept
	{
		return _size == 0 ? end() : ElementList::const_iterator(First, this);
	}
	ElementList::const_iterator ElementList::end() const noexcept
	{
		return ElementList::const_iterator(nullptr, this);
	}
}