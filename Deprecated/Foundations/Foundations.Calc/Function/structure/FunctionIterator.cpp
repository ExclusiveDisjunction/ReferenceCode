#include "FunctionIterator.h"

#include <limits>
#include "FunctionBase.h"

namespace Core::Calc::Function
{
	FunctionIterator::FunctionIterator(FunctionBase* obj, const FunctionBase* host, unsigned p) : Curr(obj), ofList(host), pos(UINT_MAX)
	{
		/*
			We only find pos if:
			1. p == UINT_MAX (default pass for position)
			2. curr != nullptr
			3. ofList != nullptr
			4. ofList->head != nullptr && ofList->tail != nullptr
		*/

		if (p == UINT_MAX && obj && host && host->FirstCh && host->LastCh)
		{
			pos = 0;
			FunctionBase* search = host->FirstCh;
			while (search)
			{
				if (search == obj)
					break;

				pos++;
				search = search->Next;
			}
		}
		else if (p != UINT_MAX)
			pos = p;
	}

	typename FunctionIterator::reference FunctionIterator::operator*()
	{
		if (!Curr)
			throw std::logic_error("Nothing to dereference.");
		
		return *Curr;
	}
	const typename FunctionIterator::reference FunctionIterator::operator*() const
	{
		if (!Curr)
			throw std::logic_error("Nothing to dereference.");

		return *Curr;
	}
	typename FunctionIterator::pointer FunctionIterator::operator->() const
	{
		if (!Curr)
			throw std::logic_error("Nothing to dereference.");

		return Curr;
	}

	typename FunctionIterator::difference_type FunctionIterator::operator-(const FunctionIterator& obj) const
	{
		if (!ofList)
			throw std::logic_error("No host list provided, this operation cannot occur.");

		if (obj.pos == UINT_MAX && pos == UINT_MAX) //both eol
			return 0;
		else if (obj.pos == UINT_MAX) //This one is not eol, but the other one is. Return # of elements between the current element and the list size.
			return ofList->CHCount - pos;
		else if (pos == UINT_MAX) //This one is eol, but the other one is not. Return # of elements between the other's current element and the list size.
			return ofList->CHCount - obj.pos;
		else //Since we have uints, we cannot use abs. So we have to determine which one is larger first.
			return pos > obj.pos ? pos - obj.pos : obj.pos - pos;
	}
	FunctionIterator& FunctionIterator::operator[](FunctionIterator::difference_type diff)
	{
		return (*this) += diff;
	}

	FunctionIterator& FunctionIterator::operator++()
	{
		return (*this) += 1;
	}
	FunctionIterator FunctionIterator::operator++(int)
	{
		FunctionIterator temp(*this);
		(*this) += 1;
		return temp;
	}
	FunctionIterator& FunctionIterator::operator--()
	{
		return (*this) -= 1;
	}
	FunctionIterator FunctionIterator::operator--(int)
	{
		FunctionIterator temp(*this);
		(*this) -= 1;
		return temp;
	}

	FunctionIterator FunctionIterator::operator+(difference_type diff) const
	{
		return (FunctionIterator(*this) += diff);
	}
	FunctionIterator& FunctionIterator::operator+=(difference_type diff)
	{
		while (Curr && diff)
		{
			Curr = Curr->Next;
			pos++;
			diff--;
		}

		if (!Curr || (ofList && pos >= ofList->CHCount)) //Out of bounds
			pos = UINT_MAX;

		return *this;
	}
	FunctionIterator FunctionIterator::operator-(difference_type diff) const
	{
		return (FunctionIterator(*this) -= diff);
	}
	FunctionIterator& FunctionIterator::operator-=(difference_type diff)
	{
		if (ofList && !Curr && diff > 0) //If current is nullptr, we assume that we are at the end of the list.
		{
			Curr = ofList->LastCh;
			diff--;
			pos = ofList->CHCount - 1;
		}

		while (Curr && diff)
		{
			Curr = Curr->Prev;
			pos--;
			diff--;
		}

		if (!Curr || (ofList && pos >= ofList->CHCount)) //Out of bounds
			pos = UINT_MAX;

		return *this;
	}

	bool FunctionIterator::operator<(const FunctionIterator& obj) const
	{
		if (pos == UINT_MAX && obj.pos == UINT_MAX)
			return false; //They are equal
		else if (pos == UINT_MAX)
			return false; //This is greater
		else if (obj.pos == UINT_MAX)
			return true; //this is less
		else 
			return pos < obj.pos;
	}
	bool FunctionIterator::operator>(const FunctionIterator& obj) const
	{
		if (pos == UINT_MAX && obj.pos == UINT_MAX)
        	return false; //They are equal
		else if (pos == UINT_MAX)
			return true; //This is greater
		else if (obj.pos == UINT_MAX)
			return false; //this is less
		else 
			return pos > obj.pos;
	}
	bool FunctionIterator::operator<=(const FunctionIterator& obj) const
	{
		if (pos == UINT_MAX && obj.pos == UINT_MAX)
			return true; //They are equal
		else if (pos == UINT_MAX)
			return false; //This is greater
		else if (obj.pos == UINT_MAX)
			return true; //this is less
		else 
			return pos <= obj.pos;
	}
	bool FunctionIterator::operator>=(const FunctionIterator& obj) const
	{
		if (pos == UINT_MAX && obj.pos == UINT_MAX)
			return true; //They are equal
		else if (pos == UINT_MAX)
			return true; //This is greater
		else if (obj.pos == UINT_MAX)
			return false; //this is less
		else 
			return pos >= obj.pos;
	}

	bool FunctionIterator::operator==(const FunctionIterator& obj) const
	{
		return pos == obj.pos && Curr == obj.Curr && ofList == obj.ofList;
	}
	bool FunctionIterator::operator!=(const FunctionIterator& obj) const
	{
		return pos != obj.pos || Curr != obj.Curr || ofList != obj.ofList;
	}
}