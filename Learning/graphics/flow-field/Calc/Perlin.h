#pragma once

#include "Matrix.h"
#include "Vector.h"

#define _USE_MATH_DEFINES

#include <iostream>
#include <math.h>

float lerp(float a1, float a2, float w);
Vector RandomGradient(int ix, int iy);
float DotGridGradient(int ix, int iy, const Vector& Obj);
void CalculatePerlin(int XMax, int YMax, Matrix& Dest);
float CalculatePerlinAtPoint(float x, float y);