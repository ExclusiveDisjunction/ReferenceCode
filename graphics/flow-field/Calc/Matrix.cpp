#include "Matrix.h"

#include <iostream>
#include <sstream>

Matrix::Matrix(int rows, int columns)
{
	Resize(rows, columns);
}
Matrix::Matrix(const Matrix& Obj)
{
	//first resize
	Resize(Obj.rows, Obj.columns);

	//Then fill
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < columns; j++)
			data[i][j] = Obj.data[i][j];
}
Matrix::~Matrix()
{
	Resize(0, 0);
}

Matrix& Matrix::operator=(const Matrix& Obj)
{
	//first resize
	Resize(Obj.rows, Obj.columns);

	//Then fill
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < columns; j++)
			data[i][j] = Obj.data[i][j];

	return *this;
}
float* Matrix::operator[](int row) const
{
	if (row < 0 || row >= this->rows)
		throw std::logic_error("Out of bounds.");

	return data[row];
}

void Matrix::Resize(int nRows, int nColumns)
{
	if (nRows < 0 || nColumns < 0)
		throw std::logic_error("The rows or columns cannot be less than zero.");

	if (nRows == rows && nColumns == columns) //nothing to do
		return;

	if (rows && columns)
	{
		//First clear out all old data.
		for (int i = 0; i < rows; i++)
			delete[] data[i];
		delete[] data;
	}

	data = nullptr;

	//Next, if the size is greater than one, re-construct the matrix.
	if (nRows > 0 && nColumns > 0)
	{
		data = new float*[nRows];
		for (int i = 0; i < nRows; i++)
		{
			data[i] = new float[nColumns];
			for (int j = 0; j < nColumns; j++)
				data[i][j] = 0.0;
		}
	}	

	this->rows = nRows;
	this->columns = nColumns;
}
void Matrix::Reset(float val)
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
			data[i][j] = val;
	}
}

void Matrix::AddBase(float** dst, float** d1, float** d2, int rows, int columns, int coeff)
{
	if (rows < 0 || columns < 0 || (coeff != -1 && coeff != 1))
		throw std::logic_error("invalid inputs.");

	for (int i = 0; i < rows; i++)
		for (int j = 0; j < columns; j++)
			dst[i][j] = d1[i][j] + (coeff)*d2[i][j];
}

Matrix Matrix::operator+(const Matrix& Obj) const
{
	if (this->rows != Obj.rows || this->columns != Obj.columns)
		throw std::logic_error("Dimension missmatch.");

	Matrix Return(rows, columns);
	Matrix::AddBase(Return.data, data, Obj.data, rows, columns, 1);
	return Return;
}
Matrix Matrix::operator-(const Matrix& Obj) const
{
	if (this->rows != Obj.rows || this->columns != Obj.columns)
		throw std::logic_error("Dimension missmatch.");

	Matrix Return(rows, columns);
	Matrix::AddBase(Return.data, data, Obj.data, rows, columns, -1);
	return Return;
}
Matrix& Matrix::operator+=(const Matrix& Obj)
{
	if (this->rows != Obj.rows || this->columns != Obj.columns)
		throw std::logic_error("Dimension missmatch.");

	Matrix::AddBase(data, data, Obj.data, rows, columns, 1);
	return *this;
}
Matrix& Matrix::operator-=(const Matrix& Obj)
{
	if (this->rows != Obj.rows || this->columns != Obj.columns)
		throw std::logic_error("Dimension missmatch.");

	Matrix::AddBase(data, data, Obj.data, rows, columns, -1);
	return *this;
}

std::ofstream& operator<<(std::ofstream& out, const Matrix& Obj)
{
	out << "MATRIX ";
	out << Obj.rows << ' ' << Obj.columns << ' ';

	for (int i = 0; i < Obj.rows; i++)
		for (int j = 0; j < Obj.columns; j++)
			out << Obj.data[i][j] << ' ';

	out << "!!MATRIX";
	return out;
}
std::ifstream& operator>>(std::ifstream& in, Matrix& Obj)
{
	std::string curr;
	in >> curr;
	if (!in || curr != "MATRIX")
	{
		Obj.Resize(0, 0);
		return in;
	}

	int rows, columns;
	in >> rows >> columns;
	if (!in)
	{
		Obj.Resize(0, 0);
		throw std::logic_error("Error: The input dimensions could not be deduced.");
	}

	Obj.Resize(rows, columns);
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			std::streamoff pre = in.tellg();
			std::string raw;
			in >> raw;
			if (raw == "!!MATRIX" || !in)
				throw std::logic_error("Error: The data could not be fully read.");
			
			in.seekg(pre);
			float ReadData;
			in >> ReadData;

			Obj.data[i][j] = ReadData;
		}
	}

	in >> curr;
	if (!in || curr != "!!MATRIX")
	{
		Obj.Resize(0, 0);
		throw std::logic_error("Error: Matrix never closed.");
	}

	return in;
}
std::ostream& operator<<(std::ostream& out, const Matrix& Obj)
{
	for (int i = 0; i < Obj.rows; i++)
	{
		for (int j = 0; j < Obj.columns; j++)
			out << Obj[i][j] << ' ';
		out << std::endl;
	}

	return out;
}