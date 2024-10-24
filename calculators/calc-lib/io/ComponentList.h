#pragma once

#include "Component.h"

#include "ComponentIterator.h"

namespace Mason::IO
{
	class FileInstance;

	class ComponentListBase
	{
	protected:
		FileInstance* _Parent;
	public:
		ComponentListBase(FileInstance* File);
		virtual ~ComponentListBase();

		virtual void Clear() = 0;
		virtual bool IsEmpty() const = 0;
	};

	template<typename T> requires (std::is_base_of<Component, T>::value == true)
	class TypedComponentList : public ComponentListBase
	{
	private:
		using Iterator = TypedCompListIterator<T>;
		using Segment = ComponentListSeg<T>;

		Segment* First = nullptr, Last = nullptr;
		unsigned int _Size = 0;

		FileInstance* _File = nullptr;

		Segment* Fetch(unsigned int Index) const
		{
			if (Index >= _Size)
				return nullptr;

			if (Index == 0 && _Size >= 1)
				return First;
			else if (Index == (_Size - 1) && _Size >= 1)
				return Last;


			Segment* Current = First;
			for (unsigned int i = 0; i < Index; i++)
				Current = Current->Next;

			return Current;
		}
		Segment* Fetch(T* const& Obj) const
		{
			if (Obj == nullptr)
				return nullptr;

			for (Segment* Current = First; Current != nullptr; Current = Current->Next)
			{
				if (Current->Current == Obj)
					return Current;
			}

			return nullptr;
		}

		Segment* AppendSegment(T* const& Obj)
		{
			Segment* Item = new Segment(Obj);

			if (Size == 0)
			{
				First = Last = Item;
				Item->Next = Item->Last = nullptr;

				_Size++;
			}
			else if (Size == 1)
			{
				Last = Item;
				First->Next = Item;
				Item->Last = First;

				_Size++;
			}
			else
			{
				Segment* TempLast = Last;
				TempLast->Next = Item;
				Item->Last = TempLast;

				Last = Item;

				_Size++;
			}

			return Item;
		}
		bool Pop(Segment* const& Obj)
		{
			if (Size == 0)
				return false;

			else if (Size == 1 && First == Obj && Last == Obj)
			{
				First = Last = nullptr;
				_Size = 0;

				Obj->Next = Obj->Last = nullptr;
			}
			else if (Obj == First)
			{
				First = Obj->Next;
				First->Last = nullptr;
				Obj->Next = Obj->Last = nullptr;

				_Size--;
			}
			else if (Obj == Last)
			{
				Last = Obj->Last;
				Last->Next = nullptr;

				Obj->Next = Obj->Last = nullptr;

				_Size--;
			}
			else
			{
				Segment* Next = Obj->Next, * Prev = Obj->Last;

				if (Next)
					Next->Last = Prev;

				if (Prev)
					Prev->Next = Next;

				Obj->Next = Obj->Last = nullptr;
				_Size--;
			}
		}
	public:
		TypedComponentList(FileInstance* File) : ComponentListBase(File) {}
		TypedComponentList(FileInstance* File, Iterator Begin, Iterator End) : ComponentListBase(File)
		{
			Clear();

			for (Iterator Current = Begin; Current != End; Current++)
				Append(*Current);
		}
		TypedComponentList(const TypedComponentList<T>& Obj) = delete;
		TypedComponentList(const TypedComponentList<T>&& Obj) = delete;
		~TypedComponentList()
		{
			Clear();
		}

		T* operator[](unsigned int Index) const
		{
			Segment* Item = Fetch(Index);

			return !Item ? nullptr : Item->Current;
		}
		T* ItemAt(unsigned int Index) const
		{
			return operator[](Index);
		}
		unsigned int const& Size = _Size;

		bool Append(T* const& Obj)
		{
			return AppendSegment(Obj) != nullptr;
		}
		bool Pop(T* const& Obj)
		{
			return Pop(Fetch(Obj));
		}
		bool Remove(unsigned int Index)
		{
			Segment* Item = Fetch(Index);
			if (!Item)
				return false;

			Pop(Item);
			delete Item;

			return true;		
		}
		bool Remove(T* const& Obj)
		{
			Segment* Item = Fetch(Obj);
			if (!Item)
				return false;

			Pop(Item);
			delete Item;

			return true;
		}
		void Clear() override
		{
			if (Size == 0)
				return;

			Segment* Current = First;
			for (Current = First; Current != nullptr;)
			{
				Segment* Next = Current->Next;
				delete Current;
				Current = Next;
			}

			_Size = 0;
			First = Last = nullptr;
		}

		bool IsEmpty() const override { return Size == 0; }

		unsigned int IndexOf(T* const& Obj) const
		{
			unsigned int i = 0;
			for (Segment* Current = First; Current != nullptr; Current = Current->Next, i++)
			{
				if (Current->Current = nullptr)
					return i;
			}

			return UINT_MAX;
		}
		bool Exits(T* const& Obj) const
		{
			for (Segment* Current = First; Current != nullptr; Current = Current->Next)
			{
				if (Current->Current == Obj)
					return true;
			}

			return false;
		}
		T* Search(Address ID) const
		{
			for (Segment* Current = First; Current != nullptr; Current = Current->Next)
			{
				if (Current->Current && Current->Current->ID() == ID)
					return Current->Current;
			}

			return nullptr;
		}

		Iterator begin() { return !First ? end() : Iterator(First); }
		Iterator end() { return Iterator(!Last ? nullptr : Last->Next); }
		const Iterator begin() const { return !First ? end() : Iterator(First); }
		const Iterator end() const { return Iterator(!Last ? nullptr : Last->Next); }
	};

	using ComponentList = TypedComponentList<Component>;
}