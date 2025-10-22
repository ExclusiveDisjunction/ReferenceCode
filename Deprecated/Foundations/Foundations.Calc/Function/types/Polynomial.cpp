#include "Polynomial.h"

namespace Core::Calc::Function
{
	Polynomial::Polynomial(unsigned int InputDim, unsigned int OutputDim) : FunctionBase(InputDim, OutputDim)
	{

	}
	Polynomial::Polynomial(FunctionBase* Obj) : Polynomial(!Obj ? 0 : Obj->InputDim(), !Obj ? 0 : Obj->OutputDim())
	{
		if (!Obj)
			throw std::logic_error("The provided function cannot be nullptr.");

		AddFunction(Obj);
	}
	Polynomial::~Polynomial()
	{

	}

	void Polynomial::AddFunction(FunctionBase* Obj)
	{
		if (!Obj || Obj->InputDim() != InputDim())
			return;

		FunctionBase::PushChild(Obj);
	}
	void Polynomial::SubtractFunction(FunctionBase* Obj)
	{
		if (!Obj || Obj->InputDim() != InputDim() || Obj->OutputDim() != OutputDim())
			return;

		if (FunctionBase::PushChild(Obj))
			Obj->operator-();
	}

	MathVector Polynomial::Evaluate(const MathVector& Obj, bool& Exists) const
	{
		Exists = true;
		if (this->ChildCount() == 0)
		{
			Exists = false;
			return MathVector::ErrorVector();
		}

		MathVector Output(OutputDim());
		for (const_iterator curr = begin(), end = this->end(); curr != end; curr++)
		{
			try
			{
				MathVector Result = curr->Evaluate(Obj, Exists);
				if (!Exists)
					return MathVector::ErrorVector();

				Output += Result;
			}
			catch (...)
			{
				Exists = false;
				return MathVector::ErrorVector();
			}
		}

		return Output;
	}

	FunctionBase* Polynomial::Clone() const
	{
		Polynomial* Return = new Polynomial(InputDim(), OutputDim());
		try
		{
			CloneBase(Return);
		}
		catch (std::exception& e)
		{
			throw e;
		}

		return Return;
	}
}