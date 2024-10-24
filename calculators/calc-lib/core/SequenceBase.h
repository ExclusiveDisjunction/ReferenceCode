#pragma once

#include "Common.h"

namespace Core
{
	template<typename T>
	class SequenceBase
	{
	public:
		SequenceBase() {}

		virtual T& operator[](unsigned int Index) const = 0;
		virtual T& Item(unsigned int Index) const = 0;
		virtual unsigned int Length() const = 0;
		
		virtual void Add(const T& Obj) = 0;
		virtual void AddList(const SequenceBase<T>* Other) = 0;
		virtual void InsertAt(unsigned int Index, const T& Value) = 0;
		virtual bool Remove(const T& Obj) = 0;
		virtual bool RemoveAt(unsigned int Index) = 0;
		virtual bool RemoveAllOf(T& Condition) = 0;
		virtual bool RemoveDuplicates() = 0;

		virtual bool Contains(const T& Obj) const = 0;
		virtual unsigned int IndexOf(const T& Obj) const = 0;

		virtual void Reverse() = 0;
		virtual void Clear() = 0;
		virtual bool IsEmpty() const = 0;
	};
}