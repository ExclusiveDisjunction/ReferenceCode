#pragma once

#include <math.h>
#include <Windows.h>
#include <random>
#include <vector>

#include "Container.h"

using uint = unsigned int;
using ulong = unsigned long long;

extern const float PI;
constexpr uint uintmax = 4294967295;

float Lerp(float X, float Y, float T); //Returns a value that is T between X and Y

class Vector2d
{
public:
	Vector2d();
	Vector2d(double X, double Y);

	double X;
	double Y;

	double Magnitude();
	double Angle();

	Vector2d& operator+=(const Vector2d& Other);
	Vector2d& operator-=(const Vector2d& Other);
	Vector2d& operator/=(double Other);
	Vector2d& operator*=(double Other);
	Vector2d& operator-();

	friend Vector2d operator+(const Vector2d& One, const Vector2d& Two);
	friend Vector2d operator-(const Vector2d& One, const Vector2d& Two);

	friend Vector2d operator*(const Vector2d& One, double Two);
	friend Vector2d operator*(double One, const Vector2d& Two);
	friend double operator*(const Vector2d& One, const Vector2d& Two);

	friend Vector2d operator/(const Vector2d& One, double Two);
	friend Vector2d operator/(double One, const Vector2d& Two);

	bool operator==(const Vector2d& Other) const;
	bool operator!=(const Vector2d& Other) const;

	operator POINT() const { return { static_cast<LONG>(X), static_cast<LONG>(Y) }; }
};