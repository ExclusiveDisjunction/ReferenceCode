#pragma once

#include "Types\BasicObject.h"

namespace Core
{
	class CORE_API Color : public BasicObject
	{
	private:
		using Byte = unsigned char;

		unsigned int _Value;
	public:
		Color();
		Color(const Byte R, const Byte G, const Byte B);
		Color(const Byte A, const Byte R, const Byte G, const Byte B);
		Color(const Color& Other);
		Color(Color&& Other) noexcept;
		Color(unsigned int Other);

		void Assign(Byte A, Byte R, Byte G, Byte B);
		void Assign(unsigned int New);

		void Reduce(float By);

		Color& operator=(const Color& Obj);
		Color& operator=(Color&& Obj) noexcept;

		Byte A() const;
		Byte R() const;
		Byte G() const;
		Byte B() const;
		
		String ToString() const override;
		String ToUIString() const override;
		String TypeName() const override;

		BasicObject* DefaultValue() const override { return new Color(); }
		BasicObject* Clone() const override { return new Color(*this); }
		bool OverrideFrom(BasicObject* Obj) override;
		void FillFromString(const String& Obj) override;

		bool HasModifyer() const override;
		BasicObjectModifyer* ConstructModifyer() const override;

		operator unsigned long() const
		{
			return static_cast<unsigned long long>(R()) | static_cast<unsigned long long>(G()) << 8 | static_cast<unsigned long long>(B()) << 16;
		}
	};

#ifdef _GDIPLUS_H
	Gdiplus::Color ColorToGDIPColor(const Color& Color);
#endif
}