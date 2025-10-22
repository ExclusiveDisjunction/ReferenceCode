#include "Vector.h"

#include <math.h>

Vector::Vector(float x, float y)
{
	this->x = x;
	this->y = y;
}

Vector Vector::FromAngle(float Theta, float Magnitude)
{
	return Vector((float)cos(Theta), (float)sin(Theta)) * Magnitude;
}

float Vector::Magnitude() const
{
	return (float)sqrt(pow(x, 2.0f) + pow(y, 2.0f));
}
float Vector::Angle() const
{
	return (float)atan2(y, x);
}

bool Vector::operator==(const Vector& Obj) const
{
	return Obj.x == x && Obj.y == y;
}
bool Vector::operator!=(const Vector& Obj) const
{
	return Obj.x != x || Obj.y != y;
}

Vector Vector::operator+(const Vector& Obj) const
{
	Vector Return(*this);
	Return += Obj;

	return Return;
}
Vector Vector::operator-(const Vector& Obj) const
{
	Vector Return(*this);
	Return -= Obj;

	return Return;
}
Vector Vector::operator*(float Two) const
{
	Vector Return(*this);
	Return *= Two;

	return Return;
}
Vector Vector::operator/(float Two) const
{
	Vector Return(*this);
	Return /= Two;

	return Return;
}
Vector& Vector::operator+=(const Vector& Obj)
{
	this->x += Obj.x;
	this->y += Obj.y;

	return *this;
}
Vector& Vector::operator-=(const Vector& Obj)
{
	this->x -= Obj.x;
	this->y -= Obj.y;

	return *this;
}
Vector& Vector::operator*=(float Obj)
{
	this->x *= Obj;
	this->y *= Obj;

	return *this;
}
Vector& Vector::operator/=(float Obj)
{
	this->x /= Obj;
	this->y /= Obj;

	return *this;
}

Vector operator*(float One, const Vector& Two)
{
	return Two * One;
}
Vector operator/(float One, const Vector& Two)
{
	return Two / One;
}

std::ostream& operator<<(std::ostream& out, const Vector& Obj)
{
	out << "< " << Obj.x << ", " << Obj.y << " >";
	return out;
}
std::ofstream& operator<<(std::ofstream& out, const Vector& Obj)
{
	out << "VECTOR 2 " << Obj.x << ' ' << Obj.y << " !!VECTOR";
	return out;
}
std::ifstream& operator>>(std::ifstream& in, Vector& Obj)
{
	std::string curr;
	in >> curr;
	if (!in || curr != "VECTOR")
	{
		Obj.x = 0;
		Obj.y = 0;
		return in;
	}

	int dim;
	in >> dim;
	if (!in)
		throw std::logic_error("Error: The input dimension could not be deduced.");
	
	in >> Obj.x >> Obj.y;
	if (!in)
		throw std::logic_error("Error: The data could not be read.");

	in >> curr;
	if (!in || curr != "!!VECTOR")
		throw std::logic_error("Error: Vector never closed.");

	return in;
}