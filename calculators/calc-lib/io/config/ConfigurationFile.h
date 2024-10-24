#pragma once

#include <iostream>

#include "ConfigurationSegment.h"
#include "ConfigurationChain.h"
#include "Core\Vector.h"
#include "Core\Str.h"
#include "Core\Types\TypeRegistry.h"

namespace Mason::Configuration
{
	class ConfigurationFileIterator
	{
	private:
		ConfigurationSegment* Current = nullptr;
	public:
		using interator_category = std::bidirectional_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = ConfigurationSegment*;
		using pointer = ConfigurationSegment**;
		using reference = ConfigurationSegment*&;

		ConfigurationFileIterator(ConfigurationSegment* This);

		reference operator*();
		pointer operator->();

		ConfigurationFileIterator& operator++(int);
		ConfigurationFileIterator& operator++();

		ConfigurationFileIterator& operator--(int);
		ConfigurationFileIterator& operator--();

		friend bool operator==(const ConfigurationFileIterator& a, const ConfigurationFileIterator& b);
		friend bool operator!=(const ConfigurationFileIterator& a, const ConfigurationFileIterator& b);
	};

	class ConfigurationFile
	{
	private:
		std::wfstream* _File = nullptr;
		Core::String _Path;

		Core::Vector<ConfigurationSegmentChainCore*> _Cores;

		//bool _Active = false;

		ConfigurationSegment* First = nullptr, * Last = nullptr;
		unsigned int _Size = 0;

	public:
		ConfigurationFile(const Core::String& Path);
		ConfigurationFile(const ConfigurationFile& Obj) = delete;
		ConfigurationFile(const ConfigurationFile&& Obj) = delete;
		~ConfigurationFile();

		friend ConfigurationSegmentChainCore;

		ConfigurationFile& operator=(const ConfigurationFile& Obj) = delete;
		ConfigurationFile& operator=(const ConfigurationFile&& Obj) = delete;

		ConfigurationFileIterator begin();
		ConfigurationFileIterator end();
		const ConfigurationFileIterator begin() const;
		const ConfigurationFileIterator end() const;

		unsigned int const& Size = _Size;
		ConfigurationSegment* operator[](unsigned int Index) const { return Retrive(Index); }
		ConfigurationSegmentChain operator[](const Core::String& Name) const { return Retrive(Name); }

		ConfigurationSegment* Retrive(unsigned int Index) const;
		ConfigurationSegmentChain Retrive(const Core::String& Name) const;

		ConfigurationSegment* Append(const Core::String& Name, Core::BasicObject* Value = new Core::String(), unsigned int Flags = 0);
		ConfigurationSegment* Clone(const ConfigurationSegment* const& Obj);
		bool Remove(const Core::String& Name);
		bool Remove(ConfigurationSegment* const& Obj);

		void Clear();
		bool Save();
		bool Load();
		
		//void Idle();
		//bool Activate();
		//const bool& Active = _Active;
	};
}