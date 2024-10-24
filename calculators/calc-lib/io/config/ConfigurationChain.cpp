#include "ConfigurationChain.h"

#include "Core\Vector.h"

using namespace Core;

namespace Mason::Configuration
{
	ConfigurationSegmentChain::ConfigurationSegmentChain(const ConfigurationSegmentChain& Obj) noexcept
	{
		if (_Core != nullptr)
		{
			_Core->DecrementSelf();
			_Core = nullptr;
		}

		if (!Obj._Core)
			return;

		_Core = Obj._Core;
		_Core->RefCount++;
	}
	ConfigurationSegmentChain::ConfigurationSegmentChain(ConfigurationSegmentChain&& Obj) noexcept
	{
		if (_Core)
		{
			_Core->DecrementSelf();
			_Core = nullptr;
		}

		_Core = Obj._Core;
		Obj._Core = nullptr;
	}
	ConfigurationSegmentChain::ConfigurationSegmentChain(const Vector<ConfigurationSegment*>& Obj) noexcept
	{
		if (Obj.Size == 0)
			return;

		if (_Core)
		{
			_Core->DecrementSelf();
			_Core = nullptr;
		}

		_Core = new ConfigurationSegmentChainCore(Obj);
		_Core->RefCount++;
	}
	ConfigurationSegmentChain::~ConfigurationSegmentChain()
	{
		if (_Core)
			_Core->DecrementSelf();

		_Core = nullptr;
	}

	ConfigurationSegment* ConfigurationSegmentChain::operator[](unsigned int Index) const
	{
		ChainElement* Obj = Fetch(Index);

		return !Obj ? nullptr : Obj->Current;
	}
	ConfigurationSegment* ConfigurationSegmentChain::operator[](const String& Name) const
	{
		if (!_Core)
			return nullptr;

		for (ChainElement* Current = _Core->First; Current != nullptr; Current = Current->Next)
		{
			if (Current->Current->Name == Name)
				return Current->Current;
		}

		return nullptr;
	}

	ChainElement* ConfigurationSegmentChain::Fetch(unsigned int Index) const
	{
		if (!_Core)
			return nullptr;

		ChainElement*& First = _Core->First, *& Last = _Core->Last;
		unsigned int& Size = _Core->Size;

		if (Index >= Size)
			return nullptr;

		if (Index == 0 && Size >= 1)
			return First;
		else if (Index == (Size - 1) && Size >= 1)
			return Last;

		ChainElement* Current = First;
		for (unsigned int i = 0; i < Index; i++)
			Current = Current->Next;

		return Current;
	}
	ChainElement* ConfigurationSegmentChain::Fetch(const ConfigurationSegment* const& Obj) const
	{
		if (!_Core)
			return nullptr;

		for (ChainElement* Current = _Core->First; Current != nullptr; Current = Current->Next)
		{
			if (Current->Current == Obj)
				return Current;
		}

		return nullptr;
	}

	bool ConfigurationSegmentChain::Append(ConfigurationSegment* const& Obj)
	{
		ChainElement*& First = _Core->First;
		ChainElement*& Last = _Core->Last;
		unsigned int& Size = _Core->Size;

		if (Size == 0)
		{
			First = Last = new ChainElement(Obj);
			Size++;
			return First;
		}
		else if (Size == 1)
		{
			Last = new ChainElement(Obj);
			First->Next = Last;
			Last->Last = First;

			Size++;
			return Last;
		}

		ChainElement* Prev = Last;
		ChainElement* New = new ChainElement(Obj);

		New->Last = Prev;
		Last->Next = New;

		Size++;
		Last = New;
		return New;
	}
	bool ConfigurationSegmentChain::Pop(ConfigurationSegment* const& Obj)
	{
		return Pop(Fetch(Obj));
	}
	bool ConfigurationSegmentChain::Pop(unsigned int Index)
	{
		return Pop(Fetch(Index));
	}
	bool ConfigurationSegmentChain::Pop(ChainElement* const& Obj)
	{
		if (!_Core)
			return false;

		ChainElement*& First = _Core->First, *& Last = _Core->Last;
		unsigned int& Size = _Core->Size;

		if (Size == 0)
			return false;
		else if (Size == 1 && Obj == First)
		{
			delete First;
			First = Last = nullptr;
			Size = 0;

			return true;
		}
		else if (Size == 2 && (Obj == First || Obj == Last))
		{
			if (Obj == First)
			{
				First = Last;
				delete Obj;

				Size--;
			}
			else if (Obj == Last)
			{
				Last = First;
				delete Obj;

				Size--;
			}
			else
				return false;

			return true;
		}
		else if (Obj == First)
		{
			First = Obj->Next;
			delete Obj;

			Size--;
			return true;
		}
		else if (Obj == Last)
		{
			Last = Obj->Last;
			delete Obj;

			Size--;
			return true;
		}
		else
		{
			ChainElement* Next = Obj->Next;
			ChainElement* Last = Obj->Last;

			if (Last)
				Last->Next = Next;
			Next->Last = Last;

			delete Obj;
			Size--;

			return true;
		}
	}

	bool ConfigurationSegmentChain::Exists(const ConfigurationSegment* const& Obj) const
	{
		return Fetch(Obj) != nullptr;
	}
	unsigned int ConfigurationSegmentChain::IndexOf(const ConfigurationSegment* const& Obj) const
	{
		unsigned int Return = 0;

		if (!_Core)
			return UINT_MAX;

		for (ChainElement* Current = _Core->First; Current != nullptr; Current = Current->Next, Return++)
		{
			if (Current->Current == Obj)
				return Return;
		}

		return UINT_MAX;
	}

	ConfigurationSegmentChain& ConfigurationSegmentChain::operator=(const ConfigurationSegmentChain& Obj)
	{
		if (_Core != nullptr)
		{
			_Core->DecrementSelf();
			_Core = nullptr;
		}

		if (!Obj._Core)
			return *this;

		_Core = Obj._Core;
		_Core->RefCount++;

		return *this;
	}
	ConfigurationSegmentChain& ConfigurationSegmentChain::operator=(ConfigurationSegmentChain&& Obj) noexcept
	{
		if (_Core)
		{
			_Core->DecrementSelf();
			_Core = nullptr;
		}

		_Core = Obj._Core;
		Obj._Core = nullptr;
		
		return *this;
	}

	bool operator==(const ConfigurationSegmentChain& One, const ConfigurationSegmentChain& Two)
	{
		return One._Core == Two._Core;
	}
	bool operator!=(const ConfigurationSegmentChain& One, const ConfigurationSegmentChain& Two)
	{
		return One._Core != Two._Core;
	}
}