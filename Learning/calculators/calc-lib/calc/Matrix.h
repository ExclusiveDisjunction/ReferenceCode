#pragma once

#include "StdCalc.h"
#include "MathVector.h"

namespace Math
{
	/// <summary>
	/// Represents a rectangular arrangment of numbers for calculations, given some row and column definition. 
	/// </summary>
	class MATH_LIB Matrix
	{
	private:
		/// <summary>
		/// The data stored in the matrix. 
		/// </summary>
		double** Data;
		unsigned int _Rows;
		unsigned int _Columns;

		void Allocate(unsigned int NewRows, unsigned int NewColumns, double Value = 0);
		void DeAllocate();

	public:
		Matrix(unsigned int Rows, unsigned int Columns, double Value = 0);
		Matrix(const Matrix& Other) noexcept;
		Matrix(Matrix&& Other) noexcept;
		~Matrix();

		unsigned int Rows() const { return _Rows; }
		unsigned int Columns() const { return _Columns; }
		bool IsValid() const { return _Rows != 0 && _Columns != 0; }

		static Matrix ErrorMatrix();
		static Matrix Identity(unsigned int Size);
		static Matrix RandomMatrix(unsigned int Rows, unsigned int Columns);

		double* operator[](unsigned int Row) const;

		Matrix& operator=(const Matrix& Other) noexcept;
		Matrix& operator=(Matrix&& Other) noexcept;

		Matrix Extract(unsigned int Starti, unsigned int Startj, unsigned int RowCount, unsigned int ColumnCount);

		void RowSwap(unsigned int OrigRow, unsigned int NewRow);
		void RowAdd(unsigned int OrigRow, double Fac, unsigned int TargetRow);

		double Determinant() const;

		Matrix Invert() const;
		Matrix Transpose() const;

		void REF();
		void RREF();

		Matrix& operator*=(double Val);
	};

	MATH_LIB Matrix operator|(const Matrix& One, const Matrix& Two);
	MATH_LIB Matrix operator*(const Matrix& One, const Matrix& Two);
	MATH_LIB MathVector operator*(const Matrix& One, const MathVector& Two);
	MATH_LIB Matrix operator*(double One, const Matrix& Two);
	MATH_LIB Matrix operator*(const Matrix& One, double Two);

	MATH_LIB bool operator==(const Matrix& One, const Matrix& Two);
	MATH_LIB bool operator!=(const Matrix& One, const Matrix& Two);

	MATH_LIB std::ostream& operator<<(std::ostream& out, const Matrix& Data);
}