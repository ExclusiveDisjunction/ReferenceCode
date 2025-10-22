#include "BaseMath.h"

const Vector2d EarthGravity = { 0, -9.81 };
const float PI = 3.141592653;

float Lerp(float X, float Y, float T) { return X * (1.0f - T) + (Y * T); }

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
double operator*(const Vector2d& One, const Vector2d& Two) { return One.X * Two.X + One.Y * Two.Y ; }

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

void LinearBezier::Render(HDC Dc, RECT WndRect, COLORREF Color) const
{
	HPEN NewLine = CreatePen(PS_SOLID, 4, Color);
	HGDIOBJ Old = SelectObject(Dc, NewLine);

	MoveToEx(Dc, Begin.X, Begin.Y, NULL);
	for (float z = 0.2f; z <= 1.0f; z += 0.2f)
	{
		float w = (1 - z);
		Vector2d New = (w * Begin) + (z * End);
		LineTo(Dc, New.X, New.Y);
	}
	
	SelectObject(Dc, Old);
}
void QuadraticBezier::Render(HDC Dc, RECT WndRect, COLORREF Color) const
{
	HPEN NewLine = CreatePen(PS_SOLID, 4, Color);
	HGDIOBJ Old = SelectObject(Dc, NewLine);

	MoveToEx(Dc, Begin.X, Begin.Y, NULL);
	for (float z = 0.0f; z <= 1.05f; z += 0.05f)
	{
		float w = (1 - z);
		Vector2d New = ((w * w) * Begin) + (2 * w * z * C1) + ((z * z) * End);
		LineTo(Dc, New.X, New.Y);
	}

	SelectObject(Dc, Old);
}
void CubicBezier::Render(HDC Dc, RECT WndRect, COLORREF Color) const
{
	HPEN NewLine = CreatePen(PS_SOLID, 4, Color);
	HGDIOBJ Old = SelectObject(Dc, NewLine);

	MoveToEx(Dc, Begin.X, Begin.Y, NULL);
	for (float z = 0.0f; z <= 1.05f; z += 0.05f)
	{
		float w = (1 - z);
		Vector2d New = ((w * w * w) * Begin) + (3 * (w * w) * z * C1) + (3 * w * (z * z) * C2) + ((z * z * z) * End);
		LineTo(Dc, New.X, New.Y);
	}

	SelectObject(Dc, Old);
}

void BezierPath::Render(HDC Dc, RECT WndRect, Vector<COLORREF> Colors) const
{
	for (uint i = 0; i < Paths.size(); i++)
		Paths[i]->Render(Dc, WndRect, Colors[i]);
}