#include "TransformationMatrix.h"

#include <math.h>

void TransformationMatrix::RotateBy(double RadAngle)
{
	double Cos = cos(RadAngle);
	double Sin = sin(RadAngle);

	TransformationMatrix Rotation(Cos, Sin, -Sin, Cos);
	TransformationMatrix Mult = (*this) * Rotation;

	iX = Mult.iX;
	iY = Mult.iY;
	jX = Mult.jX;
	jY = Mult.jY;
}
void TransformationMatrix::SkewBy(double K)
{
	TransformationMatrix Rotation(0, 0, K, 0);
	TransformationMatrix Mult = (*this) * Rotation;

	iX = Mult.iX;
	iY = Mult.iY;
	jX = Mult.jX;
	jY = Mult.jY;
}
void TransformationMatrix::Zoom(double K)
{
	TransformationMatrix Rotation(K, 0, K, 0);
	TransformationMatrix Mult = (*this) * Rotation;

	iX = Mult.iX;
	iY = Mult.iY;
	jX = Mult.jX;
	jY = Mult.jY;
}

Coord& operator*(const TransformationMatrix& Matrix, const Coord& ToModify)
{
	Coord Return = { 0, 0 };

	Return.X = ToModify.X * Matrix.iX + ToModify.X * Matrix.jX;
	Return.Y = ToModify.Y * Matrix.iY + ToModify.Y * Matrix.jY;

	return Return;
}
TransformationMatrix& operator*(const TransformationMatrix& One, const TransformationMatrix& Two)
{
	TransformationMatrix Return;

	Return.iX = One.iX * Two.iX + One.jX * Two.iY;
	Return.iY = One.iY * Two.iX + One.jY * Two.iY;
	
	Return.jX = One.iX * Two.jX + One.jX * Two.jY;
	Return.jY = One.iY * Two.jX + One.jY * Two.jY;

	return Return;
}
TransformationMatrix& TransformationMatrix::operator*=(const TransformationMatrix& Obj)
{
	double tiX, tiY, tjX, tjY;

	tiX = iX * Obj.iX + jX * Obj.iY;
	tiY = iY * Obj.iX + jY * Obj.iY;

	tjX = iX * Obj.jX + jX * Obj.jY;
	tjY = iY * Obj.jX + jY * Obj.jY;

	iX = tiX;
	iY = tiY;
	jX = tjX;
	jY = tjY;

	return *this;
}
