#include "StdCalc.h"

#include "MathVector.h"
#include <Windows.h>

namespace Core::Calc
{
	double MapLerp(double R1h, double R1l, double R2h, double R2l, double R1p)
	{
		double t = (R1p) / (R1h - R1l);
		return (R2l * (t - 1) + R2h * t);
	}

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