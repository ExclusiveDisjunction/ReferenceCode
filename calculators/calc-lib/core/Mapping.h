#pragma once

#include <iostream>

namespace Core
{
	template<typename TKey, typename TValue>
	class MappingPair
	{
	public:
		TKey Key;
		TValue Value;

		MappingPair(const TKey& _Key, const TValue& _Value) : Key(const_cast<TKey&>(_Key)), Value(const_cast<TValue&>(_Value)) {}
		MappingPair()
		{
			Key = TKey();
			Value = TValue();
		}
	};

	template<typename TKey, typename TValue>
	class MappingSegment
	{
	public:
		MappingSegment* Next = nullptr, * Last = nullptr;

		MappingPair<TKey, TValue> Root;
		TKey& Key = Root.Key;
		TValue& Value = Root.Value;

		MappingSegment(const TKey& _Key, const TValue& _Value) : Root(_Key, _Value), Next(nullptr), Last(nullptr) {}
		MappingSegment() : Root() {}

		operator MappingPair<TKey, TValue>& ()
		{
			return Root;
		}
	};

	template<typename TKey, typename TValue>
	class MappingIterator
	{
	private:
		MappingSegment<TKey, TValue>* Current;
	public:
		using iterator_category = std::forward_iterator_tag;
		using diffrence_type = std::ptrdiff_t;
		using value_type = MappingPair<TKey, TValue>;
		using pointer = value_type*;
		using reference = value_type&;

		MappingIterator(MappingSegment<TKey, TValue>* This) : Current(This) {}

		reference operator*() const { return Current->Root; }
		pointer operator->() { return &Current->Root; }

		// Prefix increment
		MappingIterator& operator++() { Current = Current->Next; return *this; }

		// Postfix increment
		MappingIterator operator++(int) { MappingIterator tmp = *this; ++(*this); return tmp; }

		friend bool operator== (const MappingIterator& a, const MappingIterator& b) { return a.Current == b.Current; };
		friend bool operator!= (const MappingIterator& a, const MappingIterator& b) { return a.Current != b.Current; };
	};

	template<typename TKey, typename TValue>
	class Mapping
	{
	protected:
		using seg = MappingSegment<TKey, TValue>*;
		using pair = MappingPair<TKey, TValue>;

		seg GetAtIndex(unsigned int Index) const
		{
			seg Return;

			if (_Size == 0 || Index >= _Size)
				Return = nullptr;
			else if (Index == 0)
				Return = First;
			else if (Index == 1 && _Size != 2)
				Return = First->Next;
			else if (Index == _Size - 1)
				Return = Last;
			else if (Index == _Size - 2)
				Return = Last->Last;
			else
			{
				Return = First;
				for (unsigned int i = 0; i < Index; i++)
					Return = Return->Next;
			}

			return Return;
		}
		seg AddNewSegment(const TKey& Key, const TValue& Value)
		{
			seg Return = new MappingSegment<TKey, TValue>(Key, Value);

			if (_Size == 0)
			{
				First = Last = Return;
				_Size = 1;
			}
			else if (_Size == 1)
			{
				Last = Return;
				First->Next = Last;

				_Size++;
			}
			else
			{
				Last->Next = Return;
				Return->Last = Last;
				Last = Return;

				_Size++;
			}

			return Return;
		}

		seg First, Last;
		unsigned int _Size;

	public:
		Mapping() : _Size(0), First(nullptr), Last(nullptr) {}
		Mapping(const MappingPair<TKey, TValue>& Obj)
		{
			Add(Obj);
		}
		Mapping(const Mapping<TKey, TValue>& Obj)
		{
			_Size = Obj._Size;
			for (unsigned int i = 0; i < Obj.Size; i++)
				Add(Obj[i]);
		}
		Mapping(Mapping<TKey, TValue>&& Obj) noexcept
		{
			_Size = Obj._Size;
			First = Obj.First;
			Last = Obj.Last;

			Obj._Size = 0;
			Obj.First = Obj.Last = nullptr;
		}
		Mapping(const std::initializer_list<pair>& Obj)
		{
			for (pair Item : Obj)
				Add(Item);
		}
		virtual ~Mapping()
		{
			Clear();
		}

		Mapping<TKey, TValue>& operator=(const Mapping<TKey, TValue>& Obj)
		{
			Clear();
			_Size = Obj.Size;
			for (unsigned int i = 0; i < Obj.Size; i++)
				Add(Obj[i]);

			return *this;
		}
		Mapping<TKey, TValue>& operator=(Mapping<TKey, TValue>&& Obj)
		{
			Clear();
			First = Obj.First;
			Last = Obj.Last;

			Obj._Size = 0;
			Obj.First = Obj.Last = nullptr;

			return *this;
		}

