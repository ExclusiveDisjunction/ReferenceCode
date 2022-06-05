#pragma once

#include <math.h>
#include <iostream>
#include <sstream>

#include <Windows.h>
#include <CommCtrl.h>

using std::wstring;
using std::wstringstream;

#define COMMAND_ADD 1
#define COMMAND_MULTIPLY 2
#define COMMAND_SUBTRACT 3
#define COMMAND_DIVIDE 4
#define COMMAND_MOD 5
#define COMMAND_POW 6
#define COMMAND_SQRT 7
#define COMMAND_STO 8
#define COMMAND_RECALL 9

struct CalculationHandle
{
public:
	wstring OneText;
	wstring TwoText;
	wstring CommandText;

	double One = 0.0, Two = 0.0;
	int Command = 0;
};
class Calculation
{
private:
	Calculation() {}

	static void CloseHandle(CalculationHandle* Handle);
public:
	static double Ans;
	static double X, Y, Z, A, B, C;

	static CalculationHandle ReadState(HWND Text1, HWND Text2, HWND Commands); //Commands needs to be a ComboBox, Text1 and Text2 need to be Edit controls.
	static void TranslateState(CalculationHandle* Handle);
	static wstring ProcessState(CalculationHandle* Handle); //Returns the result
};