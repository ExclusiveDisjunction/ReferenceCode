#include "ReferenceList.h"

#include "FileInstance.h"

namespace Mason::IO
{
	using namespace Core;

	ReferenceListCore::ReferenceListCore(FileInstance* const& Parent)
	{
		First = Last = nullptr;
		Size = 0;
		this->Parent = Parent;
		Parent->_RefCores.Add(this);
	}	
	ReferenceListCore::ReferenceListCore(ReferenceListCore* const& tmp)
	{
		if (Size != 0)
			Clear();

		if (tmp->Size == 0)
			return;

		if (Parent)
			Parent->_RefCores.Remove(this);

		Parent = tmp->Parent;
		Parent->_RefCores.Add(this);

		ReferenceListSeg* Current = nullptr;
		ReferenceListCore const& ToClone = *tmp;
		
		for (ReferenceListSeg* Obj = ToClone.First; Obj != nullptr; Obj = Obj->Next)
		{
			ReferenceListSeg* Last = Current;
			Current = new ReferenceListSeg(*Obj);

			if (Last != nullptr)
				Last->Next = Current;
			Current->Last = Last;

			Size++;
		}

		this->Last = Current;
	}
	ReferenceListCore::ReferenceListCore(const Vector<Reference*>& Obj)
	{
		if (Size != 0)
			Clear();

		if (Obj.Size == 0)
			return;

		if (Parent)
			Parent->_RefCores.Remove(this);

		Parent = Obj[0]->_Record->_Parent->_Parent;
		Parent->_RefCores.Add(this);

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

		this->Last = Current;
	}
	ReferenceListCore::~ReferenceListCore()
	{
		Clear();
		First = Last = nullptr;
		Size = 0;

		if (Parent)
			Parent->_RefCores.Remove(this);
		Parent = nullptr;
	}

	void ReferenceListCore::DecrementSelf()
	{
		if (RefCount == 0) //Pre Check just in case
			delete this;
		else
		{
			RefCount--;
			if (RefCount == 0)
				delete this;
		}
	}
	void ReferenceListCore::Clear()
	{
		if (Size == 0)
			return;

		ReferenceListSeg* Current = First, * Temp = nullptr;
		for (uint i = 0; i < Size; i++)
		{
			if (!Current)
				continue;

			Temp = Current->Next;
			delete Current;
			Current = Temp;
		}

		First = Last = nullptr;
		Size = 0;
	}
}