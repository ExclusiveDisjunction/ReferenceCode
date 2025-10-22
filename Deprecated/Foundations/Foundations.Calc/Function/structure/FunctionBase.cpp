#include "FunctionBase.h"

#include "FunctionIterator.h"

namespace Core::Calc::Function
{
	FunctionBase::FunctionBase(unsigned int Input, unsigned int Output) : _Input(Input), _Output(Output)
	{
		if (Input == 0 || Output == 0)
			throw std::exception("The input or output cannot be zero.");
	}
	FunctionBase::~FunctionBase()
	{
		ClearChildren();
	}

	void FunctionBase::AssignParent(FunctionBase* Obj)
	{
		if (Obj == nullptr)
			return;

		if (Obj->Parent != nullptr)
			Obj->Parent->RemoveChild(Obj, false);

		Obj->Parent = this;
		PushChild(Obj);
	}
	bool FunctionBase::PushChild(FunctionBase* Obj)
	{
		if (!Obj || Obj->_Input != _Input || Obj->_Output != _Output)
			return false;

		if (!LastCh)
		{
			FirstCh = LastCh = Obj;
			Obj->Next = nullptr;
			Obj->Prev = nullptr;
		}
		else
		{
			Obj->Prev = LastCh;
			LastCh = LastCh->Next = Obj;
		}

		AssignParent(Obj);
		CHCount++;	
		return true;
	}
	bool FunctionBase::PopChild(FunctionBase* Obj, bool Delete)
	{
		if (!Obj || Obj->Parent != this)
			return false;

		if (FirstCh == Obj && LastCh == Obj)
			FirstCh = LastCh = nullptr;
		else if (FirstCh == Obj)
			FirstCh = FirstCh->Next;
		else if (LastCh == Obj)
			LastCh = LastCh->Prev;
		else
		{
			FunctionBase* Next = Obj->Next, * Prev = Obj->Prev;
			if (Next)
				Next->Prev = Prev;
			if (Prev)
				Prev->Next = Next;
		}

		CHCount--;
		if (Delete)
			delete Obj;
		else
		{
			Obj->Next = nullptr;
			Obj->Prev = nullptr;
			Obj->Parent = nullptr;
		}
		return true;
	}
	void FunctionBase::ClearChildren()
	{
		FunctionBase* curr = FirstCh;
		while (curr)
		{
			FunctionBase* next = curr->Next;
			delete curr;
			curr = next;
		}

		FirstCh = LastCh = nullptr;
		CHCount = 0;
	}
	void FunctionBase::DetachFromParent()
	{
		if (!Parent)
			return;

		Parent->PopChild(this, false);
	}

	FunctionBase::iterator FunctionBase::ChildAt(unsigned i) noexcept
	{
		if (i >= CHCount)
			return end();

		return begin() + i;
	}
	FunctionBase::const_iterator FunctionBase::ChildAt(unsigned i) const noexcept
	{
		if (i >= CHCount)
			return end();

		return begin() + i;
	}

	FunctionBase::iterator FunctionBase::begin()
	{
		return !FirstCh ? end() : iterator(FirstCh, this);
	}
	FunctionBase::iterator FunctionBase::end()
	{
		return iterator(nullptr, this);
	}
	FunctionBase::const_iterator FunctionBase::begin() const
	{
		return !FirstCh ? end() : const_iterator(FirstCh, this);
	}
	FunctionBase::const_iterator FunctionBase::end() const
	{
		return const_iterator(nullptr, this);
	}

	unsigned FunctionBase::InputDim() const
	{
		return _Input;
	}
	unsigned FunctionBase::OutputDim() const
	{
		return _Output;
	}

	bool FunctionBase::ComparesTo(FunctionBase* const& Obj) const
	{
		if (!Obj)
			return false;

		bool Return = Obj->_Input == _Input && Obj->_Output == _Output && CHCount == Obj->CHCount;
		if (Return && CHCount)
		{
			FunctionBase* currT = FirstCh,
				* currO = Obj->FirstCh;
			for (; currT && currO; currT = currT->Next, currO = currO->Next)
			{
				Return &= currT->ComparesTo(currO);
				if (!Return)
					return false;
			
			}
		}

		return true;
	}
	bool FunctionBase::EquatesTo(FunctionBase* const& Obj) const
	{
		if (!Obj)
			return false;

		bool Return = Obj->_Input == _Input && Obj->_Output == _Output && CHCount == Obj->CHCount;
		if (Return && CHCount)
		{
			FunctionBase* currT = FirstCh,
				* currO = Obj->FirstCh;
			for (; currT && currO; currT = currT->Next, currO = currO->Next)
			{
				Return &= currT->EquatesTo(currO);
				if (!Return)
					return false;

			}
		}

		return true;
	}
	void FunctionBase::CloneBase(FunctionBase* Into) const
	{
		if (!Into || Into->_Input != _Input || Into->_Output != _Output)
			throw std::logic_error("Dimension mismatch with clone.");
		
		try
		{
			auto curr = begin(), end = this->end();
			for (; curr != end; curr++)
			{
				FunctionBase* temp = curr->Clone();
				temp->Prev = Into->LastCh;

				if (Into->LastCh)
					Into->LastCh = Into->LastCh->Next = temp;
				else //First element
					Into->FirstCh = Into->LastCh = temp;
			}
		}
		catch(std::exception& e)
		{
			delete Into;
			throw e;
		}

		Into->A = A;
		Into->CHCount = CHCount;
		Into->Next = Into->LastCh = Into->Parent = nullptr;
	}

	FunctionBase& FunctionBase::operator-()
	{
		A = -A; 
		return *this;
	}
}