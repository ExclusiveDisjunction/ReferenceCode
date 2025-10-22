#ifndef ELEMENTITERATOR_H
#define ELEMENTITERATOR_H

#include "IOCommon.h"
#include "Element.h"

namespace Core::IO
{
	/// <summary>
	/// A class that allows for the horizontal transversal of an ElementSimilar object. This satisifies bidirectional_iterator.
	/// </summary>
	/// <typeparam name="ElementT">Any type that satsifies the ElementSimilar concept.</typeparam>
	template<typename ElementT>
	class ElementIterator
	{
	private:
		const void* ParentList = nullptr;
		ElementT* Current = nullptr;
	public:
		ElementIterator(ElementT* Current, const void* ParentList)
		{
			this->Current = Current;
			this->ParentList = ParentList;
		}
		ElementIterator(const ElementIterator<ElementT>& Obj) noexcept
		{
			ParentList = Obj.ParentList;
			Current = Obj.Current;
		}
		ElementIterator(ElementIterator<ElementT>&& Obj) noexcept
		{
			ParentList = std::exchange(Obj.ParentList, nullptr);
			Current = std::exchange(Obj.Current, nullptr);
		}
		~ElementIterator()
		{
			ParentList = Current = nullptr;
		}

		ElementIterator<ElementT>& operator=(const ElementIterator<ElementT>& Obj) noexcept
		{
			ParentList = Obj.ParentList;
			Current = Obj.Current;

			return *this;
		}
		ElementIterator<ElementT>& operator=(ElementIterator<ElementT>& Obj) noexcept
		{
			ParentList = std::exchange(Obj.ParentList, nullptr);
			Current = std::exchange(Obj.Current, nullptr);

			return *this;
		}
		
		
		using iterator_category = std::bidirectional_iterator_tag;
		using difference_type = std::size_t;
		using value_type = ElementT;
		using pointer = ElementT*;
		using reference = ElementT&;

		const void* getParentList() const { return ParentList; }

		reference operator*() const
		{
			if (!Current)
				throw std::logic_error("OUT OF BOUNDS!");

			return *Current;
		}
		pointer operator->()
		{
			return Current;
		}

		ElementIterator& operator++()
		{
			if (Current)
				Current = Current->Next;

			return *this;
		}
		ElementIterator operator++(int)
		{
			ElementIterator<ElementT> temp(*this);
			++(*this);

			return temp;
		}
		ElementIterator& operator--()
		{
			if (Current)
				Current = Current->Last;

			return *this;
		}
		ElementIterator operator--(int)
		{
			ElementIterator<ElementT> temp(*this);
			--(*this);

			return temp;
		}

		difference_type operator-(const ElementIterator& Obj) const
		{
			if (!Obj.Current || !!Current || Obj.Current == Current || Obj.Current->Parent == Current->Parent)
				return 0;

			//Special case, the current and obj are siblings
			if (Obj.Current->Next == Current || Obj.Current->Last == Current)
				return 1;

			/*
			* First, trace back to the first element in the list, the root.
			* Then from the root, count how far the this->Current is from it
			* Then from the root, count how far the Obj.Current is from it
			* Return the absolute value of the difference between them.
			*/

			ElementT* Root;
			size_t CountFromThis = 0,
				CountFromObj = 0;

			for (ElementT* Temp = Current; Temp != nullptr; Temp = Temp->Last)
			{
				Root = Temp; //Will always set the root to be the previous value of temp for each iteration. This means that when temp goes to nullptr, Root will be the first element in the list.
				CountFromThis++;

				if (Temp == Obj.Current)
				{
					//The correct one was found
					return CountFromThis; //Since the count was counting from this->Current going backwards, if it reaches Obj.Current, it holds the distance between this->Current and Obj.Current;
				}
			}

			if (!Root) //Just in case
				return 0;

			for (ElementT* Temp = Root; Temp != nullptr; Temp = Temp->Next)
			{
				if (Temp == Obj.Current)
				{
					int diff = static_cast<int>(CountFromThis - CountFromObj);
					return (size_t)abs(diff);
				}

				CountFromObj++;
			}

			return 0; //Nothing found
		}
		ElementIterator operator+(difference_type diffnum) const
		{
			return (ElementIterator<ElementT>(*this) += diffnum);
		}
		ElementIterator& operator+=(difference_type diffnum)
		{
			while (Current && diffnum > 0)
			{
				Current = Current->Next;
				diffnum--;
			}

			return *this;
		}
		ElementIterator operator-(difference_type diffnum) const
		{
			return (ElementIterator<ElementT>(*this) -= diffnum);
		}
		ElementIterator& operator-=(difference_type diffnum)
		{
			while (Current && diffnum > 0)
			{
				Current = Current->Last;
				diffnum--;
			}
		}

		bool operator==(const ElementIterator& Obj) const
		{
			return Current == Obj.Current;
		}
		bool operator!=(const ElementIterator& Obj) const
		{
			return Current != Obj.Current;
		}
	};
}

#endif