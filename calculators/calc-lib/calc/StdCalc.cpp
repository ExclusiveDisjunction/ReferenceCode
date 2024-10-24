#include "StdCalc.h"

#include "MathVector.h"
#include "..\Core\Vector.h"
#include <Windows.h>

using namespace Core;

namespace Math
{
	MathVector OriginToScreen(const MathVector& Old, const MathVector& OPos, RECT WndRect)
	{
		return MathVector((Old.Point[0] * 50.0) + OPos.Point[0], (Old.Point[1] * -50.0) + OPos.Point[1]);
	}
	MathVector ScreenToOrigin(const MathVector& Old, const MathVector& OPos, RECT WndRect)
	{
		return MathVector((Old.Point[0] - OPos.Point[0]) / 50, (Old.Point[1] - OPos.Point[1]) / -50);
	}

	double MapLerp(double R1h, double R1l, double R2h, double R2l, double R1p)
	{
		double t = (R1p) / (R1h - R1l);
		return (R2l * (t - 1) + R2h * t);
	}
	//AaColor HSVtoRGB(double H, double S, double V)
	//{
	//	if (H > 360 || H < 0 || S > 100 || S < 0 || V > 100 || V < 0)
	//		return AaColor();
	//
	//	double s = S / 100;
	//	double v = V / 100;
	//	double C = s * v;
	//	double X = C * (1 - abs(fmod(H / 60.0, 2) - 1));
	//	double m = v - C;
	//	double r, g, b;
	//
	//	if (H >= 0 && H < 60)
	//		r = C, g = X, b = 0;
	//	else if (H >= 60 && H < 120)
	//		r = X, g = C, b = 0;
	//	else if (H >= 120 && H < 180)
	//		r = 0, g = C, b = X;
	//	else if (H >= 180 && H < 240)
	//		r = 0, g = X, b = C;
	//	else if (H >= 240 && H < 300)
	//		r = X, g = 0, b = C;
	//	else
	//		r = C, g = 0, b = X;
	//
	//	int R = static_cast<int>((r + m) * 255);
	//	int G = static_cast<int>((g + m) * 255);
	//	int B = static_cast<int>((b + m) * 255);
	//
	//	return AaColor(0xFF, R, G, B);
	//}

	int GreatestCommonDenominator(int a, int b)
	{
		int t;
		while (1)
		{
			t = a % b;
			if (t == 0)
				return b;
			a = b;
			b = t;
		}
	}
}