#pragma once

#include "IOCommon.h"
#include "Element.h"
#include "ElementIterator.h"

namespace Core::IO
{
	/// <summary>
	/// Describes a list of elements. Can be bound to an element to act as its children, or can exist standalone as storage.
	/// ElementLists OWN the elements that they contain, and therefore will not allow the user to add Elements that are previously owned. 
	/// </summary>
	class IO_API ElementList
	{
	private:
		Element* Host = nullptr;
		Element* First = nullptr;
		Element* Last = nullptr;

		std::size_t _size = 0;

		bool CanBeTakenIn(Element* New) const;

	public:
		/// <summary>
		/// Creates the ElementList unbound to an element.
		/// </summary>
		ElementList() noexcept;
		/// <summary>
		/// Creates the ElementList, and then calls on ElementList::setHost(Element*) to bind to 'Host'.
		/// </summary>
		/// <param name="Host"></param>
		ElementList(Element* Host) noexcept;
		ElementList(const ElementList& Obj) noexcept = delete;
		ElementList(ElementList&& Obj) noexcept = delete;
		~ElementList();

		ElementList& operator=(const ElementList& Obj) noexcept = delete;
		ElementList& operator=(ElementList&& Obj) noexcept = delete;

		using iterator = ElementIterator<Element>;
		using const_iterator = ElementIterator<const Element>;

		iterator operator[](size_t i) noexcept;
		iterator ItemAt(size_t i) noexcept;
		const_iterator operator[](size_t i) const noexcept;
		const_iterator ItemAt(size_t i) const noexcept;

		Element* getHost() const noexcept;
		/// <summary>
		/// Sets the host of the current list, and if 'NewHost'  is not nullptr, it will delete & replace its 'Children' parameter.
		/// Throws if NewHost->CanHaveChildren == false.
		/// </summary>
		/// <param name="NewHost"></param>
		void setHost(Element* NewHost);

		std::size_t count() const noexcept { return _size; }

		bool Contains(Element& Elem) const noexcept;

		/// <summary>
		/// Fills the list with 'newSize' number of Elements, each with the parent equal to the Host of this list.
		/// </summary>
		/// <param name="newSize">The number of new elements. After resize returns, this->count() == newSize.</param>
		void resize(std::size_t newSize) noexcept;
		/// <summary>
		/// Empties the list. Deletes all nodes contained within it. After clear returns, this->count() == 0.
		/// </summary>
		void clear() noexcept;

		/// <summary>
		/// Adds the Element to the end of the list. Throws if the New is nullptr or is previously owned.
		/// </summary>
		/// <param name="New">The new Element to add to the list.</param>
		void push_back(Element* New);
		/// <summary>
		/// Adds the Element after the iterator provided. Throws if the New is nullptr or previously owned, if the 'after' is ATE of the list, or if 'after' is not included in this list.
		/// </summary>
		/// <param name="New">The new Element to add to the list.</param>
		/// <param name="after">The location to add the new element after.</param>
		void insert(Element* New, iterator after);
		/// <summary>
		/// Removes the last item from the list.
		/// </summary>
		void pop_back() noexcept;
		/// <summary>
		/// Removes the item at the iterator provided, assuming that the iterator is in range.
		/// </summary>
		/// <param name="at">The location of the element to remove.</param>
		/// <returns>True if the item was removed, false if 'at' is ATE, or if 'at' is not included in this list.</returns>
		bool erase(iterator at) noexcept;
		/// <summary>
		/// Removes the item at the iterator provided, without deleting the element.
		/// </summary>
		/// <param name="at">The location of the element to remove.</param>
		/// <returns>True if the item was detached, false if 'at' is ATE or if 'at' is not included in the list.</returns>
		bool detach(iterator at) noexcept;

		iterator begin() noexcept;
		iterator end() noexcept;
		const_iterator begin() const noexcept;
		const_iterator end() const noexcept;
	};
}