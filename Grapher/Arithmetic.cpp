#include "Arithmetic.h"

const double PI = 3.141592653;

Vector2d::Vector2d() : X(0), Y(0) { }
Vector2d::Vector2d(double X, double Y) : X(X), Y(Y) {}

double Vector2d::Magnitude() { return sqrt(X * X + Y * Y); }
double Vector2d::Angle() { return atan(Magnitude()); }

Vector2d& Vector2d::operator+=(const Vector2d& Other)
{
	X += Other.X;
	Y += Other.Y;
	return *this;
}
Vector2d& Vector2d::operator-=(const Vector2d& Other)
{
	X -= Other.X;
	Y -= Other.Y;
	return *this;
}
Vector2d& Vector2d::operator/=(double Other)
{
	X /= Other;
	Y /= Other;
	return *this;
}
Vector2d& Vector2d::operator*=(double Other)
{
	X *= Other;
	Y *= Other;
	return *this;
}
Vector2d& Vector2d::operator-()
{
	X = -X;
	Y = -Y;
	return *this;
}

Vector2d operator+(const Vector2d& One, const Vector2d& Two) { return { One.X + Two.X, One.Y + Two.Y }; }
Vector2d operator-(const Vector2d& One, const Vector2d& Two) { return { One.X - Two.X, One.Y - Two.Y }; }

Vector2d operator*(const Vector2d& One, double Two) { return { One.X * Two, One.Y * Two }; }
Vector2d operator*(double One, const Vector2d& Two) { return { Two.X * One, Two.Y * One }; }
double operator*(const Vector2d& One, const Vector2d& Two) { return One.X * Two.X + One.Y * Two.Y; }

Vector2d operator/(const Vector2d& One, double Two) { return { One.X / Two, One.Y / Two }; }
Vector2d operator/(double One, const Vector2d& Two) { return { Two.X / One, Two.Y / One }; }

bool Vector2d::operator==(const Vector2d& Other) const
{
	return Other.X == X && Other.Y == Y;
}
bool Vector2d::operator!=(const Vector2d& Other) const
{
	return !(*this == Other);
}