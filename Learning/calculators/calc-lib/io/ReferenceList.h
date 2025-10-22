#pragma once

#include "Reference.h"
#include "..\Core\Container.h"

namespace Mason::IO
{
	struct ReferenceListSeg
	{
		ReferenceListSeg(Reference* Current) : Current(Current)
		{
			if (Current)
				Current->IncrementSelf();
		}
		~ReferenceListSeg()
		{
			Current->DecrementSelf();
		}

		Reference* Current = nullptr;
		ReferenceListSeg* Next = nullptr, * Last = nullptr;
	};

	class ReferenceIterator
	{
	private:
		ReferenceListSeg* Current = nullptr;
		
	public:
		using iterator_category = std::bidirectional_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = Reference*;
		using pointer = value_type*;
		using reference = value_type&;

		ReferenceIterator(ReferenceListSeg* This);

		reference operator*();
		pointer operator->();

		ReferenceIterator& operator++(int);
		ReferenceIterator& operator++();

		ReferenceIterator& operator--(int);
		ReferenceIterator& operator--();

		friend bool operator==(const ReferenceIterator& A, const ReferenceIterator& B);
		friend bool operator!=(const ReferenceIterator& A, const ReferenceIterator& B);
	};

	class ReferenceListCore
	{
	private:
		ReferenceListCore() = delete;
		ReferenceListCore(FileInstance* const& Parent);
		ReferenceListCore(ReferenceListCore* const& Obj);
		ReferenceListCore(const ReferenceListCore& Obj) = delete;
		ReferenceListCore(const ReferenceListCore&& Obj) = delete;
		ReferenceListCore(const Core::Vector<Reference*>& Obj);
		~ReferenceListCore();

		friend FileInstance;
		friend class ReferenceList;

		ReferenceListCore& operator=(const ReferenceListCore& Obj) = delete;
		ReferenceListCore& operator=(const ReferenceListCore&& Obj) = delete;

		void DecrementSelf();
		void Clear();

		unsigned int Size = 0;
		ReferenceListSeg* First = nullptr, * Last = nullptr;
		FileInstance* Parent = nullptr;

		unsigned int RefCount = 0;

		ReferenceIterator begin() { return !First ? end() : ReferenceIterator(First); }
		ReferenceIterator end() { return ReferenceIterator(!Last ? nullptr : Last->Next); }
		const ReferenceIterator begin() const { return !First ? end() : ReferenceIterator(First); }
		const ReferenceIterator end() const { return ReferenceIterator(!Last ? nullptr : Last->Next); }
	};

	class ReferenceList
	{
	private:
		ReferenceListCore* _Core = nullptr;

		ReferenceListSeg* Fetch(unsigned int Index) const;
		ReferenceListSeg* Fetch(Reference* const& Obj) const;

	public:
		ReferenceList() = delete;
		ReferenceList(FileInstance* File);
		ReferenceList(const ReferenceList& Obj) noexcept;
		ReferenceList(ReferenceList&& Obj) noexcept;
		ReferenceList(const Core::Vector<Reference*>& Obj) noexcept; //Fills the core with the new data instead of replacing the core.
		~ReferenceList();

		Reference* operator[](unsigned int Index) const;
		Reference* operator[](const Component*& Obj) const;
		unsigned int Size() const { return !_Core ? 0 : _Core->Size; }

		bool Append(Reference* const& Obj);
		bool Pop(Reference* const& Obj);
		bool Pop(unsigned int Index);
		bool Pop(ReferenceListSeg* Obj);

		unsigned int IndexOf(Reference* const& Obj) const;
		bool Exists(Reference* const& Obj) const;

		ReferenceList& operator=(const ReferenceList& Obj);
		ReferenceList& operator=(ReferenceList&& Obj) noexcept;
		ReferenceList& operator=(const Core::Vector<Reference*>& Obj); //Fills the core with the new data instead of replacing the core.

		friend bool operator==(const ReferenceList& A, const ReferenceList& B);
		friend bool operator!=(const ReferenceList& A, const ReferenceList& B);

		ReferenceIterator begin() { return !_Core ? end() : _Core->begin(); }
		ReferenceIterator end() { return !_Core ? ReferenceIterator(nullptr) : _Core->end(); }
		const ReferenceIterator begin() const { return !_Core ? end() : _Core->begin(); }
		const ReferenceIterator end() const { return !_Core ? ReferenceIterator(nullptr) : _Core->end(); }
	};
}