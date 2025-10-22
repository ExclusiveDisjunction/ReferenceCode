#pragma once

#include "..\..\StdCalc.h"
#include "..\structure\FunctionBase.h"
#include "..\types\Polynomial.h"

namespace Core::Function
{
	class MATH_LIB BezierMonomial : public FunctionBase
	{
	public:
		BezierMonomial(unsigned int Dim, int i, int n, MathVector Target);

		MathVector Point;
		int i = 0;
		int n = 0;

		MathVector Evaluate(const MathVector& T, bool& Exists) const override;

		bool AllowsChildAppend() const override { return false; }
		unsigned int AllowedChildCount() const override { return 0; }
		FunctionBase* Clone() const override;
	};

	Polynomial* MakeBezier(unsigned int Dim, unsigned int Rank, const std::vector<MathVector>& points);
}