#include "Element.h"

#include "ElementList.h"
#include "Reference.h"
#include "FileInstance.h"

namespace Core::IO
{
	Element::Element(Element* Parent, bool CanHaveChildren) : Element(Parent ? Parent->ParentFile : nullptr, Parent, CanHaveChildren)
	{

	}
	Element::Element(FileInstance* Inst, Element* Parent, bool CanHaveChildren)
	{
		if (Parent)
		{
			if (!Parent->SupportsChildren())
				throw std::logic_error("ERROR: The parent is valid, but does not support children.");

			Parent->getChildren().push_back(this);
		}

		if (CanHaveChildren)
			State |= ElementState::ES_CanHaveChildren; //NOTE that this->Children is equal to nullptr UNTIL a child is added, so long as (State & ES_CanHaveChildren) is true.

		this->ParentFile = Inst;
		if (Inst)
		{
			if (Inst->IsIdling())
				throw std::logic_error("ERROR: Cannot add a new element when the parent file is idling!");
			this->ID = Inst->GetNextID();
		}
	}
	Element::~Element()
	{
		if (Parent && Parent->Children)
		{
			ElementIterator<Element> iter(this, Parent->Children);
			Parent->Children->detach(iter);
		}
		if (Children)
			delete Children;
		if (RefCore)
			RefCore->Unbind();
	}

	bool Element::HasChildren() const noexcept { return SupportsChildren() && Children != nullptr && Children->count() != 0; }

	unsigned int Element::getTypeID() const noexcept
	{
		return TypeID;
	}
	unsigned int Element::getID() const noexcept
	{
		return ID;
	}
	unsigned short Element::getState() const noexcept
	{
		return State;
	}

	Reference Element::getReference() noexcept
	{
		if (!RefCore)
			RefCore = new ReferenceCore(this);

		return Reference(RefCore);
	}
	Reference Element::getParent() const noexcept
	{
		if (!Parent)
			return Reference(nullptr);

		return Parent->getReference();
	}
	Reference Element::getNextSibling() const noexcept
	{
		if (!Next)
			return Reference(nullptr);

		return Next->getReference();
	}
	Reference Element::getPreviousSibling() const noexcept
	{
		if (!Last)
			return Reference(nullptr);

		return Last->getReference();
	}
	ElementList& Element::getChildren()
	{
		if (!SupportsChildren())
			throw std::logic_error("ERROR: This element does not support children.");

		return *(!Children ? (Children = new ElementList(this)) : Children);
	}
	const ElementList& Element::getChildren() const
	{
		if (!SupportsChildren() || !Children)
			throw std::logic_error("ERROR: This element does not support children, or the children are not loaded.");

		return *Children;
	}
}