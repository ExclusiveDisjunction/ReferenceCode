#pragma once

#include <initializer_list>

#include "Vector.h"
#include "VectorIterator.h"
#include "Types\BasicObject.h"
#include "Types\TypeRegistry.h"

namespace Core
{
	class CORE_API String;

	class CORE_API VectorB : public BasicObject, public Vector<BasicObject*>
	{
	public:
		VectorB() : Vector() {}
		VectorB(const VectorB& Other) : Vector(Other) {}
		VectorB(BasicObject** List, unsigned Len) : Vector(List, Len) {}
		VectorB(const std::initializer_list<BasicObject*>& List) : Vector(List) {}
		VectorB(const Vector<BasicObject*>& List) : Vector(List) {}
		template<typename T, typename Conversion>
		VectorB(const Vector<T>& List, Conversion Conv) : _Size(0)
		{
			for (T Item : List)
			{
				BasicObject* Converted = Conv(Item);
				this->Add(Converted);
			}
		}
		VectorB(BasicObject* Obj) : VectorB()
		{
			Add(Obj);
		}

#ifdef _VECTOR_
		VectorB(const std::vector<BasicObject*>& Other) : VectorB()
		{
			for (BasicObject* Obj : Other)
				Add(Obj);
		}
#endif

		template<typename ReturnT, typename Conversion>
		Vector<ReturnT> ConvertVector(Conversion Conv)
		{
			Vector<ReturnT> Return;
			seg Current = First;
			for (unsigned int i = 0; i < _Size; i++)
			{
				Return.Add(Conv(Current->Value));
				Current = Current->Next;
			}
		}

		String ToString() const override;
		String ToUIString() const override;
		String TypeName() const override;

		BasicObject* DefaultValue() const override;
		BasicObject* Clone() const override;
		bool OverrideFrom(BasicObject* Obj) override;

		void FillFromString(const String& Obj) override;

		bool HasModifyer() const override;
		BasicObjectModifyer* ConstructModifyer() const override;
	};
}