		const unsigned int& Size = _Size;
		pair& operator[](unsigned int Index) const
		{
			seg Return = GetAtIndex(Index);
			if (!Return)
				throw std::exception("The index provided was out of range.");

			return Return->Root;
		}
		TValue& operator[](const TKey& Key) const
		{
			seg Current = First;
			for (unsigned int i = 0; i < _Size; i++, Current = Current->Next)
			{
				if (Current->Key == Key)
					return Current->Value;
			}

			throw std::exception("The key provided is not in this mapping");
		}
		pair& Item(unsigned int Index) const { return operator[](Index); }

		pair Search(const TKey& For, bool& Found, unsigned int& Index) const
		{
			Index = 0;
			for (seg Current = First; Current != nullptr; Current = Current->Next, Index++)
			{
				if (Current->Key == For)
				{
					Found = true;
					return Current->operator MappingPair<TKey, TValue>&();
				}
			}

			Found = false;
			Index = UINT_MAX;
			return MappingPair<TKey, TValue>();
		}
		pair SearchValue(const TValue& For, bool& Found, unsigned int& Index) const
		{
			Index = 0;
			for (seg Current = First; Current != nullptr; Current = Current->Next, Index++)
			{
				if (Current->Value == For)
				{
					Found = true;
					return Current->operator MappingPair<TKey, TValue>&();
				}
			}

			Found = false;
			Index = UINT_MAX;
			return pair();
		}

		void Add(const TKey& Key, const TValue& Value)
		{
			if (Contains(Key))
				return;

			AddNewSegment(Key, Value);
		}
		void Add(const pair& Pair)
		{
			if (Contains(Pair.Key))
				return;

			AddNewSegment(Pair.Key, Pair.Value);
		}
		bool Remove(const TKey& Key)
		{
			return RemoveAt(IndexOf(Key));
		}
		bool RemoveValue(const TValue& Value)
		{
			unsigned int index = 0;
			for (seg current = First; current != nullptr; current = current->Next, index++)
			{
				if (current->Value == Value)
					break;
			}

			return RemoveAt(index);
		}
		bool RemoveAt(unsigned int Index)
		{
			if (Index >= _Size)
				return false;

			if (Index == 0) //Delete First Index
			{
				seg Second = First->Next;
				delete First;
				if (Second)
					Second->Last = 0;

				First = Second;
				_Size--;
				if (_Size == 0)
					First = Last = nullptr;
				return true;
			}
			else if (Index == (_Size - 1)) //Delete Last Index
			{
				seg SecondLast = Last->Last;
				delete Last;
				if (SecondLast)
					SecondLast->Next = 0;

				Last = SecondLast;
				_Size--;

				if (_Size == 0)
					First = Last = nullptr;
				return true;
			}

			seg Target = GetAtIndex(Index);
			seg Next = Target->Next;
			seg Last = Target->Last;

			if (Last)
				Last->Next = Next;
			Next->Last = Last;

			delete Target;
			_Size--;

			if (_Size == 0)
				First = this->Last = nullptr;

			return true;
		}
		void Clear()
		{
			if (_Size == 0 || !First || !Last)
				return;

			if (Size == 1)
			{
				delete First;

				First = Last = nullptr;
				_Size = 0;
				return;
			}
			else if (Size == 2)
			{
				delete First;
				delete Last;

				First = Last = nullptr;
				_Size = 0;
				return;
			}

			seg Current = this->First;
			for (unsigned int i = 0; i < Size; i++)
			{
				seg Temp = Current->Next;
				delete Current;
				Current = Temp;
			}

			_Size = 0;
			First = Last = nullptr;
		}

		bool Contains(const TKey& Key)
		{
			seg Current = First;
			for (unsigned int i = 0; i < _Size; i++, Current = Current->Next)
			{
				if (Current->Key == Key)
					return true;
			}

			return false;
		}
		unsigned int IndexOf(const TKey& Key)
		{
			seg Current = First;
			for (unsigned int i = 0; i < _Size; i++, Current = Current->Next)
			{
				if (Current->Key == Key)
					return i;
			}

			return UINT64_MAX;
		}

		MappingIterator<TKey, TValue> begin() { return !First ? end() : MappingIterator<TKey, TValue>(First); }
		MappingIterator<TKey, TValue> end() { return MappingIterator<TKey, TValue>(!Last ? nullptr : Last->Next); }
		const MappingIterator<TKey, TValue> begin() const { return !First ? end() : MappingIterator<TKey, TValue>(First); }
		const MappingIterator<TKey, TValue> end() const { return MappingIterator<TKey, TValue>(!Last ? nullptr : Last->Next); }
	};
}
