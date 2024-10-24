#pragma once

#include "Core\Str.h"

namespace Mason::Configuration
{
	class ConfigurationFile;

	class ConfigurationSegment
	{
	public:
		ConfigurationSegment() : Value(nullptr) {}
		ConfigurationSegment(const Core::String& Value) : Value(new Core::String(Value)) {}
		ConfigurationSegment(const long long& Value) : Value(new Core::Integer(Value)) {}
		ConfigurationSegment(const double& Value) : Value(new Core::Double(Value)) {}
		ConfigurationSegment(const bool& Value) : Value(new Core::Boolean(Value)) {}
		~ConfigurationSegment()
		{
			delete Value;
			Value = nullptr;
		}

		ConfigurationFile* Parent = nullptr;
		ConfigurationSegment* Next = nullptr, * Last = nullptr;

		Core::String Name;
		Core::BasicObject* Value;
		unsigned int Flags = 0;
	};
}