#pragma once

#include <iostream>
#include <string>
#include "..\..\StdCalc.h"

namespace Core::Calc::Function
{
	class MATH_LIB FunctionBase;

	class MATH_LIB FunctionIterator
	{
	private:
		FunctionBase* Curr;
		const FunctionBase* ofList;
		unsigned pos;
	public:
		FunctionIterator(FunctionBase* obj, const FunctionBase* host, unsigned p = UINT_MAX);

		using iterator_category = std::random_access_iterator_tag;
		using iterator_tag = std::random_access_iterator_tag;
		using difference_type = std::size_t;
		using value_type = FunctionBase;
		using pointer = FunctionBase*;
		using reference = FunctionBase&;

		reference operator*();
		const reference operator*() const;
		pointer operator->() const;

		difference_type operator-(const FunctionIterator& obj) const;
		FunctionIterator& operator[](difference_type diff);

		FunctionIterator& operator++();
		FunctionIterator operator++(int);
		FunctionIterator& operator--();
		FunctionIterator operator--(int);

		FunctionIterator operator+(difference_type diff) const;
		FunctionIterator& operator+=(difference_type diff);
		FunctionIterator operator-(difference_type diff) const;
		FunctionIterator& operator-=(difference_type diff);

		bool operator<(const FunctionIterator& obj) const;
		bool operator>(const FunctionIterator& obj) const;
		bool operator<=(const FunctionIterator& obj) const;
		bool operator>=(const FunctionIterator& obj) const;

		bool operator==(const FunctionIterator& obj) const;
		bool operator!=(const FunctionIterator& obj) const;
	};

	class MATH_LIB ConstFunctionIterator
	{
	private:
		FunctionIterator iter;
	public:
		ConstFunctionIterator(FunctionIterator iter);

		using iterator_category = std::random_access_iterator_tag;
		using iterator_tag = std::random_access_iterator_tag;
		using difference_type = std::size_t;
		using value_type = const FunctionBase;
		using pointer = value_type*;
		using reference = value_type&;

		reference operator*() const;
		pointer operator->() const;

		difference_type operator-(const ConstFunctionIterator& obj) const;
		ConstFunctionIterator& operator[](difference_type diff);

		ConstFunctionIterator& operator++();
		ConstFunctionIterator operator++(int);
		ConstFunctionIterator& operator--();
		ConstFunctionIterator operator--(int);

		ConstFunctionIterator operator+(difference_type diff) const;
		ConstFunctionIterator& operator+=(difference_type diff);
		ConstFunctionIterator operator-(difference_type diff) const;
		ConstFunctionIterator& operator-=(difference_type diff);

		bool operator<(const ConstFunctionIterator& obj) const;
		bool operator>(const ConstFunctionIterator& obj) const;
		bool operator<=(const ConstFunctionIterator& obj) const;
		bool operator>=(const ConstFunctionIterator& obj) const;

		bool operator==(const ConstFunctionIterator& obj) const;
		bool operator!=(const ConstFunctionIterator& obj) const;
	};

	template<typename T, typename IterT>
	class CastedFunctionIterator
	{
	private:
		IterT iter;
	public:
		CastedFunctionIterator(const IterT i);

		using iterator_tag = IterT::iterator_tag;
		using difference_type = IterT::difference_type;
		using value_type = T;
		using pointer = value_type*;
		using reference = value_type&;

		bool canDecrement() const;
		reference operator*();
		const reference operator*() const;
		pointer operator->() const;

		difference_type operator-(const CastedFunctionIterator<T, IterT>& obj) const;
		CastedFunctionIterator<T, IterT>& operator[](difference_type diff);

		CastedFunctionIterator<T, IterT>& operator++();
		CastedFunctionIterator<T, IterT> operator++(int);
		CastedFunctionIterator<T, IterT>& operator--();
		CastedFunctionIterator<T, IterT> operator--(int);

		CastedFunctionIterator<T, IterT> operator+(difference_type diff) const;
		CastedFunctionIterator<T, IterT>& operator+=(difference_type diff);
		CastedFunctionIterator<T, IterT> operator-(difference_type diff) const;
		CastedFunctionIterator<T, IterT>& operator-=(difference_type diff);

		bool operator<(const CastedFunctionIterator<T, IterT>& obj) const;
		bool operator>(const CastedFunctionIterator<T, IterT>& obj) const;
		bool operator<=(const CastedFunctionIterator<T, IterT>& obj) const;
		bool operator>=(const CastedFunctionIterator<T, IterT>& obj) const;

		bool operator==(const CastedFunctionIterator<T, IterT>& obj) const;
		bool operator!=(const CastedFunctionIterator<T, IterT>& obj) const;
	};
}

#include "CastedFunctionIterator.tpp"