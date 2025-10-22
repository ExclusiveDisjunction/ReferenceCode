#pragma once

#include "..\structure\FunctionBase.h"
#include <vector>

namespace Core::Function
{
	class MATH_LIB VectorFunction : public FunctionBase
	{
	private:
		void Fill(unsigned int OutputDim);
		void Clear();

		FunctionBase** Func;
	protected:
		void ChildRemoved(FunctionBase* Item) override;

		void AddChild(FunctionBase* Child) override {}
		void RemoveChild(FunctionBase* Child, bool Delete = true) override {}
	public:
		VectorFunction(unsigned int InputDim, unsigned int OutputDim);
		VectorFunction(unsigned int InputDim, unsigned int OutputDim, const std::vector<FunctionBase*>& ToHost) : VectorFunction(InputDim, OutputDim, ToHost.begin(), ToHost.end()) {}
		template<typename iter> requires std::forward_iterator<iter>
		VectorFunction(unsigned int InputDim, unsigned int OutputDim, iter beg, iter end) : VectorFunction(InputDim, OutputDim)
		{
			unsigned int i = 0;
			for (iter curr = beg; curr != end; curr++, i++)
			{
				if (i >= OutputDim)
					throw std::logic_error("Error: The size between the begining and end overflows the output dim of the function.");

				FunctionBase*& obj = *curr;
				if (!obj || obj->InputDim() != _Input || obj->OutputDim() != 1)
					continue;

				AssignParent(obj);
				Func[i] = obj;
			}
		}
		template<std::convertible_to<FunctionBase*>... Args>
		VectorFunction(unsigned int InputDim, unsigned int OutputDim, Args... Value) : VectorFunction(InputDim, OutputDim)
		{
			std::vector<FunctionBase*> ToFill = std::vector<FunctionBase*>({ (static_cast<FunctionBase*>(Value))... });
			if (ToFill.size() != OutputDim)
				return;

			VectorFunction(InputDim, OutputDim, ToFill.begin(), ToFill.end());
		}
		VectorFunction(const VectorFunction& Obj) = delete;
		VectorFunction(VectorFunction&& Obj) = delete;
		virtual ~VectorFunction();

		VectorFunction& operator=(const VectorFunction& Obj) = delete;
		VectorFunction& operator=(VectorFunction&& Obj) = delete;

		void AssignFunction(unsigned int Index, FunctionBase* Func);
		FunctionBase* operator[](unsigned int Index) const;

		MathVector Evaluate(const MathVector& In, bool& Exists) const override;

		unsigned int AllowedChildCount() const override { return _Output; }
		bool AllowsChildAppend() const override { return false; }

		bool ComparesTo(FunctionBase* const& Obj) const override;
		bool EquatesTo(FunctionBase* const& Obj) const override;
		FunctionBase* Clone() const override;

		bool operator==(const VectorFunction& Obj)
		{
			return Obj.EquatesTo(this);
		}
		bool operator!=(const VectorFunction& Obj)
		{
			return !Obj.EquatesTo(this);
		}
	};
}