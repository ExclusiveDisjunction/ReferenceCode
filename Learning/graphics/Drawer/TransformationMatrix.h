#pragma once

#include "Coord.h"

class TransformationMatrix
{
private:
	double iX, iY;
	double jX, jY;

public:
	TransformationMatrix() : TransformationMatrix(1, 0, 0, 1) { }
	TransformationMatrix(double iX, double iY, double jX, double jY) : iX(iX), iY(iY), jX(jX), jY(jY) {}
	TransformationMatrix(Coord* i, Coord* j) : iX(i->X), iY(i->Y), jX(j->X), jY(j->Y) {}

	void RotateBy(double RadAngle); //Only accepts radian measures
	void SkewBy(double K);
	void Zoom(double K);

	friend Coord& operator*(const TransformationMatrix& Matrix, const Coord& ToModify);
	friend TransformationMatrix& operator*(const TransformationMatrix& One, const TransformationMatrix& Two);

	TransformationMatrix& operator*=(const TransformationMatrix& Obj);
};