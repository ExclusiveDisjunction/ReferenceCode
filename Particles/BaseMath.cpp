#include "BaseMath.h"

const Vector EarthGravity = { 0, -9.81 };
std::default_random_engine RandomEngine;

float Lerp(float X, float Y, float T) { return X * (1.0f - T) + (Y * T); }

Vector::Vector() : X(0), Y(0) { }
Vector::Vector(double X, double Y) : X(X), Y(Y) {}

double Vector::Magnitude() { return sqrt(X * X + Y * Y); }
double Vector::Angle() { return atan(Magnitude()); }

Vector& Vector::operator+=(const Vector& Other)
{
	X += Other.X;
	Y += Other.Y;
	return *this;
}
Vector& Vector::operator-=(const Vector& Other)
{
	X -= Other.X;
	Y -= Other.Y;
	return *this;
}
Vector& Vector::operator/=(double Other)
{
	X /= Other;
	Y /= Other;
	return *this;
}
Vector& Vector::operator*=(double Other)
{
	X *= Other;
	Y *= Other;
	return *this;
}
Vector& Vector::operator-()
{
	X = -X;
	Y = -Y;
	return *this;
}

Vector operator+(const Vector& One, double Two) { return { One.X + Two, One.Y + Two }; }
Vector operator+(double One, const Vector& Two) { return { Two.X + One, Two.Y + One }; }
Vector operator+(const Vector& One, const Vector& Two) { return { One.X + Two.X, One.Y + Two.Y }; }

Vector operator-(const Vector& One, double Two) { return { One.X - Two, One.Y - Two }; }
Vector operator-(double One, const Vector& Two) { return { Two.X - One, Two.Y - One }; }
Vector operator-(const Vector& One, const Vector& Two) { return { One.X - Two.X, One.Y - Two.Y }; }

Vector operator*(const Vector& One, double Two) { return { One.X * Two, One.Y * Two }; }
Vector operator*(double One, const Vector& Two) { return { Two.X * One, Two.Y * One }; }
Vector operator*(const Vector& One, const Vector& Two) { return { One.X * Two.X, One.Y * Two.Y }; }

Vector operator/(const Vector& One, double Two) { return { One.X / Two, One.Y / Two }; }
Vector operator/(double One, const Vector& Two) { return { Two.X / One, Two.Y / One }; }
Vector operator/(const Vector& One, const Vector& Two) { return { One.X / Two.X, One.Y / Two.Y }; }

bool Vector::operator==(const Vector& Other) const
{
	return Other.X == X && Other.Y == Y;
}
bool Vector::operator!=(const Vector& Other) const
{
	return !(*this == Other);
}

Vector TranslateCoord(const Vector& Target, RECT Rect)
{
	return { (Rect.right / 2) + Target.X, (Rect.bottom / 2) - Target.Y };
}
GRectangle TranslateRect(const GRectangle& Rect, RECT Area)
{
	return { (Area.right / 2) + Rect.X, (Area.bottom / 2) - Rect.Y - Rect.Height, (Area.right / 2) + Rect.Width, (Area.bottom / 2) - Rect.Height };
}