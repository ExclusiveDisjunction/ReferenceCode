#include "Perlin.h"

#include <random>

float lerp(float a1, float a2, float w)
{
	return a1 + (a2 - a1) * w;
}

Vector RandomGradient(int ix, int iy)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution angleDist(0.0f, 2.0f * (float)M_PI);

	float angle = angleDist(gen);
	return Vector(cos(angle), sin(angle));
}
float DotGridGradient(int ix, int iy, const Vector& Obj)
{
	Vector Gradient = RandomGradient(ix, iy);
	Vector Distance = Obj - Vector((float)ix, (float)iy);

	return Distance.x * Gradient.x + Distance.y * Gradient.y;
}
void CalculatePerlin(int XMax, int YMax, Matrix& Dest)
{
	if (XMax <= 0 || YMax <= 0)
	{
		Dest.Resize(0, 0);
		throw std::logic_error("The XMax and YMax must both be greater than zero.");
	}

	//Resize, will reset the matrix to all zeros.
	Dest.Resize(XMax, YMax);

	for (int i = 0; i < XMax; i++)
	{
		for (int j = 0; j < YMax; j++)
		{
			float Result = CalculatePerlinAtPoint((float)i + 0.5f, (float)j + 0.5f);
			//Convert result to [-2pi, 2pi]
			Result *= 2.0f * (float)M_PI;

			Dest[i][j] = Result;
		}
	}
}
float CalculatePerlinAtPoint(float x, float y)
{
	//Determine location
	int x0 = (int)x;
	int x1 = x0 + 1;
	int y0 = (int)y;
	int y1 = y0 + 1;

	//Determine weights
	float sx = x - (float)x0;
	float sy = y - (float)y0;

	float n0, n1, ix0, ix1;

	Vector xy(x, y);

	n0 = DotGridGradient(x0, y0, xy);
	n1 = DotGridGradient(x1, y0, xy);
	ix0 = lerp(n0, n1, sx);

	n0 = DotGridGradient(x0, y1, xy);
	n1 = DotGridGradient(x1, y1, xy);
	ix1 = lerp(n0, n1, sx);

	return lerp(ix0, ix1, sy);
}