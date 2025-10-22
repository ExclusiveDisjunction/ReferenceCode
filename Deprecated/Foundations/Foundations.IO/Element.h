#pragma once

#include "IOCommon.h"
#include "AttributesWriter.h"
#include <map>

namespace Core::IO
{
	class IO_API ElementList;
	template<typename ElementT>
	class ElementIterator;
	class IO_API ReferenceCore;
	class IO_API Reference;
	class IO_API FileInstance;

	enum ElementState
	{
		ES_Modified = 1,
		ES_CanHaveChildren = 2,
		ES_Loaded = 4
	};

	/// <summary>
	/// Represents a bare down, one directional tree structure that stores the locations of an Element in a file. This is used by the FileInstance for Idling.
	/// </summary>
	class ElementIdle
	{
	public:
		~ElementIdle()
		{
			if (FirstChild)
			{
				ElementIdle* Current = FirstChild;
				while (Current)
				{
					ElementIdle* Temp = Current->Next;
					delete Current;
					Current = Temp;
				}
			}
		}

		ElementIdle* Next = nullptr;
		ElementIdle* FirstChild = nullptr;

		std::streamoff Begin = 0, End = 0;
		std::streamoff AttrBegin = 0, HEnd = 0;
	};

	/// <summary>
	/// A node in the Element tree. This is the backbone that other types can be made from to hold data.
	/// </summary>
	class IO_API Element
	{
	public: //FIX
		Element* Parent = nullptr;
		Element* Next = nullptr, * Last = nullptr;
		FileInstance* ParentFile = nullptr;

		ElementList* Children = nullptr;
		ReferenceCore* RefCore = nullptr;

		std::streamoff Begin = 0, End = 0;
		std::streamoff AttrBegin = 0, HEnd = 0;

		unsigned int TypeID = 0;
		unsigned int ID = 0;
		unsigned short State = 0;
	protected:		
		Element(FileInstance* Instance, Element* Parent, bool CanHaveChildren);
		Element(Element* Parent, bool CanHaveChildren);

		virtual void WriteAttributes(AttributesWriter& out) const 
		{
			out.WriteKey("Hi") << "your mother";
		} //Element in base class (unloaded) do not output any properties, rather, the system will copy the attributes over for it.
		virtual void ReadAttributes(const std::map<std::string, std::string>& attrs)
		{
			for (const std::pair<std::string, std::string>& kv : attrs)
				std::cout << "\t\"" << kv.first << "\":\"" << kv.second << "\"" << std::endl;
		} //The Element in base class (unloaded) does not read any properties. 

	public:
		Element(Element* Parent) noexcept : Element(Parent, true) {}
		Element(Element& Parent) noexcept : Element(&Parent, true) {}
		Element(const Element& Obj) noexcept = delete;
		Element(Element&& Obj) noexcept = delete;
		virtual ~Element();

		Element& operator=(const Element& Obj) noexcept = delete;
		Element& operator=(Element&& Obj) noexcept = delete;

		friend IO_API ElementList;
		friend ElementIterator<Element>;
		friend ElementIterator<const Element>;
		friend IO_API ReferenceCore;
		friend IO_API FileInstance;

		unsigned int getTypeID() const noexcept;
		unsigned int getID() const noexcept;
		unsigned short getState() const noexcept;
		
		Reference getReference() noexcept;
		Reference getParent() const noexcept;
		Reference getNextSibling() const noexcept;
		Reference getPreviousSibling() const noexcept;
		ElementList& getChildren();
		const ElementList& getChildren() const;
		bool SupportsChildren() const noexcept { return (State & ES_CanHaveChildren); }
		bool HasChildren() const noexcept;
	};
}