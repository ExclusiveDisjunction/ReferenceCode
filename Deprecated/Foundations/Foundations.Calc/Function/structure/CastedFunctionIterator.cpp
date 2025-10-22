#include "FunctionIterator.h"

namespace Core::Calc::Function
{
    template<typename T, typename IterT>
    CastedFunctionIterator<T, IterT>::CastedFunctionIterator(const IterT i) : iter(i) {}

    template<typename T, typename IterT>
    bool CastedFunctionIterator<T, IterT>::canDecrement() const
    {
        try
        {
            (void)dynamic_cast<T &>(*iter);
            return true; // Since the cast did not result in an exception, it is valid.
        }
        catch (...)
        {
            return false; // Either iter is not valid or the cast cannot occur.
        }
    }
    template<typename T, typename IterT>
    typename CastedFunctionIterator<T, IterT>::reference CastedFunctionIterator<T, IterT>::operator*()
    {
        try
        {
            return dynamic_cast<T &>(*iter);
        }
        catch (std::bad_cast &e)
        {
            throw std::logic_error(std::string("Typeid \'") + typeid(T).name() + "\' is not a derivable type for the current function pointer.");
        }
    }
    template<typename T, typename IterT>
    const typename CastedFunctionIterator<T, IterT>::reference CastedFunctionIterator<T, IterT>::operator*() const
    {
        try
        {
            return dynamic_cast<T &>(*iter);
        }
        catch (std::bad_cast &e)
        {
            throw std::logic_error(std::string("Typeid \'") + typeid(T).name() + "\' is not a derivable type for the current function pointer.");
        }
    }
    template<typename T, typename IterT>
    typename CastedFunctionIterator<T, IterT>::pointer CastedFunctionIterator<T, IterT>::operator->() const
    {
        try
        {
            return dynamic_cast<pointer>(iter.operator->());
        }
        catch (std::exception &e)
        {
            throw std::logic_error("Cannot decrement.");
        }
    }

    template<typename T, typename IterT>
    typename CastedFunctionIterator<T, IterT>::difference_type CastedFunctionIterator<T, IterT>::operator-(const CastedFunctionIterator<T, IterT> &obj) const
    {
        return iter - obj.iter;
    }
    template<typename T, typename IterT>
    CastedFunctionIterator<T, IterT> &CastedFunctionIterator<T, IterT>::operator[](CastedFunctionIterator<T, IterT>::difference_type diff)
    {
        iter[diff];
        return *this;
    }

    template<typename T, typename IterT>
    CastedFunctionIterator<T, IterT> &CastedFunctionIterator<T, IterT>::operator++()
    {
        ++iter;
        return *this;
    }
    template<typename T, typename IterT>
    CastedFunctionIterator<T, IterT> CastedFunctionIterator<T, IterT>::operator++(int)
    {
        CastedFunctionIterator<T, IterT> Return(*this);
        iter++;
        return *this;
    }
    template<typename T, typename IterT>
    CastedFunctionIterator<T, IterT> &CastedFunctionIterator<T, IterT>::operator--()
    {
        --iter;
        return *this;
    }
    template<typename T, typename IterT>
    CastedFunctionIterator<T, IterT> CastedFunctionIterator<T, IterT>::operator--(int)
    {
        CastedFunctionIterator<T, IterT> Return(*this);
        iter--;
        return *this;
    }

    template<typename T, typename IterT>
    CastedFunctionIterator<T, IterT> CastedFunctionIterator<T, IterT>::operator+(CastedFunctionIterator<T, IterT>::difference_type diff) const
    {
        return (CastedFunctionIterator<T, IterT>(*this) += diff);
    }
    template<typename T, typename IterT>
    CastedFunctionIterator<T, IterT> &CastedFunctionIterator<T, IterT>::operator+=(CastedFunctionIterator<T, IterT>::difference_type diff)
    {
        iter += diff;
        return *this;
    }
    template<typename T, typename IterT>
    CastedFunctionIterator<T, IterT> CastedFunctionIterator<T, IterT>::operator-(CastedFunctionIterator<T, IterT>::difference_type diff) const
    {
        return (CastedFunctionIterator<T, IterT>(*this) -= diff);
    }
    template<typename T, typename IterT>
    CastedFunctionIterator<T, IterT> &CastedFunctionIterator<T, IterT>::operator-=(CastedFunctionIterator<T, IterT>::difference_type diff)
    {
        iter -= diff;
        return *this;
    }

    template<typename T, typename IterT>
    bool CastedFunctionIterator<T, IterT>::operator<(const CastedFunctionIterator<T, IterT> &obj) const
    {
        return iter < obj.iter;
    }
    template<typename T, typename IterT>
    bool CastedFunctionIterator<T, IterT>::operator>(const CastedFunctionIterator<T, IterT> &obj) const
    {
        return iter > obj.iter;
    }
    template<typename T, typename IterT>
    bool CastedFunctionIterator<T, IterT>::operator<=(const CastedFunctionIterator<T, IterT> &obj) const
    {
        return iter <= obj.iter;
    }
    template<typename T, typename IterT>
    bool CastedFunctionIterator<T, IterT>::operator>=(const CastedFunctionIterator<T, IterT> &obj) const
    {
        return iter >= obj.iter;
    }

    template<typename T, typename IterT>
    bool CastedFunctionIterator<T, IterT>::operator==(const CastedFunctionIterator<T, IterT> &obj) const { return iter == obj.iter; }
    template<typename T, typename IterT>
    bool CastedFunctionIterator<T, IterT>::operator!=(const CastedFunctionIterator<T, IterT> &obj) const { return iter != obj.iter; }
}