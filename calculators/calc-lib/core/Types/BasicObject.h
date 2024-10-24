#pragma once

#include "..\Common.h"

namespace Core
{
	class CORE_API String;
	class CORE_API BasicObject;

	class CORE_API BasicObjectModifyer
	{
	public:
		virtual BasicObject* Modify(BasicObject* Old) = 0;
		virtual BasicObject* Create() = 0;
		virtual void View(BasicObject* Obj) = 0;
	};

	class CORE_API BasicObject
	{
	public:
		virtual String ToString() const = 0;
		virtual String ToUIString() const = 0;
		virtual String TypeName() const = 0;

		virtual BasicObject* DefaultValue() const = 0;
		virtual BasicObject* Clone() const = 0;
		virtual bool OverrideFrom(BasicObject* Obj)= 0; 
		virtual void FillFromString(const String& Obj)= 0;

		virtual bool HasModifyer() const = 0;
		virtual BasicObjectModifyer* ConstructModifyer() const = 0;
	};

	//Finish writing simple basic objects.

	class CORE_API Integer : public BasicObject
	{
	public:
		Integer();
		Integer(long long Value);
		Integer(const Integer& Obj);
		Integer(Integer&& Obj) noexcept;

		Integer& operator=(const Integer& Obj) noexcept;
		Integer& operator=(Integer&& Obj) noexcept;
		Integer& operator=(long long Value) noexcept;

		long long Value;

		String ToString() const override;
		String ToUIString() const override;
		String TypeName() const override;

		BasicObject* DefaultValue() const override;
		BasicObject* Clone() const override { return new Integer(*this); }
		bool OverrideFrom(BasicObject* Obj) override;

		void FillFromString(const String& Obj) override;

		bool HasModifyer() const override;
		BasicObjectModifyer* ConstructModifyer() const override;

		bool operator==(const Integer& B) const { return this->Value == B.Value; }
		bool operator!=(const Integer& B) const { return !(*this == B); }

		bool operator<(const Integer& B) const { return this->Value < B.Value; }
		bool operator<=(const Integer& B) const { return this->Value <= B.Value; }
		bool operator>(const Integer& B) const { return this->Value > B.Value; }
		bool operator>=(const Integer& B) const { return this->Value >= B.Value; }
		
		Integer operator+(const Integer& B) { return Integer(this->Value + B.Value); }
		Integer& operator+=(const Integer& B) { this->Value += B.Value; return *this; }
		Integer operator-(const Integer& B) { return Integer(this->Value - B.Value); }
		Integer& operator-=(const Integer & B) { this->Value -= B.Value; return *this; }
		Integer operator*(const Integer& B) { return Integer(this->Value * B.Value); }
		Integer& operator*=(const Integer& B) { this->Value *= B.Value; return *this; }
		Integer operator/(const Integer& B) { return Integer(this->Value / B.Value); }
		Integer& operator/=(const Integer& B) { this->Value /= B.Value; return *this; }
	};

	class CORE_API Double : public BasicObject
	{
	public:
		Double();
		Double(double Value);
		Double(const Double& Obj);
		Double(Double&& Obj) noexcept;

		Double& operator=(const Double& Obj) noexcept;
		Double& operator=(Double&& Obj) noexcept;
		Double& operator=(double Value) noexcept;

		double Value;

		String ToString() const override;
		String ToUIString() const override;
		String TypeName() const override;

		BasicObject* DefaultValue() const override;
		BasicObject* Clone() const override { return new Double(*this); }
		bool OverrideFrom(BasicObject* Obj) override;

		void FillFromString(const String& Obj) override;

		bool HasModifyer() const override;
		BasicObjectModifyer* ConstructModifyer() const override;

		bool operator==(const Double& B) const { return this->Value == B.Value; }
		bool operator!=(const Double& B) const { return !(*this == B); }

		bool operator<(const Double& B) const { return this->Value < B.Value; }
		bool operator<=(const Double& B) const { return this->Value <= B.Value; }
		bool operator>(const Double& B) const { return this->Value > B.Value; }
		bool operator>=(const Double& B) const { return this->Value >= B.Value; }

		Double operator+(const Double& B) { return Double(this->Value + B.Value); }
		Double& operator+=(const Double& B) { this->Value += B.Value; return *this; }
		Double operator-(const Double& B) { return Double(this->Value - B.Value); }
		Double& operator-=(const Double& B) { this->Value -= B.Value; return *this; }
		Double operator*(const Double& B) { return Double(this->Value * B.Value); }
		Double& operator*=(const Double& B) { this->Value *= B.Value; return *this; }
		Double operator/(const Double& B) { return Double(this->Value / B.Value); }
		Double& operator/=(const Double& B) { this->Value /= B.Value; return *this; }
	};
	class CORE_API Boolean : public BasicObject
	{
	public:
		Boolean();
		Boolean(bool Value);
		Boolean(const Boolean& Obj);
		Boolean(Boolean&& Obj) noexcept;

		Boolean& operator=(const Boolean& Obj) noexcept;
		Boolean& operator=(Boolean&& Obj) noexcept;
		Boolean& operator=(bool Value) noexcept;

		bool Value;

		String ToString() const override;
		String ToUIString() const override;
		String TypeName() const override;

		BasicObject* DefaultValue() const override;
		BasicObject* Clone() const override { return new Boolean(*this); }
		bool OverrideFrom(BasicObject* Obj) override;

		void FillFromString(const String& Obj) override;

		bool HasModifyer() const override;
		BasicObjectModifyer* ConstructModifyer() const override;

		bool operator==(const Boolean& B) const { return this->Value == B.Value; }
		bool operator!=(const Boolean& B) const { return !(*this == B); }
	};
}