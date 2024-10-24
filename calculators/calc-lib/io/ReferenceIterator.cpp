#include "ReferenceList.h"

namespace Mason::IO
{
	ReferenceIterator::ReferenceIterator(ReferenceListSeg* This) : Current(This) {}
	
	ReferenceIterator::reference ReferenceIterator::operator*()
	{
		return Current->Current;
	}
	ReferenceIterator::pointer ReferenceIterator::operator->()
	{
		return &Current->Current;
	}

	ReferenceIterator& ReferenceIterator::operator++(int)
	{
		return operator++();
	}
	ReferenceIterator& ReferenceIterator::operator++()
	{
		if (Current != nullptr)
			Current = Current->Next;

		return *this;
	}

	ReferenceIterator& ReferenceIterator::operator--(int)
	{
		return operator--();
	}
	ReferenceIterator& ReferenceIterator::operator--()
	{
		if (Current != nullptr)
			Current = Current->Last;

		return *this;
	}

	bool operator==(const ReferenceIterator& A, const ReferenceIterator& B)
	{
		return A.Current == B.Current;
	}
	bool operator!=(const ReferenceIterator& A, const ReferenceIterator& B)
	{
		return A.Current != B.Current;
	}
}