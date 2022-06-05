#pragma once

#include <iostream>
#include <vector>

#include "Str.h"

using std::vector;
using std::wstring;

enum PROC_MODE
{
	NONE = 0,
	FACTOR = 1,
	MULTIPLES = 2,
	EXPORT = 3,
	SETTING_CHANGE = 4,
	EXIT = 5
};

typedef struct __HANDLE
{
	PROC_MODE Mode = PROC_MODE::NONE;
	vector<wstring> Data;
	
};