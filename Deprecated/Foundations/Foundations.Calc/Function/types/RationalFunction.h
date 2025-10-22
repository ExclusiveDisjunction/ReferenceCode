#pragma once

#include "..\structure\FunctionBase.h"

#include <iostream>

namespace Core::Calc::Function
{
	class MATH_LIB RationalFunction : public FunctionBase
	{
	public:
		RationalFunction(unsigned int InputDim);
		RationalFunction(FunctionBase* Obj);
		template<std::convertible_to<FunctionBase*>... Args>
		RationalFunction(unsigned int Dim, Args... Objs) : RationalFunction(Dim)
		{
			std::vector<FunctionBase*> Func = std::vector<FunctionBase*>({ (static_cast<FunctionBase*>(Objs))... });
			if (Func.size() == 0)
				return;

			for (FunctionBase* Obj : Func)
				FunctionBase::PushChild(Obj);
		}
		RationalFunction(const RationalFunction& Obj) = delete;
		RationalFunction(RationalFunction&& Obj) = delete;
		~RationalFunction();

		RationalFunction& operator=(const RationalFunction& Obj) = delete;
		RationalFunction& operator=(const RationalFunction& Obj) = delete;

		void MultiplyFunction(FunctionBase* Obj);
		template<std::convertible_to<FunctionBase*>... Args>
		void MultiplyFunctions(Args... Obj)
		{
			std::vector<FunctionBase*> Conv = std::vector<FunctionBase*>({ ((FunctionBase*)Obj)... });

			for (FunctionBase* Item : Conv)
				FunctionBase::PushChild(Item);
		}
		void DivideFunction(FunctionBase* Obj);

		MathVector Evaluate(const MathVector& X, bool& Exists) const override;
		
		bool EquatesTo(FunctionBase* const& Obj) const override;
		bool ComparesTo(FunctionBase* const& Obj) const override;
		FunctionBase* Clone() const override;
	};
}