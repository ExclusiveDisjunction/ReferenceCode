#include "ConfigIns.h"

#include "Core\Str.h"

#include <Windows.h>
#include <iostream>
#include <fstream>
#include <thread>

using namespace std;

namespace Mason::Configuration
{
	using namespace Core;

	ConfigurationFile* ConfigIns::_File = nullptr;
	String ConfigIns::_BaseDirectory = String();
	String ConfigIns::_IdmediateDirectory = String();
	String ConfigIns::_ConfigPath = String();
	Mapping<String, ConfigurationSegment*> ConfigIns::_Properties;

	Mapping<String, BasicObject*> ConfigIns::DefaultSettings = Mapping<String, BasicObject*>();

	void ConfigIns::Initalize()
	{
		auto FilePath = [](String Path)
		{
			Vector<String> Parts = Path.Split(L'\\');
			unsigned int Len = Parts.Size;

			String Return = (Len == 0 ? String() : Parts[0]);
			for (unsigned int i = 1; i < Len - 1; i++)
				Return += L'\\' + Parts[i];

			return Return;
		};

		wchar_t* RawBuffer = new wchar_t[MAX_PATH];
		ZeroMemory(RawBuffer, MAX_PATH);
		GetCurrentDirectoryW(MAX_PATH, RawBuffer);

		_IdmediateDirectory = FilePath(RawBuffer) + L'\\';

		ZeroMemory(RawBuffer, MAX_PATH);
		_get_wpgmptr(&RawBuffer);
		_BaseDirectory = FilePath(RawBuffer) + L'\\';

		delete[] RawBuffer;

		String ConfigPath = _BaseDirectory + L"config\\AppInstance.masonconfig";
		_ConfigPath = ConfigPath;
		_File = new ConfigurationFile(ConfigPath);

		LoadProperties();
	}
	void ConfigIns::ShutDown()
	{
		Save();
		delete _File;
		_Properties.Clear();
		DefaultSettings.Clear();

		_BaseDirectory = _IdmediateDirectory = _ConfigPath = String();
	}

	void ConfigIns::LoadProperties()
	{
		if (!_File || !_File->Load())
			return;
		_Properties.Clear();

		for (ConfigurationSegment* Item : *_File)
			_Properties.Add(Item->Name, Item);

		for (MappingPair<String, BasicObject*>& DefProp : DefaultSettings)
		{
			if (!_Properties.Contains(DefProp.Key))
			{
				ConfigurationSegment* New = _File->Append(DefProp.Key, DefProp.Value->Clone());
				_Properties.Add(DefProp.Key, New);
			}
		}
	}
	void ConfigIns::Save()
	{
		if (!_File)
			return;

		for (MappingPair<String, BasicObject*>& DefProp : DefaultSettings)
		{
			if (!_Properties.Contains(DefProp.Key))
			{
				ConfigurationSegment* New = _File->Append(DefProp.Key, DefProp.Value->Clone());
				_Properties.Add(DefProp.Key, New);
			}
		}

		_File->Save();
	}

	BasicObject* ConfigIns::RetriveProperty(const String& Name)
	{
		bool Found = false;
		unsigned int FoundAt = 0;
		MappingPair<String, ConfigurationSegment*> Return = _Properties.Search(Name, Found, FoundAt);

		return !Found ? nullptr : Return.Value->Value;
	}

	void ConfigIns::ResetProperty(const String& Name)
	{
		BasicObject* ResetValue;
		ConfigurationSegment* ToReset;

		Mapping<String, BasicObject*>& _DefaultSettings = DefaultSettings;
		Mapping<String, ConfigurationSegment*>& Properties = _Properties;

		auto SearchDef = [_DefaultSettings, Name](BasicObject* ResetValue) -> void
		{
			for (MappingPair<String, BasicObject*>& Item : _DefaultSettings)
			{
				if (Item.Key == Name)
				{
					ResetValue = Item.Value->Clone();
					return;
				}
			}
		};
		auto SearchReset = [Properties, Name](ConfigurationSegment* ToReset) -> void
		{
			for (MappingPair<String, ConfigurationSegment*>& Item : Properties)
			{
				if (Item.Key == Name)
				{
					ToReset = Item.Value;
					return;
				}
			}
		};

		ResetValue = nullptr;
		ToReset = nullptr;
		thread ResetSearchThread = thread(SearchDef, ResetValue), ToResetThread = thread(SearchReset, ToReset);
		ResetSearchThread.join();
		ToResetThread.join();

		if (!ResetValue || !ToReset)
			return;

		delete ToReset->Value;
		ToReset->Value = ResetValue->Clone();

		delete ResetValue;

	}
}