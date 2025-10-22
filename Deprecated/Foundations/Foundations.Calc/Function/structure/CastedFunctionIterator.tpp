#include "FunctionIterator.h"

namespace Core::Calc::Function
{
    template<typename T>
    CastedFunctionIterator<T>::CastedFunctionIterator(const FunctionIterator i) : iter(i) {}

    template<typename T>
    bool CastedFunctionIterator<T>::canDecrement() const
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
    template <typename T>
    typename CastedFunctionIterator<T>::reference CastedFunctionIterator<T>::operator*()
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
    template <typename T>
    const typename CastedFunctionIterator<T>::reference CastedFunctionIterator<T>::operator*() const
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
    template <typename T>
    typename CastedFunctionIterator<T>::pointer CastedFunctionIterator<T>::operator->() const
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

    template <typename T>
    typename CastedFunctionIterator<T>::difference_type CastedFunctionIterator<T>::operator-(const CastedFunctionIterator<T> &obj) const
    {
        return iter - obj.iter;
    }
    template <typename T>
    CastedFunctionIterator<T> &CastedFunctionIterator<T>::operator[](CastedFunctionIterator<T>::difference_type diff)
    {
        iter[diff];
        return *this;
    }

    template <typename T>
    CastedFunctionIterator<T> &CastedFunctionIterator<T>::operator++()
    {
        ++iter;
        return *this;
    }
    template <typename T>
    CastedFunctionIterator<T> CastedFunctionIterator<T>::operator++(int)
    {
        CastedFunctionIterator<T> Return(*this);
        iter++;
        return *this;
    }
    template <typename T>
    CastedFunctionIterator<T> &CastedFunctionIterator<T>::operator--()
    {
        --iter;
        return *this;
    }
    template <typename T>
    CastedFunctionIterator<T> CastedFunctionIterator<T>::operator--(int)
    {
        CastedFunctionIterator<T> Return(*this);
        iter--;
        return *this;
    }

    template <typename T>
    CastedFunctionIterator<T> CastedFunctionIterator<T>::operator+(CastedFunctionIterator<T>::difference_type diff) const
    {
        return (CastedFunctionIterator<T>(*this) += diff);
    }
    template <typename T>
    CastedFunctionIterator<T> &CastedFunctionIterator<T>::operator+=(CastedFunctionIterator<T>::difference_type diff)
    {
        iter += diff;
        return *this;
    }
    template <typename T>
    CastedFunctionIterator<T> CastedFunctionIterator<T>::operator-(CastedFunctionIterator<T>::difference_type diff) const
    {
        return (CastedFunctionIterator<T>(*this) -= diff);
    }
    template <typename T>
    CastedFunctionIterator<T> &CastedFunctionIterator<T>::operator-=(CastedFunctionIterator<T>::difference_type diff)
    {
        iter += diff;
        return *this;
    }

    template <typename T>
    bool CastedFunctionIterator<T>::operator<(const CastedFunctionIterator<T> &obj) const
    {
        return iter < obj.iter;
    }
    template <typename T>
    bool CastedFunctionIterator<T>::operator>(const CastedFunctionIterator<T> &obj) const
    {
        return iter > obj.iter;
    }
    template <typename T>
    bool CastedFunctionIterator<T>::operator<=(const CastedFunctionIterator<T> &obj) const
    {
        return iter <= obj.iter;
    }
    template <typename T>
    bool CastedFunctionIterator<T>::operator>=(const CastedFunctionIterator<T> &obj) const
    {
        return iter >= obj.iter;
    }

    template <typename T>
    bool CastedFunctionIterator<T>::operator==(const CastedFunctionIterator<T> &obj) const { return iter == obj.iter; }
    template <typename T>
    bool CastedFunctionIterator<T>::operator!=(const CastedFunctionIterator<T> &obj) const { return iter != obj.iter; }
}