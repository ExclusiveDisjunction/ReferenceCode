#pragma once

#include "ConfigurationSegment.h"

namespace Mason::Configuration
{
	class ConfigurationFile;
	class ConfigurationSegmentChain;

	struct ChainElement
	{
		ChainElement(ConfigurationSegment* Current) : Current(Current) {}

		ConfigurationSegment* Current = nullptr;
		ChainElement* Next = nullptr, * Last = nullptr;
	};

	class ChainIterator
	{
	private:
		ChainElement* Current = nullptr;

	public:
		using iterator_category = std::bidirectional_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = ConfigurationSegment*;
		using pointer = ConfigurationSegment**;
		using reference = ConfigurationSegment*&;

		ChainIterator(ChainElement* This);

		reference operator*();
		pointer operator->();

		ChainIterator& operator++(int);
		ChainIterator& operator++();

		ChainIterator& operator--(int);
		ChainIterator& operator--();

		friend bool operator==(const ChainIterator& a, const ChainIterator& b);
		friend bool operator!=(const ChainIterator& a, const ChainIterator& b);
	};

	class ConfigurationSegmentChainCore
	{
	private:
		ConfigurationSegmentChainCore() = delete;
		ConfigurationSegmentChainCore(ConfigurationFile* const& Parent);
		ConfigurationSegmentChainCore(const ConfigurationSegmentChainCore& Obj) = delete;
		ConfigurationSegmentChainCore(const ConfigurationSegmentChainCore&& Obj) = delete;
		ConfigurationSegmentChainCore(ConfigurationSegmentChainCore* const& ToClone);
		ConfigurationSegmentChainCore(const Core::Vector<ConfigurationSegment*>& Objects);
		~ConfigurationSegmentChainCore();

		friend ConfigurationFile;
		friend ConfigurationSegmentChain;

		ConfigurationSegmentChainCore& operator=(const ConfigurationSegmentChainCore& Obj) = delete;
		ConfigurationSegmentChainCore& operator=(const ConfigurationSegmentChainCore&& Obj) = delete;

		void DecrementSelf();
		void Clear();

		unsigned int Size = 0;
		ChainElement* First = nullptr, * Last = nullptr;
		ConfigurationFile* Parent = nullptr;

		unsigned int RefCount = 0;

		ChainIterator begin() { return !First ? end() : ChainIterator(First); }
		ChainIterator end() { return ChainIterator(!Last ? nullptr : Last->Next); }
		const ChainIterator begin() const { return !First ? end() : ChainIterator(First); }
		const ChainIterator end() const { return ChainIterator(!Last ? nullptr : Last->Next); }
	};

	class ConfigurationSegmentChain
	{
	private:
		ConfigurationSegmentChainCore* _Core = nullptr;

		ChainElement* Fetch(unsigned int Index) const;
		ChainElement* Fetch(const ConfigurationSegment* const& Obj) const;

		bool Pop(ChainElement* const& Obj);

		ConfigurationSegmentChain(ConfigurationSegmentChainCore* Core)
		{
			Core->RefCount++;
			_Core = Core;
		}
	public:
		ConfigurationSegmentChain() = delete;
		ConfigurationSegmentChain(const ConfigurationSegmentChain& Obj) noexcept;
		ConfigurationSegmentChain(ConfigurationSegmentChain&& Obj) noexcept;
		ConfigurationSegmentChain(const Core::Vector<ConfigurationSegment*>& Obj) noexcept;
		~ConfigurationSegmentChain();

		ConfigurationSegment* operator[](unsigned int Index) const;
		ConfigurationSegment* operator[](const Core::String& Name) const;
		unsigned int Size() const { return !_Core ? 0 : _Core->Size; }

		bool Append(ConfigurationSegment* const& Obj);
		bool Pop(ConfigurationSegment* const& Obj);
		bool Pop(unsigned int Index);
		
		unsigned int IndexOf(const ConfigurationSegment* const& Obj) const;
		bool Exists(const ConfigurationSegment* const& Obj) const;

		ConfigurationSegmentChain& operator=(const ConfigurationSegmentChain& Obj);
		ConfigurationSegmentChain& operator=(ConfigurationSegmentChain&& Obj) noexcept;

		friend bool operator==(const ConfigurationSegmentChain& One, const ConfigurationSegmentChain& Two);
		friend bool operator!=(const ConfigurationSegmentChain& One, const ConfigurationSegmentChain& Two);

		ChainIterator begin() { return !_Core ? end() : _Core->begin(); }
		ChainIterator end() { return !_Core ? ChainIterator(nullptr) : _Core->end(); }
		const ChainIterator begin() const { return !_Core ? end() : _Core->begin(); }
		const ChainIterator end() const { return !_Core ? ChainIterator(nullptr) : _Core->end(); }
	};
}