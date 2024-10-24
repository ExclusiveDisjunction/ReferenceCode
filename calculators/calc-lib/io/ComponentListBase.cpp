#include "ComponentList.h"

#include "FileInstance.h"

namespace Mason::IO
{
	ComponentListBase::ComponentListBase(FileInstance* File) : _Parent(File)
	{
		_Parent->_CompLists.Add(this);
	}
	ComponentListBase::~ComponentListBase()
	{
		_Parent->_CompLists.Remove(this);
		Clear();
	}
}