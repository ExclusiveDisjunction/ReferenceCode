#pragma once

#include "Vector.h"

#include <d2d1.h>

class Particle
{
private:
	Vector currPos;
	D2D1_COLOR_F Color;
public:
	Particle();
	Particle(const Vector& Position, const D2D1_COLOR_F& Color);

	void RandomizeValues(float xMax, float yMax, const D2D1_COLOR_F& Color);
	
	const Vector& getCurrentPosition() const { return currPos; }
	Vector& getCurrentPosition();
	D2D1_COLOR_F& getColor() { return Color; }
};