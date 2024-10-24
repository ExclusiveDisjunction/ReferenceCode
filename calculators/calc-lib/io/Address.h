#pragma once

#include "..\Common.h"

namespace Mason::IO
{
	struct Address
	{
	public:
		Address(unsigned long long ID, unsigned long long TypeID) : ID(ID), TypeID(TypeID) {}
		Address() : ID(0), TypeID(0) {}

		unsigned long long ID = 0;
		unsigned long long TypeID = 0;
	};
}