#pragma once

#include "..\structure\FunctionBase.h"

#include <iostream>

namespace Core::Calc::Function
{
	class MATH_LIB Polynomial : public FunctionBase
	{
	public:
		Polynomial(unsigned int InputDim, unsigned int OutputDim);
		Polynomial(FunctionBase* Obj);
		template<std::convertible_to<FunctionBase*>... Args>
		Polynomial(unsigned int InputDim, unsigned int OutputDim, Args... Objs) : Polynomial(InputDim, OutputDim)
		{
			std::vector<FunctionBase*> Func = std::vector<FunctionBase*>({ (static_cast<FunctionBase*>(Objs))... });

			for (FunctionBase* Item : Func)
				FunctionBase::PushChild(Item);
		}
		Polynomial(const Polynomial& Obj) = delete;
		Polynomial(Polynomial&& Obj) = delete;
		~Polynomial();

		Polynomial& operator=(const Polynomial& Obj) = delete;
		Polynomial& operator=(Polynomial&& Obj) = delete;

		void AddFunction(FunctionBase* Obj);
		template<std::convertible_to<FunctionBase*>... Args>
		void AddFunctions(Args... Obj)
		{
			std::vector<FunctionBase*> Conv = std::vector<FunctionBase*>({ ((FunctionBase*)Obj)... });

			for (FunctionBase* Item : Conv)
				FunctionBase::PushChild(Item);
		}
		void SubtractFunction(FunctionBase* Obj);
		template<std::convertible_to<FunctionBase*>... Args>
		void SubtractFunctions(Args... Obj)
		{
			std::vector<FunctionBase*> Conv = std::vector<FunctionBase*>({ ((FunctionBase*)Obj)... });

			for (FunctionBase* Item : Conv)
			{
				if (FunctionBase::PushChild(Item))
					Item->operator-();
			}
		}

		MathVector Evaluate(const MathVector& Obj, bool& Exists) const override;

		bool EquatesTo(FunctionBase* const& Obj) const override;
		bool ComparesTo(FunctionBase* const& Obj) const override;
		FunctionBase* Clone() const override;
	};
}