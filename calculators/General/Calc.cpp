#include "Calc.h"

using namespace std;

double Calculation::Ans = 0.0, Calculation::X = 0.0, Calculation::Y = 0.0, Calculation::Z = 0.0, Calculation::A = 0.0, Calculation::B = 0.0, Calculation::C = 0.0;

void Calculation::CloseHandle(CalculationHandle* Handle)
{
	Handle->One = 0.0;
	Handle->Two = 0.0;
	Handle->Command = 0;
	Handle->OneText.clear();
	Handle->TwoText.clear();
}

CalculationHandle Calculation::ReadState(HWND Text1, HWND Text2, HWND Commands)
{
	if (!Text1 || !Text2 || !Commands)
		return CalculationHandle();

	CalculationHandle Handle = { 0 };

	int TextLen = static_cast<int>(SendMessageW(Text1, WM_GETTEXTLENGTH, 0, 0)) + 1;
	wchar_t* Text = new wchar_t[TextLen];
	SendMessageW(Text1, WM_GETTEXT, TextLen, reinterpret_cast<LPARAM>(Text));

	Handle.OneText = Text;
	delete[] Text;
	Text = nullptr;

	TextLen = static_cast<int>(SendMessageW(Text2, WM_GETTEXTLENGTH, 0, 0)) + 1;
	Text = new wchar_t[TextLen];
	SendMessageW(Text2, WM_GETTEXT, TextLen, reinterpret_cast<LPARAM>(Text));

	Handle.TwoText = Text;
	delete[] Text;
	Text = nullptr;

	int ThisIndex = static_cast<int>(SendMessageW(Commands, CB_GETCURSEL, 0, 0));
	TextLen = static_cast<int>(SendMessageW(Commands, CB_GETLBTEXTLEN, 0, 0)) + 1;
	Text = new wchar_t[TextLen];
	SendMessageW(Commands, CB_GETLBTEXT, ThisIndex, reinterpret_cast<LPARAM>(Text));

	Handle.CommandText = Text;
	delete[] Text;
	Text = nullptr;

	return Handle;
}
void Calculation::TranslateState(CalculationHandle* Handle)
{
	wstring& CommandText = Handle->CommandText;
	int& Command = Handle->Command;

	if (CommandText == L"Add")
		Command = COMMAND_ADD;
	else if (CommandText == L"Subtract")
		Command = COMMAND_SUBTRACT;
	else if (CommandText == L"Multiply")
		Command = COMMAND_MULTIPLY;
	else if (CommandText == L"Divide")
		Command = COMMAND_DIVIDE;
	else if (CommandText == L"Mod")
		Command = COMMAND_MOD;
	else if (CommandText == L"Power")
		Command = COMMAND_POW;
	else if (CommandText == L"Square Root")
		Command = COMMAND_SQRT;
	else if (CommandText == L"Store")
		Command = COMMAND_STO;
	else if (CommandText == L"Recall")
		Command = COMMAND_RECALL;

	{
		wstring& OneText = Handle->OneText;
		double& One = Handle->One;
		if (OneText == L"Ans")
			One = Calculation::Ans;
		else if (OneText == L"X" && Command != COMMAND_RECALL)
			One = Calculation::X;
		else if (OneText == L"Y" && Command != COMMAND_RECALL)
			One = Calculation::Y;
		else if (OneText == L"Z" && Command != COMMAND_RECALL)
			One = Calculation::Z;
		else if (OneText == L"A" && Command != COMMAND_RECALL)
			One = Calculation::A;
		else if (OneText == L"B" && Command != COMMAND_RECALL)
			One = Calculation::B;
		else if (OneText == L"C" && Command != COMMAND_RECALL)
			One = Calculation::C;
		else
		{
			wstringstream Stream(OneText);
			Stream >> One;
		}
	}

	{
		wstring& TwoText = Handle->TwoText;
		double& Two = Handle->Two;

		if (TwoText == L"Ans")
			Two = Calculation::Ans;
		else if (TwoText == L"X" && Command != COMMAND_STO)
			Two = Calculation::X;
		else if (TwoText == L"Y" && Command != COMMAND_STO)
			Two = Calculation::Y;
		else if (TwoText == L"Z" && Command != COMMAND_STO)
			Two = Calculation::Z;
		else if (TwoText == L"A" && Command != COMMAND_STO)
			Two = Calculation::A;
		else if (TwoText == L"B" && Command != COMMAND_STO)
			Two = Calculation::B;
		else if (TwoText == L"C" && Command != COMMAND_STO)
			Two = Calculation::C;
		else
		{
			wstringstream Stream(TwoText);
			Stream >> Two;
		}
	}
}
wstring Calculation::ProcessState(CalculationHandle* Handle)
{
	int Command = Handle->Command;

	double Result = 0.0;
	switch (Command)
	{
	case COMMAND_RECALL:
	{
		wstring Target = Handle->OneText;
		
		if (Target == L"X")
			Result = X;
		else if (Target == L"Y")
			Result = Y;
		else if (Target == L"Z")
			Result = Z;
		else if (Target == L"A")
			Result = A;
		else if (Target == L"B")
			Result = B;
		else if (Target == L"C")
			Result = C;
		break;
	}
	case COMMAND_STO:
	{
		wstring Target = Handle->OneText;
		double Val = Handle->Two;

		if (Target == L"X")
			Result = X = Val;
		else if (Target == L"Y")
			Result = Y = Val;
		else if (Target == L"Z")
			Result = Z = Val;
		else if (Target == L"A")
			Result = A = Val;
		else if (Target == L"B")
			Result = B = Val;
		else if (Target == L"C")
			Result = C = Val;

		break;
	}
	case COMMAND_ADD:
	{
		double One = Handle->One;
		double Two = Handle->Two;

		Result = One + Two;
		break;
	}
	case COMMAND_SUBTRACT:
	{
		double One = Handle->One;
		double Two = Handle->Two;

		Result = One - Two;
		break;
	}
	case COMMAND_MULTIPLY:
	{
		double One = Handle->One;
		double Two = Handle->Two;

		Result = One * Two;
		break;
	}
	case COMMAND_DIVIDE:
	{
		double One = Handle->One;
		double Two = Handle->Two;

		Result = One / Two;
		break;
	}
	case COMMAND_MOD:
	{
		long long One = static_cast<long long>(Handle->One);
		long long Two = static_cast<long long>(Handle->Two);

		Result = static_cast<double>(One % Two);
		break;
	}
	case COMMAND_POW:
	{
		double One = Handle->One;
		double Two = Handle->Two;

		Result = pow(One, Two);
		break;
	}
	case COMMAND_SQRT:
	{
		double One = Handle->One;

		Result = sqrt(One);
		break;
	}
	}

	wstring Return = L"Result: " + to_wstring(Result);
	CloseHandle(Handle);
	return Return;
}