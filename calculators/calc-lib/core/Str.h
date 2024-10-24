#pragma once

#include <iostream>

#include "Common.h"
#include "Types\BasicObject.h"

namespace Core
{
	template<typename T>
	class Vector;

	class CORE_API String : public BasicObject
	{
	private:
		wchar_t* Text;
		unsigned int _Len = 0, _Compacity = 3;

		String FilterToNum(bool NegitiveNumbers, bool Decimals) const;
		void AssignByChars(const wchar_t* Chars);
		void AssignByChars(const wchar_t Char);
		//void AssignByChars(const char* Chars);
		void AssignByChars(const char Char);
	public:
		String();
		String(const String& Other);
		String(const wchar_t* Data);
		String(const wchar_t Data);
		String(const char Data);
		String(const std::basic_string<wchar_t>& Obj);
		String(const int& Data);
		String(const unsigned int& Data);
		String(const long long& Data);
		String(const long& Data);
		String(const unsigned long long& Data);
		String(const float& Data);
		String(const double& Data);
		~String()
		{
			Clear();
		}

		friend class AString;

		bool Add(const wchar_t& Item);
		bool Add(const char& Item);
		bool Remove(const wchar_t& Item);
		bool RemoveAt(unsigned int Index);
		bool RemoveAllOf(const wchar_t& Item);
		bool RemoveRange(unsigned int Begin, unsigned int End);
		void Clear();

		String SubString(unsigned int Begin) const
		{
			return SubString(Begin, _Len - 1);
		}
		String SubString(unsigned int Begin, unsigned int End) const;
		const wchar_t* ConstWchar() const;

		unsigned int Length() const;
		unsigned int IndexOf(const wchar_t& Item) const;
		bool Contains(const wchar_t& Item) const;
		bool Contains(String Other) const;
		unsigned int CountOf(const wchar_t& Item) const;

		wchar_t operator[](int Index) const
		{
			return operator[]((unsigned int)Index);
		}
		wchar_t operator[](unsigned int Index) const;

		String ToLower() const;
		String ToUpper() const;

		int ToInt() const;
		unsigned int ToUInt() const;
		long long ToLong() const;
		unsigned long long ToULong() const;
		double ToDouble() const;
		float ToFloat() const;
		bool ToBool() const
		{
			return *this == L"True";
		}

		Vector<String> Split(wchar_t At) const;
		static String Combine(Vector<String> Parts, wchar_t CombineChar);
		String ReplaceChar(wchar_t Replace, wchar_t With) const;
		String ReplaceAll(wchar_t With) const;
		String ShiftEachChar(int By) const;

		bool BeginsWith(const wchar_t Value) const;
		bool BeginsWith(const String& Value) const;

		String RemoveWhiteSpace() const;
		String RemoveFromFront(wchar_t Item) const;
		String RemoveFromEnd(wchar_t Item) const;
		int TabIndex() const;

		static unsigned int GetStrLen(const wchar_t*& Chars);
		static unsigned int GetStrLen(const char*& Chars);

		String& operator+=(const wchar_t Two);
		String& operator+=(const String& Two);
		friend CORE_API String operator+(const String& One, const wchar_t Two);
		friend CORE_API String operator+(const String& One, const String& Two);
		friend CORE_API String operator+(const wchar_t One, const String& Two);

		friend CORE_API bool operator==(const String& One, const String& Two);
		friend CORE_API bool operator!=(const String& One, const String& Two);

		friend CORE_API bool operator==(const String& One, const wchar_t* Two);
		friend CORE_API bool operator!=(const String& One, const wchar_t* Two);
		friend CORE_API bool operator==(const wchar_t* One, const String& Two);
		friend CORE_API bool operator!=(const wchar_t* One, const String& Two);

		String& operator=(const String& Obj);
		String& operator=(const wchar_t* Obj);

		operator std::wstring() const;
		explicit operator const wchar_t* () const;

		String ToString() const override { return *this; }
		String ToUIString() const override { return *this; }
		String TypeName() const override { return L"STRING"; }

		BasicObject* DefaultValue() const override { return new String(); }
		BasicObject* Clone() const override { return new String(*this); }
		bool OverrideFrom(BasicObject* Obj) override
		{
			String* New = dynamic_cast<String*>(Obj);
			if (!New)
				return false;

			*this = *New;
			return true;
		}

		void FillFromString(const String& Obj) override
		{
			*this = Obj;
		}

		bool HasModifyer() const override { return true; }
		BasicObjectModifyer* ConstructModifyer() const override
		{
			//TODO: Create String Basic Modifyer.
			return nullptr;
		}

		friend CORE_API std::wostream& operator<<(std::wostream& stream, const String& String);
		friend CORE_API std::wistream& operator>>(std::wistream& stream, String& Str);
	};

	using StringList = Vector<String>;
}

CORE_API std::wistream& getline(std::wistream& Istream, Core::String& Str);

#ifdef _FILESYSTEM_
CORE_API inline bool create_directory(const Core::String& Path)
{
	return std::filesystem::create_directory((std::wstring)Path);
}
CORE_API inline bool exists(const Core::String& Path)
{
	return std::filesystem::exists((std::wstring)Path);
}
#endif
