#include "ConfigurationFile.h"

namespace Mason::Configuration
{
	ConfigurationFileIterator::ConfigurationFileIterator(ConfigurationSegment* This) : Current(This) { }

	ConfigurationFileIterator::reference ConfigurationFileIterator::operator*()
	{ 
		return Current; 
	}
	ConfigurationFileIterator::pointer ConfigurationFileIterator::operator->()
	{ 
		return &Current; 
	}

	ConfigurationFileIterator& ConfigurationFileIterator::operator++(int) 
	{ 
		ConfigurationFileIterator& tmp = *this;
		++tmp; 
		return tmp;
	}
	ConfigurationFileIterator& ConfigurationFileIterator::operator++()
	{ 
		Current = Current->Next;
		return *this;
	}

	ConfigurationFileIterator& ConfigurationFileIterator::operator--(int)
	{ 
		--(*this); 
		return *this; 
	}
	ConfigurationFileIterator& ConfigurationFileIterator::operator--()
	{ 
		Current = Current->Last; return *this;
	}

	bool operator==(const ConfigurationFileIterator& a, const ConfigurationFileIterator& b)
	{ 
		return a.Current == b.Current;
	}
	bool operator!=(const ConfigurationFileIterator& a, const ConfigurationFileIterator& b) 
	{ 
		return a.Current != b.Current;
	}
}