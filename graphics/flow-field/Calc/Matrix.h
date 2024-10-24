#pragma once

#include <iostream>
#include <fstream>

class Matrix
{
private:
	float** data = nullptr;
	int rows = 0, columns = 0;

	static void AddBase(float** dst, float** d1, float** d2, int rows, int columns, int coeff); //1 coeff for add, -1 for subtract.
public:
	Matrix() : Matrix(0, 0) {}
	Matrix(int rows, int columns);
	Matrix(const Matrix& Obj);
	~Matrix();

	Matrix& operator=(const Matrix& Obj);
	float* operator[](int row) const;

	void Resize(int nRows, int nColumns);
	void Reset(float val = 0.0);

	int NumRows() const { return rows; }
	int NumCols() const { return columns; }

	Matrix operator+(const Matrix& Obj) const;
	Matrix operator-(const Matrix& Obj) const;
	Matrix& operator+=(const Matrix& Obj);
	Matrix& operator-=(const Matrix& Obj);

	friend std::ofstream& operator<<(std::ofstream& out, const Matrix& Obj); //Outputs to file
	friend std::ifstream& operator>>(std::ifstream& in, Matrix& Obj); //Inputs from file
	friend std::ostream& operator<<(std::ostream& out, const Matrix& Obj); //Outputs for display
};