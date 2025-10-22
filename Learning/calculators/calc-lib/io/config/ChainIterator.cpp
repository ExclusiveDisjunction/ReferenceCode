#include "ConfigurationChain.h"

namespace Mason::Configuration
{
	ChainIterator::ChainIterator(ChainElement* This) : Current(This) { }

	ChainIterator::reference ChainIterator::operator*()
	{ 
		return Current->Current; 
	}
	ChainIterator::pointer ChainIterator::operator->() 
	{ 
		return &Current->Current;
	}

	ChainIterator& ChainIterator::operator++(int)
	{ 
		ChainIterator& tmp = *this; ++tmp;  return tmp; 
	}
	ChainIterator& ChainIterator::operator++()
	{ 
		Current = Current->Next; return *this;
	}

	ChainIterator& ChainIterator::operator--(int)
	{ 
		--(*this); 
		return *this; 
	}
	ChainIterator& ChainIterator::operator--()
	{ 
		Current = Current->Last; 
		return *this; 
	}

	bool operator==(const ChainIterator& a, const ChainIterator& b)
	{ 
		return a.Current == b.Current; 
	};
	bool operator!=(const ChainIterator& a, const ChainIterator& b) 
	{ 
		return a.Current != b.Current;
	};
}