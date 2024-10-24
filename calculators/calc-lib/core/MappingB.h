#pragma once

#include "Mapping.h"
#include "Str.h"
#include "Types\BasicObject.h"

namespace Core
{
	class MappingB : public BasicObject, public Mapping<BasicObject*, BasicObject*>
	{
	public:
		MappingB() : Mapping() {}
		MappingB(const MappingPair<BasicObject*, BasicObject*>& Obj) : Mapping(Obj) {}
		MappingB(const std::initializer_list<pair>& Obj) : Mapping(Obj) {}
		MappingB(const MappingB& Obj) = delete;
		MappingB(MappingB&& Obj) = delete;

		MappingB& operator=(const MappingB& Obj) = delete;
		MappingB& operator=(MappingB&& Obj) = delete;

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