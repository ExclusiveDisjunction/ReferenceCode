#include "AttributesWriter.h"

namespace Core::IO
{

	AttributesWriter::AttributesWriter(std::ostream& out) : out(out)
	{
		KeyIsActive = false;
	}

	AttributesWriter& AttributesWriter::WriteKey(const std::string& KeyName)
	{
		if (KeyIsActive)
			throw std::logic_error("ERROR: Key not closed.");

		auto iter = std::find_if(KeyName.begin(), KeyName.end(), [](char eval) -> bool
			{
				return isspace(eval);
			});
		if (iter != KeyName.end())
			throw std::logic_error("ERROR: The key contains white space, and this is not allowed.");

		out << ' ' << KeyName << ':';
		KeyIsActive = true;
		return *this;
	}
	AttributesWriter& AttributesWriter::WriteKey(const std::string& KeyName, const std::string& Value)
	{
		try
		{
			WriteKey(KeyName);
		}
		catch (std::logic_error& e)
		{
			throw e;
		}

		return *this << Value;
	}

	void AttributesWriter::CloseKey()
	{
		KeyIsActive = false;
	}

	IO_API AttributesWriter& operator<<(AttributesWriter& out, const std::string& Data)
	{
		if (!out.KeyIsActive)
			throw std::logic_error("ERROR: Key not active!");

		out.out << '"';

		for (char item : Data) //Ensures that quotes have an escape before them. This is used by the reading algorithim to differentiate between quotes that define the string & quotes that are internal to the string.
			(item == '"' ? (out.out << '\\') : out.out) << item;

		out.out << '"';
		out.CloseKey();
		return out;
	}
	IO_API AttributesWriter& operator<<(AttributesWriter& out, int obj)
	{
		if (!out.KeyIsActive)
			throw std::logic_error("ERROR: Key not active!");

		out.out << obj;
		out.CloseKey();
		return out;
	}
	IO_API AttributesWriter& operator<<(AttributesWriter& out, unsigned int obj)
	{
		if (!out.KeyIsActive)
			throw std::logic_error("ERROR: Key not active!");

		out.out << obj;
		out.CloseKey();
		return out;
	}
	IO_API AttributesWriter& operator<<(AttributesWriter& out, long long obj)
	{
		if (!out.KeyIsActive)
			throw std::logic_error("ERROR: Key not active!");

		out.out << obj;
		out.CloseKey();
		return out;
	}
	IO_API AttributesWriter& operator<<(AttributesWriter& out, unsigned long long obj)
	{
		if (!out.KeyIsActive)
			throw std::logic_error("ERROR: Key not active!");

		out.out << obj;
		out.CloseKey();
		return out;
	}
	IO_API AttributesWriter& operator<<(AttributesWriter& out, float obj)
	{
		if (!out.KeyIsActive)
			throw std::logic_error("ERROR: Key not active!");

		out.out << obj;
		out.CloseKey();
		return out;
	}
	IO_API AttributesWriter& operator<<(AttributesWriter& out, double obj)
	{
		if (!out.KeyIsActive)
			throw std::logic_error("ERROR: Key not active!");

		out.out << obj;
		out.CloseKey();
		return out;
	}
	IO_API AttributesWriter& operator<<(AttributesWriter& out, char obj)
	{
		if (!out.KeyIsActive)
			throw std::logic_error("ERROR: Key not active!");

		out.out << (char)obj;
		out.CloseKey();
		return out;
	}
	IO_API AttributesWriter& operator<<(AttributesWriter& out, void* obj)
	{
		if (!out.KeyIsActive)
			throw std::logic_error("ERROR: Key not active!");

		out.out << obj;
		out.CloseKey();
		return out;
	}
}