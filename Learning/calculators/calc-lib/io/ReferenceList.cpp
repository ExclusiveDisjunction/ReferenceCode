#include "ReferenceList.h"

namespace Mason::IO
{
	ReferenceList::ReferenceList(FileInstance* File)
	{
		_Core = new ReferenceListCore(File);
	}
	ReferenceList::ReferenceList(const ReferenceList& Obj) noexcept
	{
		if (_Core != nullptr)
		{
			_Core->DecrementSelf();
			_Core = nullptr;
		}

		if (!Obj._Core)
			return;

		_Core = Obj._Core;
		_Core->RefCount++;
	}
	ReferenceList::ReferenceList(ReferenceList&& Obj) noexcept
	{
		if (_Core)
		{
			_Core->DecrementSelf();
			_Core = nullptr;
		}

		_Core = Obj._Core;
		Obj._Core = nullptr;
	}
	ReferenceList::ReferenceList(const Core::Vector<Reference*>& Obj) noexcept
	{
		if (Obj.Size == 0)
			return;

		if (_Core)
		{
			_Core->DecrementSelf();
			_Core = nullptr;
		}

		_Core = new ReferenceListCore(Obj);
		_Core->RefCount++;
	}
	ReferenceList::~ReferenceList()
	{
		if (_Core)
			_Core->DecrementSelf();

		_Core = nullptr;
	}

	Reference* ReferenceList::operator[](unsigned int Index) const
	{
		ReferenceListSeg* Obj = Fetch(Index);
		
		return !Obj ? nullptr : Obj->Current;
	}
	Reference* ReferenceList::operator[](const Component*& Obj) const
	{
		if (!_Core)
			return nullptr;

		for (ReferenceListSeg* Current = _Core->First; Current != nullptr; Current = Current->Next)
		{
			if (Current->Current->_Target == Obj)
				return Current->Current;
		}

		return nullptr;
	}

	ReferenceListSeg* ReferenceList::Fetch(unsigned int Index) const
	{
		if (!_Core)
			return nullptr;

		ReferenceListSeg*& First = _Core->First, *& Last = _Core->Last;
		unsigned int& Size = _Core->Size;

		if (Index >= Size)
			return nullptr;

		if (Index == 0 && Size >= 1)
			return First;
		else if (Index == (Size - 1) && Size >= 1)
			return Last;

		ReferenceListSeg* Current = First;
		for (unsigned int i = 0; i < Index; i++)
			Current = Current->Next;

		return Current;
	}
	ReferenceListSeg* ReferenceList::Fetch(Reference* const& Obj) const
	{
		if (!_Core)
			return nullptr;

		for (ReferenceListSeg* Current = _Core->First; Current != nullptr; Current = Current->Next)
		{
			if (Current->Current == Obj)
				return Current;
		}

		return nullptr;
	}

	bool ReferenceList::Append(Reference* const& Obj)
	{
		ReferenceListSeg*& First = _Core->First;
		ReferenceListSeg*& Last = _Core->Last;
		unsigned int& Size = _Core->Size;

		if (Size == 0)
		{
			First = Last = new ReferenceListSeg(Obj);
			Size++;
			return First;
		}
		else if (Size == 1)
		{
			Last = new ReferenceListSeg(Obj);
			First->Next = Last;
			Last->Last = First;

			Size++;
			return Last;
		}

		ReferenceListSeg* Prev = Last;
		ReferenceListSeg* New = new ReferenceListSeg(Obj);

		New->Last = Prev;
		Last->Next = New;

		Size++;
		Last = New;
		return New;
	}
	bool ReferenceList::Pop(Reference* const& Obj)
	{
		return Pop(Fetch(Obj));
	}
	bool ReferenceList::Pop(unsigned int Index)
	{
		return Pop(Fetch(Index));
	}
	bool ReferenceList::Pop(ReferenceListSeg* Obj)
	{
		if (!_Core)
			return false;

		ReferenceListSeg*& First = _Core->First, *& Last = _Core->Last;
		unsigned int& Size = _Core->Size;

		if (Size == 0)
			return false;
		else if (Size == 1 && Obj == First)
		{
			delete First;
			First = Last = nullptr;
			Size = 0;

			return true;
		}
		else if (Size == 2 && (Obj == First || Obj == Last))
		{
			if (Obj == First)
			{
				First = Last;
				delete Obj;

				Size--;
			}
			else if (Obj == Last)
			{
				Last = First;
				delete Obj;

				Size--;
			}
			else
				return false;

			return true;
		}
		else if (Obj == First)
		{
			First = Obj->Next;
			delete Obj;

			Size--;
			return true;
		}
		else if (Obj == Last)
		{
			Last = Obj->Last;
			delete Obj;

			Size--;
			return true;
		}
		else
		{
			ReferenceListSeg* Next = Obj->Next;
			ReferenceListSeg* Last = Obj->Last;

			if (Last)
				Last->Next = Next;
			Next->Last = Last;

			delete Obj;
			Size--;

			return true;
		}
	}

	unsigned int ReferenceList::IndexOf(Reference* const& Obj) const
	{
		unsigned int Return = 0;

		if (!_Core)
			return UINT_MAX;

		for (ReferenceListSeg* Current = _Core->First; Current != nullptr; Current = Current->Next, Return++)
		{
			if (Current->Current == Obj)
				return Return;
		}

		return UINT_MAX;
	}
	bool ReferenceList::Exists(Reference* const& Obj) const
	{
		return Fetch(Obj) != nullptr;
	}

	ReferenceList& ReferenceList::operator=(const ReferenceList& Obj)
	{
		if (_Core != nullptr)
		{
			_Core->DecrementSelf();
			_Core = nullptr;
		}

		if (!Obj._Core)
			return *this;

		_Core = Obj._Core;
		_Core->RefCount++;

		return *this;
	}
	ReferenceList& ReferenceList::operator=(ReferenceList&& Obj) noexcept
	{
		if (_Core)
		{
			_Core->DecrementSelf();
			_Core = nullptr;
		}

		_Core = Obj._Core;
		Obj._Core = nullptr;

		return *this;
	}
	ReferenceList& ReferenceList::operator=(const Core::Vector<Reference*>& Obj)
	{
		if (_Core->Size != 0)
			_Core->Clear();

		ReferenceListSeg*& First = _Core->First, *& LLast = _Core->Last;
		unsigned int& Size = _Core->Size;

		if (Obj.Size == 0)
			return *this;

		ReferenceListSeg* Current = nullptr, * Last = nullptr;
		for (Reference* Item : Obj)
		{
			Current = new ReferenceListSeg(Item);
			Current->Last = Last;
			Current->Next = nullptr;
			if (Last)
				Last->Next = Current;

			if (!Last)
				First = Current;

			Last = Current;

			Size++;
		}

		LLast = Current;
		return *this;
	}

	bool operator==(const ReferenceList& A, const ReferenceList& B)
	{
		return A._Core == B._Core;
	}
	bool operator!=(const ReferenceList& A, const ReferenceList& B)
	{
		return A._Core != B._Core;
	}
}