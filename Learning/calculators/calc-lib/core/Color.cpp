#include "Color.h"

#include "Str.h"
#include <Windows.h>
#include <gdiplus.h>
#pragma comment(lib, "Gdiplus.lib")

namespace Core
{
	Color::Color() : _Value(0)
	{

	}
	Color::Color(const Byte R, const Byte G, const Byte B)
	{
		Assign(0xFF, R, G, B);
	}
	Color::Color(const Byte A, const Byte R, const Byte G, const Byte B)
	{
		Assign(A, R, G, B);
	}
	Color::Color(const Color& Other)
	{
		Assign(Other.A(), Other.R(), Other.G(), Other.B());
	}
	Color::Color(Color&& Other) noexcept
	{
		Assign(Other.A(), Other.R(), Other.G(), Other.B());
		Other._Value = 0;
	}
	Color::Color(unsigned int Other)
	{
		_Value = Other;
	}

	void Color::Assign(Byte A, Byte R, Byte G, Byte B)
	{
		_Value = static_cast<unsigned int>((A << 24) | (R << 16) | (G << 8) | B);
	}
	void Color::Assign(unsigned int New)
	{
		_Value = New;
	}

	void Color::Reduce(float By)
	{
		Assign(A(), static_cast<Byte>(R() * By), static_cast<Byte>(G() * By), static_cast<Byte>(B() * By));
	}

	Color& Color::operator=(const Color& Other)
	{
		Assign(Other.A(), Other.R(), Other.G(), Other.B());
		return *this;
	}
	Color& Color::operator=(Color&& Other) noexcept
	{
		Assign(Other.A(), Other.R(), Other.G(), Other.B());
		Other._Value = 0;

		return *this;
	}

	Color::Byte Color::A() const
	{
		return static_cast<Byte>((_Value >> 24) & 0xFF);
	}
	Color::Byte Color::R() const
	{
		return static_cast<Byte>((_Value >> 16) & 0xFF);
	}
	Color::Byte Color::G() const
	{
		return static_cast<Byte>((_Value >> 8) & 0xFF);
	}
	Color::Byte Color::B() const
	{
		return static_cast<Byte>(_Value & 0xFF);
	}

	String Color::ToString() const
	{
		return String(_Value) + L" _AS_COLOR";
	}
	String Color::ToUIString() const
	{
		return L"#"; //TODO: Write algor that converts to hex.
	}
	String Color::TypeName() const
	{
		return L"COLOR";
	}

	bool Color::OverrideFrom(BasicObject* Obj)
	{
		Color* FirstConv = dynamic_cast<Color*>(Obj);
		if (FirstConv)
		{
			*this = *FirstConv;
			return true;
		}

		String* SecondConv = dynamic_cast<String*>(Obj);
		if (SecondConv)
		{
			this->FillFromString(*SecondConv);
			return true;
		}

		return false;
	}
	void Color::FillFromString(const String& Obj)
	{
		*this = Color(Obj.ToUInt());
	}

	bool Color::HasModifyer() const { return true; }
	BasicObjectModifyer* Color::ConstructModifyer() const
	{
		//TODO: Write Color Basic Modifyer
		return nullptr;
	}

	COLORREF ColorToColorRef(const Color& Color)
	{
		return RGB(Color.R(), Color.G(), Color.B());
	}	
	Gdiplus::Color ColorToGDIPColor(const Color& Color)
	{
		return Gdiplus::Color(Color.A(), Color.R(), Color.G(), Color.B());
	}
}