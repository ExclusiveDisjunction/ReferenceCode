#include "Particle.h"

#include <iostream>
#include <time.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <random>

Particle::Particle() : Particle(Vector(), D2D1::ColorF(D2D1::ColorF::Black))
{

}
Particle::Particle(const Vector& PositionInit, const D2D1_COLOR_F& Color)
{
	this->currPos = PositionInit;
	this->Color = Color;
}

void Particle::RandomizeValues(float xMax, float yMax, const D2D1_COLOR_F& Color)
{
	if (xMax <= 0.0f || yMax <= 0.0f)
		throw std::logic_error("The quantities cannot be less than or equal to zero.");

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> xDist(0.0f, xMax), yDist(0.0f, yMax);

	currPos = Vector(xDist(gen), yDist(gen));
	this->Color = Color;
}

Vector& Particle::getCurrentPosition()
{
	return currPos;
}