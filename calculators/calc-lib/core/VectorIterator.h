#pragma once

namespace Core
{
	template<typename T>
	class VectorIterator;
	template<typename T>
	class Vector;

	template<typename T>
	class VectorSeg
	{
	public:
		VectorSeg() : Next(nullptr), Last(nullptr) {}
		VectorSeg(const T& Value) : Value(Value), Next(nullptr), Last(nullptr) {}

		VectorSeg<T>* Next = nullptr;
		VectorSeg<T>* Last = nullptr;
		T Value = T();
	};

	template<typename T>
	class VectorIterator
	{
	private:
		VectorSeg<T>* Current;
	public:
		using iterator_category = std::forward_iterator_tag;
		using diffrence_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;

		VectorIterator(VectorSeg<T>* This) : Current(This) {}

		reference operator*() const { return Current->Value; }
		pointer operator->() { return &Current->Value; }

		// Prefix increment
		VectorIterator& operator++() { Current = Current->Next; return *this; }

		// Postfix increment
		VectorIterator operator++(int) { VectorIterator tmp = *this; ++(*this); return tmp; }

		friend bool operator== (const VectorIterator& a, const VectorIterator& b) { return a.Current == b.Current; };
		friend bool operator!= (const VectorIterator& a, const VectorIterator& b) { return a.Current != b.Current; };
	};
}