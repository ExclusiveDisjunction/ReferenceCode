#pragma once

#include <iostream>

/*
*
* Str.h
*
* Defines the class String, which is used to define and modify text. This is my own version, not ANSI.
*
*/

template<typename T>
class Vector;

using uint = unsigned int;

using std::wstring;
class String
{
private:
	char* Text;
	uint _Len = 0, _Compacity = 3;

	String FilterToNum(bool NegitiveNumbers, bool Decimals) const;
	void AssignByChars(const char* Chars);
	void AssignByChars(const char Char);
public:
	String();
	String(const String& Other);
	String(const char* Data);
	String(const char Data);
	String(const std::basic_string<char>& Obj);
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

	bool Add(const char& Item);
	bool Remove(const char& Item);
	bool RemoveAt(uint Index);
	bool RemoveAllOf(const char& Item);
	bool RemoveRange(uint Begin, uint End);
	void Clear();

	String SubString(uint Begin) const
	{
		return SubString(Begin, _Len - 1);
	}
	String SubString(uint Begin, uint End) const;
	const char* ConstWchar() const;

	uint Length() const;
	uint IndexOf(const char& Item) const;
	bool Contains(const char& Item) const;
	bool Contains(String Other) const;
	uint CountOf(const char& Item) const;

	char operator[](int Index) const
	{
		return operator[]((unsigned int)Index);
	}
	char operator[](uint Index) const;

	String ToLower() const;
	String ToUpper() const;

	int ToInt() const;
	uint ToUInt() const;
	long long ToLong() const;
	unsigned long long ToULong() const;
	double ToDouble() const;
	float ToFloat() const;
	bool ToBool() const
	{
		return *this == "True";
	}

	Vector<String> Split(char At) const;
	static String Combine(Vector<String> Parts, char CombineChar);
	String ReplaceChar(char Replace, char With) const;
	String ReplaceAll(char With) const;
	String ShiftEachChar(int By) const;

	bool BeginsWith(const char Value) const;
	bool BeginsWith(const String& Value) const;

	String RemoveWhiteSpace() const;
	String RemoveFromFront(char Item) const;
	String RemoveFromEnd(char Item) const;
	int TabIndex() const;

	static uint GetStrLen(const char*& Chars);

	String& operator+=(const char Two);
	String& operator+=(const String& Two);
	friend String operator+(const String& One, const char Two);
	friend String operator+(const String& One, const String& Two);
	friend String operator+(const char One, const String& Two);

	friend bool operator==(const String& One, const String& Two);
	friend bool operator!=(const String& One, const String& Two);

	friend bool operator==(const String& One, const char* Two);
	friend bool operator!=(const String& One, const char* Two);
	friend bool operator==(const char* One, const String& Two);
	friend bool operator!=(const char* One, const String& Two);

	String& operator=(const String& Obj);
	String& operator=(const char* Obj);

	operator std::string() const;
	explicit operator const char*() const;

	friend std::wostream& operator<<(std::wostream& stream, const String& string);
};

using rstr = String&;
using pstr = String*;
using StringList = Vector<String>;


std::istream& getline(std::istream& Istream, String& Str);
#ifdef _FILESYSTEM_
inline bool create_directory(const String& Path)
{
	return std::filesystem::create_directory((string)Path);
}
inline bool exists(const String& Path)
{
	return std::filesystem::exists((string)Path);
}
#endif
