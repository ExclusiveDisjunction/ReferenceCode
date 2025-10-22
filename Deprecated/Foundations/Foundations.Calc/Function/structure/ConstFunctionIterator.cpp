#include "FunctionIterator.h"

#include "FunctionBase.h"

namespace Core::Calc::Function
{
	ConstFunctionIterator::ConstFunctionIterator(FunctionIterator iter) : iter(iter) {}

	typename ConstFunctionIterator::reference ConstFunctionIterator::operator*() const
	{
		try
		{
			return const_cast<const FunctionBase&>(*iter);
		}
		catch (std::logic_error& e)
		{
			throw e;
		}
		catch (std::bad_cast& e)
		{
			throw e;
		}
	}
	typename ConstFunctionIterator::pointer ConstFunctionIterator::operator->() const
	{
		try
		{
			FunctionBase* ptr = iter->operator->();
			return const_cast<const FunctionBase*>(ptr);
		}
		catch (std::exception& e)
		{
			throw e;
		}
	}

	typename ConstFunctionIterator::difference_type ConstFunctionIterator::operator-(const ConstFunctionIterator& obj) const
	{
		return iter - obj.iter;
	}
	ConstFunctionIterator& ConstFunctionIterator::operator[](ConstFunctionIterator::difference_type diff)
	{
		iter[diff];
		return *this;
	}

	ConstFunctionIterator& ConstFunctionIterator::operator++()
	{
		++iter;
		return *this;
	}
	ConstFunctionIterator ConstFunctionIterator::operator++(int)
	{
		ConstFunctionIterator Return(*this);
		iter++;
		return *this;
	}
	ConstFunctionIterator& ConstFunctionIterator::operator--()
	{
		--iter;
		return *this;
	}
	ConstFunctionIterator ConstFunctionIterator::operator--(int)
	{
		ConstFunctionIterator Return(*this);
		iter--;
		return *this;
	}

	ConstFunctionIterator ConstFunctionIterator::operator+(difference_type diff) const
	{
		return ConstFunctionIterator(*this) += diff;
	}
	ConstFunctionIterator& ConstFunctionIterator::operator+=(difference_type diff)
	{
		iter += diff;
		return *this;
	}
	ConstFunctionIterator ConstFunctionIterator::operator-(difference_type diff) const
	{
		return ConstFunctionIterator(*this) -= diff;
	}
	ConstFunctionIterator& ConstFunctionIterator::operator-=(difference_type diff)
	{
		iter -= diff;
		return *this;
	}

	bool ConstFunctionIterator::operator<(const ConstFunctionIterator& obj) const
	{
		return iter < obj.iter;
	}
	bool ConstFunctionIterator::operator>(const ConstFunctionIterator& obj) const
	{
		return iter > obj.iter;
	}
	bool ConstFunctionIterator::operator<=(const ConstFunctionIterator& obj) const
	{
		return iter <= obj.iter;
	}
	bool ConstFunctionIterator::operator>=(const ConstFunctionIterator& obj) const
	{
		return iter >= obj.iter;
	}

	bool ConstFunctionIterator::operator==(const ConstFunctionIterator& obj) const
	{
		return iter == obj.iter;
	}
	bool ConstFunctionIterator::operator!=(const ConstFunctionIterator& obj) const
	{
		return iter != obj.iter;
	}
}