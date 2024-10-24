#pragma once

#include "..\StdCalc.h"
#include "Function.h"
#include "Polynomial.h"

namespace SimPack::Math::Function
{
	class MathCore BezierMonomial : public FunctionBase
	{
	public:
		BezierMonomial(unsigned int Dim, int i, int n, MathVector Target) : FunctionBase(Dim, 1), Point(Target)
		{
			this->i = i;
			this->n = n;
			float fac = 1.0f;
			for (int j = 1; j <= i; j++)
				fac *= j;
			this->A = (pow(n, i)) / (fac);
		}
		/*
		* Represents a single iteration of the bezier formula.
		* (n choose i) is equal to FunctionBase::A.
		* i is equal to the position in the polynomial.
		* p sub i is equal to point.
		*/

		MathVector Point;
		int i = 0;
		int n = 0;

		MathVector Evaluate(const MathVector& T, bool& Exists) const override
		{
			return this->A * pow(1 - T, n - 1) * pow(T, i) * Point;
		}
	};

	class Bezier : public FunctionBase
	{
	private:
		unsigned int Rank = 0;
		BezierPolynomial* Segments = nullptr;
	public:
		Bezier(unsigned int Dim, unsigned int Rank) : FunctionBase(Dim, 1) {}
		Bezier(unsigned int Dim, unsigned int Rank, MathVector Obj[])
		{
			for (int i = 0; i < Rank; i++)
			{
				Points[i] = Obj[i];
				Segments->Append(new BezierMonomial(i, Rank, Points[i]));
			}
		}
		virtual ~Bezier()
		{
			delete Segments;
		}

		/// <summary>
		/// The different points of Rank size to calculate.
		/// </summary>
		MathVector<Dim> Points[Rank];

		MathVector<Dim> Evaluate(Scalar T, bool& Exists) const override
		{
			Exists = true;

			if (T < 0.0f || T > 1.0f)
			{
				Exists = false;
				return MathVector<Dim>();
			}

			return Segments->Evaluate(T, Exists);
		}
	};

	template<int Dim> requires (Dim >= 2)
	using LeniarBezier = Bezier<Dim, 2>;

	template<int Dim> requires (Dim >= 2)
	using QuadraticBezier = Bezier<Dim, 3>;

	template<int Dim> requires (Dim >= 2)
	using CubicBezier = Bezier<Dim, 4>;

	template<int Dim> requires (Dim >= 2)
	using QuarticBezier = Bezier<Dim, 5>;
}