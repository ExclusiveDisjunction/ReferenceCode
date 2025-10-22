#pragma once

#include <math.h>
#include <Windows.h>
#include "Color.h"
#include <random>

using uint = unsigned;
const float PI = 3.141592653;

extern std::default_random_engine RandomEngine;
float Lerp(float X, float Y, float T); //Returns a value that is T between X and Y

class Vector
{
public:
	Vector();
	Vector(double X, double Y);

	double X;
	double Y;

	double Magnitude();
	double Angle();

	Vector& operator+=(const Vector& Other);
	Vector& operator-=(const Vector& Other);
	Vector& operator/=(double Other);
	Vector& operator*=(double Other);
	Vector& operator-();

	friend Vector operator+(const Vector& One, double Two);
	friend Vector operator+(double One, const Vector& Two);
	friend Vector operator+(const Vector& One, const Vector& Two);

	friend Vector operator-(const Vector& One, double Two);
	friend Vector operator-(double One, const Vector& Two);
	friend Vector operator-(const Vector& One, const Vector& Two);

	friend Vector operator*(const Vector& One, double Two);
	friend Vector operator*(double One, const Vector& Two);
	friend Vector operator*(const Vector& One, const Vector& Two);

	friend Vector operator/(const Vector& One, double Two);
	friend Vector operator/(double One, const Vector& Two);
	friend Vector operator/(const Vector& One, const Vector& Two);

	bool operator==(const Vector& Other) const;
	bool operator!=(const Vector& Other) const;

	operator POINT() const { return { static_cast<LONG>(X), static_cast<LONG>(Y) }; }
};
struct AngleRange
{
public:
	float AngleBegin;
	float AngleEnd;
};
struct GRectangle
{
public:
	GRectangle(double X, double Y, double Width, double Height) : X(X), Y(Y), Width(Width), Height(Height) {}

	double X, Y, Width, Height;

	operator RECT() const { return { static_cast<LONG>(X), static_cast<LONG>(Y), static_cast<LONG>(X + Width), static_cast<LONG>(Y + Height) }; }
};

extern const Vector EarthGravity;

Vector TranslateCoord(const Vector& Target, RECT Rect);
GRectangle TranslateRect(const GRectangle& Rect, RECT Area);