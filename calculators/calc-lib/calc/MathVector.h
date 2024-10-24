#pragma once

#include "StdCalc.h"

#include <iostream>
#include <vector>

namespace Math
{
	class MATH_LIB Matrix;

	class MATH_LIB MathVector
	{
	private:
		void Allocate(unsigned int Dim, double Val);
		void DeAllocate();

		unsigned int _Dim = 1;

	public:
		MathVector(double Val) : MathVector(1, Val) {}
		MathVector(unsigned int Dim, double Val = 0.0);
		MathVector(unsigned int Dim, double* Point);
		template<std::convertible_to<double>... Args>
		MathVector(Args... Value)
		{
			auto ToFill = std::vector<double>({ ((double)Value)... });
			unsigned int Dim = (unsigned int)ToFill.size();

			if (Dim <= 0)
				throw std::exception("The dimension is equal to zero, and this is not allowed.");
			Allocate(Dim, 0);

			for (unsigned int i = 0; i < Dim; i++)
				Point[i] = ToFill[i];
		}
		MathVector(const MathVector& Obj) noexcept;
		MathVector(MathVector&& Obj) noexcept;
		~MathVector();

		MathVector& operator=(const MathVector& Obj) noexcept;
		MathVector& operator=(MathVector&& Obj) noexcept;

		static MathVector ErrorVector();
		static MathVector ZeroVector(unsigned int Dim);

		double* Point = nullptr;
		int Dim() const { return _Dim; }
		bool IsValid() const { return _Dim > 0; }

		double& operator[](unsigned int TargetDim) const;

		double Magnitude() const;
		double Angle() const;

		static MathVector CrossProduct(const MathVector& One, const MathVector& Two);
		static double DotProduct(const MathVector& One, const MathVector& Two);

		MathVector& operator+=(const MathVector& Obj);
		MathVector& operator-=(const MathVector& Obj);
		MathVector& operator*=(double Obj);
		MathVector& operator/=(double Obj);

		operator double() const;
		operator Matrix() const;

#ifdef _WINDOWS_
		operator POINT() const
		{
			if (Dim() < 2)
				throw std::exception("This object does not have enough dimensions to construct a POINT object.");

			return POINT{ static_cast<LONG>(Point[0]), static_cast<LONG>(Point[1]) };
		}
#endif
	};

	MATH_LIB MathVector operator+(const MathVector& One, const MathVector& Two);
	MATH_LIB MathVector operator-(const MathVector& One, const MathVector& Two);
	MATH_LIB MathVector operator*(const MathVector& One, double Two);
	MATH_LIB MathVector operator*(double One, const MathVector& Two);
	MATH_LIB MathVector operator/(const MathVector& One, double Two);

	MATH_LIB bool operator==(const MathVector& One, const MathVector& Two);
	MATH_LIB bool operator!=(const MathVector& One, const MathVector& Two);
}