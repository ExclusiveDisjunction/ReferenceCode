#pragma once

#include <initializer_list>
#include <memory>
#include <vector>
#include "SequenceBase.h"

namespace Core
{
	template<typename T>
	class ListIterator
	{

	};

	template<typename T>
	class List : public SequenceBase<T>
	{
	protected:
		T* Data = nullptr;
		unsigned int _Size;
		unsigned int _Capacity;

		void EnsureCapacity(unsigned int Capacity)
		{
			if (_Capacity > Capacity || _Capacity == Capacity)
				return;

			if (Data)
				realloc(Data, sizeof(T) * Capacity);
			else
				Data = (T*)malloc(sizeof(T) * Capacity);

			_Capacity = Capacity;
		}
	public:
		List()
		{
			Data = nullptr;
			_Size = _Capacity = 0;
		}
		List(const SequenceBase<T>*& Other)
		{
			unsigned int Len = Other->Length();
			if (Len == 0)
				return;

			EnsureCapacity(Len);
			for (unsigned int i = 0; i < Len; i++)
				Data[i] = Other->Item(i);

			_Size = Len;
		}
		List(const T* List, unsigned int Len)
		{
			if (List == nullptr || Len == 0)
				return;

			EnsureCapacity(Len);
			memcpy(Data, List, sizeof(T) * Len);
			_Size = Len;
		}
		List(const std::initializer_list<T>& Data)
		{
			unsigned int Len = Data.size();
			std::vector<T> Conv = std::vector<T>(Data);

			EnsureCapacity(Len);
			unsigned int i = 0;
			for (auto Iterator = Conv.begin(); Iterator != Conv.end(); ++Iterator, i++)
				this->Data[i] = *Iterator;
			
			_Size = Len;
		}
		List(unsigned int Capacity)
		{
			EnsureCapacity(Capacity);
			_Size = 0;
		}
		List(T& Obj)
		{
			EnsureCapacity(1);
			Data[0] = Obj;
		}
		List(const T& Obj)
		{
			EnsureCapacity(1);
			Data[0] = Obj;
		}
		List(const T& Obj, unsigned int Capaicity)
		{
			EnsureCapacity(Capacity);
			_Size = Capacity;

			for (unsigned int i = 0; i < Size; i++)
				Data[i] = Obj;
		}
		virtual ~List()
		{
			Clear();
		}

		T& operator[](unsigned int i) const override
		{
			if (i > Size - 1)
				throw std::exception("OUT OF RANGE");

			return Data[i];
		}
		T& Item(unsigned int Index) const override { return operator[](Index); }
		const unsigned int& Size = _Size;
		const unsigned int& Capacity = _Capacity;
		unsigned int Length() const override { return _Size; }

		void Add(const T& Obj) override
		{
			if (Size == Capacity)
				EnsureCapacity((_Capacity += 3));

			Data[Size] = Obj;
			_Size++;
		}
		void AddList(const SequenceBase<T>* Other) override
		{
			unsigned int Len = Other->Length();
			EnsureCapacity(Size + Len);

			for (unsigned int i = 0, j = Size; i < Len; i++, j++)
				Data[j] = Other->Item(i);
			_Size = _Size + Len;
		}
		void InsertAt(unsigned int Index, const T& Value) override
		{
			if (Size == Capacity)
				EnsureCapacity(Capacity + 1);

			if (Index == (Size - 1))
			{
				_Size++;
				Data[Index] = Value;
				return;
			}

			memcpy(&Data[Index + 1], &Data[Index], sizeof(T) * (Size));
			Data[Index] = Value;
			_Size++;
		}
		bool Remove(const T& Obj) override
		{
			RemoveAt(IndexOf(Obj));
		}
		bool RemoveAt(unsigned int Index) override
		{
			if (Index >= Size)
				return false;

			if (Index == (_Size - 1))
			{
				_Size--;
				return true;
			}
			
			_Size--;
			memcpy(&Data[Index], &Data[Index + 1], sizeof(T) * static_cast<size_t>(Size - Index - 1));
			return true;
		}
		bool RemoveAllOf(T& Condition) override
		{

		}
		bool RemoveDuplicates() override
		{

		}

		bool Contains(const T& Obj) const override
		{

		}
		template<typename Condition>
		bool Search(Condition Cond) const
		{

		}
		template<typename Condition>
		T Find(Condition Cond) const
		{

		}
		unsigned int IndexOf(const T& Obj) const
		{

		}

		void Reverse() override
		{

		}
		void Clear() override
		{
			free(Data);
			_Size = _Capacity = 0;
		}
		bool IsEmpty() const override
		{
			return _Size != 0;
		}

		List<T> SubList(unsigned int BeginIndex) const
		{

		}
		List<T> SubList(unsigned int BeginIndex, unsigned int EndIndex) const
		{

		}
		template<typename ReturnT, typename Conversion>
		List<ReturnT> ConvertList(Conversion Conv)
		{

		}

		ListIterator<T> begin() {}
		ListIterator<T> end() {}
		const ListIterator<T> begin() const { }
		const ListIterator<T> end() const {}

		friend List<T> operator+(const List<T>& One, const T& Two)
		{

		}
		friend List<T> operator+(const List<T>& One, const List<T>& Two)
		{

		}

		List<T>& operator=(const List<T>& Other)
		{

		}
		List<T>& operator=(const T& Obj)
		{

		}

		bool operator==(const List<T>& Other) const
		{

		}
		bool operator!=(const List<T>& Other) const
		{

		}

	};
}