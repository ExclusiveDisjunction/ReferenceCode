#pragma once

#include "Component.h"

namespace Mason::IO
{
	template<typename T> requires (std::is_base_of<Component, T>::value == true)
	class TypedComponentList;

	template<typename T> requires (std::is_base_of<Component, T>::value == true)
	class ComponentListSeg
	{
	public:
		T* Current = nullptr;
		ComponentListSeg<T>* Next = nullptr, * Last = nullptr;

		friend TypedComponentList<T>;
	public:
		ComponentListSeg(T* Current) : Current(Current) {}
	};

	template<typename T> requires (std::is_base_of<Component, T>::value == true)
	class TypedCompListIterator
	{
	private:
		ComponentListSeg<T>* Current = nullptr;

	public:
		TypedCompListIterator(ComponentListSeg<T>* Current) : Current(Current) {}

		using iterator_category = std::bidirectional_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = T*;
		using reference = value_type&;
		using pointer = value_type*;

		reference operator*() const
		{
			return Current->Current;
		}
		pointer operator->()
		{
			return &Current->Current;
		}

		TypedCompListIterator<T>& operator++(int)
		{
			return operator++();
		}
		TypedCompListIterator<T>& operator++()
		{
			if (Current)
				Current = Current->Next;

			return *this;
		}

		TypedCompListIterator<T>& operator--(int)
		{
			return operator--();
		}
		TypedCompListIterator<T>& operator--()
		{
			if (Current)
				Current = Current->Last;

			return *this;
		}

		bool operator==(const TypedCompListIterator<T>& B)
		{
			return Current == B.Current;
		}
		bool operator!=(const TypedCompListIterator<T>& B)
		{
			return Current != B.Current;
		}
	};

	using ComponentListIterator = TypedCompListIterator<Component>;

	template<typename T> requires (std::is_base_of<Component, T>::value == true)
	class LeniarTypedCompIterator
	{
	private:
		T* Current = nullptr;

	public:
		LeniarTypedCompIterator(T* Current) : Current(Current) {}

		using iterator_category = std::bidirectional_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = T*;
		using reference = value_type&;
		using pointer = value_type*;

		reference operator*()
		{
			return Current;
		}
		pointer operator->()
		{
			return &Current;
		}

		LeniarTypedCompIterator<T>& operator++(int)
		{
			return operator++();
		}
		LeniarTypedCompIterator<T>& operator++()
		{
			if (Current)
				Current = Current->Next;

			return *this;
		}

		LeniarTypedCompIterator<T>& operator--(int)
		{
			return operator--();
		}
		LeniarTypedCompIterator<T>& operator--()
		{
			if (Current)
				Current = Current->Last;

			return *this;
		}

		bool operator==(const LeniarTypedCompIterator<T>& B)
		{
			return Current == B.Current;
		}
		bool operator!=(const LeniarTypedCompIterator<T>& B)
		{
			return Current != B.Current;
		}
	};

	using LeniarComponentIterator = LeniarTypedCompIterator<Component>;
}