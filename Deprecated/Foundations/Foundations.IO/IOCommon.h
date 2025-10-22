#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "StrTool.h"

#ifdef FOUNDATIONSIO_EXPORTS 
#define IO_API __declspec(dllexport)
#else
#define IO_API __declspec(dllimport)
#endif