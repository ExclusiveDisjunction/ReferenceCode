#include "VectorFunction.h"

using namespace Core;

namespace Core::Function
{
	VectorFunction::VectorFunction(unsigned int InputDim, unsigned int OutputDim) : FunctionBase(InputDim, OutputDim)
	{
		Fill(OutputDim);
	}
	VectorFunction::~VectorFunction()
	{
		for (unsigned int i = 0; i < _Output; i++)
		{
			delete Func[i];
			Func[i] = nullptr;
		}
	}

	void VectorFunction::Fill(unsigned int Dim)
	{
		Clear();

		Func = new FunctionBase*[Dim];
		memset(Func, 0, sizeof(FunctionBase*) * Dim);

		for (unsigned int i = 0; i < Dim; i++)
			Func[i] = nullptr;

		this->Size = Dim;
	}
	void VectorFunction::Clear()
	{
		if (Func)
		{
			for (unsigned int i = 0; i < _Output; i++)
				delete Func[i];

			delete[] Func;
		}

		Func = nullptr;
		Size = 0;
	}
	void VectorFunction::ChildRemoved(FunctionBase* Item)
	{
		//Vector Funtions cannot have children
	}

	FunctionBase* VectorFunction::operator[](unsigned int Index) const
	{
		if (Index >= _Output)
			return nullptr;

		return Func[Index];
	}
	void VectorFunction::AssignFunction(unsigned int Index, FunctionBase* Func)
	{
		if (Index >= _Output || !Func || Func->InputDim() != _Input || Func->OutputDim() != 1)
			return;

		this->Func[Index] = Func;
		AssignParent(Func);
	}

	MathVector VectorFunction::Evaluate(const MathVector& In, bool& Exists) const
	{
		if (_Output == 0 || !Func || In.Dim() != _Input)
		{
			Exists = false;
			return MathVector::ErrorVector();
		}

		MathVector Return(_Output);
		Exists = true;
		for (unsigned int i = 0; i < _Output; i++)
		{
			MathVector Result = Func[i]->Evaluate(In, Exists);
			if (!Exists)
				return MathVector::ErrorVector();
			else
				Return[i] = Result;
		}

		return Return;
	}

	bool VectorFunction::ComparesTo(FunctionBase* const& Obj) const
	{
		const VectorFunction* Conv = dynamic_cast<const VectorFunction*>(Obj);
		if (!Conv)
			return false;

		if (Conv->_Output != _Output || Conv->_Input != _Input)
			return false;

		bool Return = true;
		for (unsigned int i = 0; i < _Output; i++)
			Return &= Conv->Func[i]->ComparesTo(Func[i]);

		return Return;
	}
	bool VectorFunction::EquatesTo(FunctionBase* const& Obj) const
	{
		const VectorFunction* Conv = dynamic_cast<const VectorFunction*>(Obj);
		if (!Conv)
			return false;

		if (Conv->_Output != _Output || Conv->_Input != _Input)
			return false;

		bool Return = true;
		for (unsigned int i = 0; i < _Output; i++)
			Return &= Conv->Func[i]->EquatesTo(Func[i]);

		return Return;
	}
	FunctionBase* VectorFunction::Clone() const
	{
		VectorFunction* Return = new VectorFunction(_Input, _Output);
		for (unsigned int i = 0; i < _Output; i++)
			Return->Func[i] = Func[i]->Clone();

		return Return;
	}
}