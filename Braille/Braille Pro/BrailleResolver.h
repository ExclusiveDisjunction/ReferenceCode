#pragma once

#include "BrailleDot.h"

struct BrailleGrid
{
public:
	BrailleDot *One, *Two, *Three, *Four, *Five, *Six;
};

bool ProcessKeyDown(WPARAM key);

void GenerateItem(BrailleGrid& GridOne, BrailleGrid& GridTwo);
void FillKey(char Key, BrailleGrid& GridOne, BrailleGrid& GridTwo);