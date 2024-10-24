#include "ConfigurationChain.h"

#include "ConfigurationFile.h"

using namespace Core;

namespace Mason::Configuration
{
	ConfigurationSegmentChainCore::ConfigurationSegmentChainCore(ConfigurationFile* const& Parent)
	{
		First = Last = nullptr;
		Size = 0;
		this->Parent = Parent;
		Parent->_Cores.Add(this);
	}
	ConfigurationSegmentChainCore::ConfigurationSegmentChainCore(ConfigurationSegmentChainCore* const& tmp)
	{
		if (Size != 0)
			Clear();

		if (tmp->Size == 0)
			return;

		if (Parent)
			Parent->_Cores.Remove(this);

		Parent = tmp->Parent;
		Parent->_Cores.Add(this);

		ChainElement* Current = nullptr;

		ConfigurationSegmentChainCore const& ToClone = *tmp;
		for (ChainElement* Obj = ToClone.First; Obj != nullptr; Obj = Obj->Next)
		{
			ChainElement* Last = Current;
			Current = new ChainElement(*Obj);

			if (Last != nullptr)
				Last->Next = Current;
			Current->Last = Last;
			
			Size++;
		}

		this->Last = Current;
	}
	ConfigurationSegmentChainCore::ConfigurationSegmentChainCore(const Vector<ConfigurationSegment*>& Objs)
	{
		if (Size != 0)
			Clear();

		if (Objs.Size == 0)
			return;

		if (Parent)
			Parent->_Cores.Remove(this);

		Parent = Objs[0]->Parent;
		Parent->_Cores.Add(this);

		ChainElement* Current = nullptr, *Last = nullptr;

		for (ConfigurationSegment* Obj : Objs)
		{
			Current = new ChainElement(Obj);
			Current->Last = Last;
			Current->Next = nullptr;
			if (Last)
				Last->Next = Current;

			if (!Last)
				First = Current;

			Last = Current;		

			Size++;
		}

		this->Last = Current;
	}
	ConfigurationSegmentChainCore::~ConfigurationSegmentChainCore()
	{
		Clear();
		First = Last = nullptr;
		Size = 0;

		if (Parent)
			Parent->_Cores.Remove(this);
		Parent = nullptr;
	}

	void ConfigurationSegmentChainCore::Clear()
	{
		if (Size == 0)
			return;

		ChainElement* Current = First, * Temp = nullptr;
		for (unsigned i = 0; i < Size; i++)
		{
			if (!Current)
				continue;

			Temp = Current->Next;
			delete Current;
			Current = Temp;
		}

		First = Last = nullptr;
		Size = 0;
	}
	void ConfigurationSegmentChainCore::DecrementSelf()
	{
		if (RefCount == 0) //Pre Check just in case
			delete this;
		else
		{
			RefCount--;
			if (RefCount == 0)
				delete this;
		}
	}
}
