#pragma once

#include <iostream>
#include <fstream>

class Vector
{
public:
	Vector(float x = 0.0, float y = 0.0);

	static Vector FromAngle(float Theta, float Magnitude = 1.0);
	
	float x = 0.0, y = 0.0;

	float Magnitude() const;
	float Angle() const;

	bool operator==(const Vector& Obj) const;
	bool operator!=(const Vector& Obj) const;

	Vector operator+(const Vector& Obj) const;
	Vector operator-(const Vector& Obj) const;
	Vector operator*(float Obj) const;
	Vector operator/(float Obj) const;
	Vector& operator+=(const Vector& Obj);
	Vector& operator-=(const Vector& Obj);
	Vector& operator*=(float Obj);
	Vector& operator/=(float Obj);

	friend std::ostream& operator<<(std::ostream& out, const Vector& Obj);
	friend std::ofstream& operator<<(std::ofstream& out, const Vector& Obj);
	friend std::ifstream& operator>>(std::ifstream& in, Vector& Obj);
};

Vector operator*(float One, const Vector& Two);
Vector operator/(float One, const Vector& Two);