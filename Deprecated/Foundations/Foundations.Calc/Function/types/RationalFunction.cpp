#include "RationalFunction.h"

#include "..\general\CoreFunctions.h"

namespace Core::Calc::Function
{
	RationalFunction::RationalFunction(unsigned int InputDim) : FunctionBase(InputDim, 1)
	{

	}
	RationalFunction::RationalFunction(FunctionBase* Obj) : FunctionBase(!Obj ? 0 : Obj->InputDim())
	{
		MultiplyFunction(Obj);
	}
	RationalFunction::~RationalFunction()
	{

	}

	void RationalFunction::MultiplyFunction(FunctionBase* Obj)
	{
		if (!Obj || Obj->InputDim() != InputDim() || Obj->OutputDim() != 1)
			return;

		FunctionBase::PushChild(Obj);
	}
	void RationalFunction::DivideFunction(FunctionBase* Obj)
	{
		if (!Obj || Obj->InputDim() != InputDim() || Obj->OutputDim() != 1)
			return;

		//We must invert the function
		FunctionBase* Inverted = new FnMonomial(Obj, 1, -1);
		MultiplyFunction(Inverted);
	}

	MathVector RationalFunction::Evaluate(const MathVector& X, bool& Exists) const
	{
		Exists = true;
		if (this->ChildCount() == 0)
		{
			Exists = false;
			return MathVector::ErrorVector();
		}

		MathVector Output(1);
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

	bool RationalFunction::EquatesTo(FunctionBase* const& Obj) const
	{
		RationalFunction* Conv = dynamic_cast<RationalFunction*>(Obj);
		if (!Conv)
			return false;

		if (Conv->InputDim() != InputDim() || Conv->Size != Size)
			return false;

		bool Return = true;
		for (FunctionRelationSeg* CurrentH = First,* CurrentO = Conv->First; CurrentH != nullptr && CurrentO != nullptr; CurrentH = CurrentH->Next, CurrentO = CurrentO->Next)
			Return &= CurrentH->Target && CurrentH->Target->EquatesTo(CurrentO->Target) && CurrentH->Flag == CurrentO->Flag;
		
		return Return;
	}
	bool RationalFunction::ComparesTo(FunctionBase* const& Obj) const
	{
		RationalFunction* Conv = dynamic_cast<RationalFunction*>(Obj);
		if (!Conv)
			return false;

		if (Conv->InputDim() != InputDim() || Conv->Size != Size)
			return false;

		bool Return = true;
		for (FunctionRelationSeg* CurrentH = First, *CurrentO = Conv->First; CurrentH != nullptr && CurrentO != nullptr; CurrentH = CurrentH->Next, CurrentO = CurrentO->Next)
			Return &= CurrentH->Target && CurrentH->Target->ComparesTo(CurrentO->Target) && CurrentH->Flag == CurrentO->Flag;

		return Return;
	}
	FunctionBase* RationalFunction::Clone() const
	{
		if (Size == 0)
			return new RationalFunction(InputDim());

		RationalFunction* Return = new RationalFunction(InputDim());
		FunctionRelationSeg* Current, * Temp = nullptr;
		for (Current = this->First; Current != nullptr; Current = Current->Next)
		{
			FunctionRelationSeg* Other = new FunctionRelationSeg(Current->Target, nullptr, nullptr);
			Other->Flag = Current->Flag;

			if (Temp)
				Temp->Next = Other;
			Other->Previous = Temp;

			if (Return->First != nullptr)
				Return->First = Other;

			Temp = Other;
			Return->Size++;
		}

		Return->Last = Temp;
		return Return;
	}
}