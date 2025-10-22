#pragma once

#include "IOCommon.h"
#include <fstream>

namespace Core::IO
{
	class IO_API AttributesWriter
	{
	private:
		std::ostream& out;

		bool KeyIsActive = false;
	public:
		AttributesWriter(std::ostream& out);

		AttributesWriter& WriteKey(const std::string& KeyName);
		AttributesWriter& WriteKey(const std::string& KeyName, const std::string& Value);

		friend IO_API AttributesWriter& operator<<(AttributesWriter& out, const std::string& Data);
		friend IO_API AttributesWriter& operator<<(AttributesWriter& out, int obj);
		friend IO_API AttributesWriter& operator<<(AttributesWriter& out, unsigned int obj);
		friend IO_API AttributesWriter& operator<<(AttributesWriter& out, long long obj);
		friend IO_API AttributesWriter& operator<<(AttributesWriter& out, unsigned long long obj);
		friend IO_API AttributesWriter& operator<<(AttributesWriter& out, float obj);
		friend IO_API AttributesWriter& operator<<(AttributesWriter& out, double obj);
		friend IO_API AttributesWriter& operator<<(AttributesWriter& out, char obj);
		friend IO_API AttributesWriter& operator<<(AttributesWriter& out, void* obj);

		void CloseKey();
	};
}