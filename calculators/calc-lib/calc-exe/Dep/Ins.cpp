#include "Ins.h"

#include "Files\Settings Parsing.h"
#include "UICommon.h"
#include "Files\ProjectIO.h"
#include "Resource\resource.h"

namespace Mason
{
	namespace InsData
	{
		/// <summary>
		/// The global path for ALL Ins instances.
		/// </summary>
		String _Path;
		/// <summary>
		/// The base directory of the program
		/// </summary>
		String _BaseDir;
	}

	using namespace InsData;
	using namespace Files;

	void Ins::InitiateBase(HINSTANCE Ins, LPWSTR CmdLine)
	{
		wchar_t* Text;
		_get_wpgmptr(&Text);
		_BaseDir = FilePath(Text) + L'\\';

		_Path = _BaseDir + L"config\\ins.armin";

		if (!std::filesystem::exists(static_cast<std::filesystem::path>(_BaseDir + L"config")))
			std::filesystem::create_directory(static_cast<std::filesystem::path>(_BaseDir + L"config"));

		MasonIcon = LoadIconW(Ins, MAKEINTRESOURCE(ID_MASONICON));
		MasonCursor = LoadCursorW(NULL, IDC_ARROW);
	}
	void Ins::Shutdown()
	{
		if (LoadedProject)
		{
			delete LoadedProject;
			LoadedProject = nullptr;
		}

		_Path.Clear();
		_BaseDir.Clear();
	}

	String Ins::BaseDir()
	{
		return _BaseDir;
	}
	bool Ins::IsLoaded()
	{
		return LoadedProject != nullptr;
	}

	void Ins::LoadData()
	{
		if (!FileExists(_Path))
			return;

		SettingsParser* Parser = SettingsParser::Load(_Path);
		if (!Parser)
			return;

		SettingsItemList* Items = Parser->SettingsItems();
		if (!Items)
			return;

		LastWindowState = SW_NORMAL;
		LoadLast = false;
		LastLoaded = String();
		DevMode = false;
		LightMode = false;
		FirstTime = true;

		for (uint i = 0; i < Items->Count; i++)
		{
			SettingsItem* Current = Items->Item(i);
			String Name = Current->Name;
			if (Name == L"LastWindowState")
				LastWindowState = Current->Value.ToInt();
			else if (Name == L"LoadLast")
				LoadLast = Current->Value == L"True";
			else if (Name == L"LastLoaded")
				LastLoaded = Current->Value;
			else if (Name == L"DevMode")
				DevMode = Current->Value == L"True";
			else if (Name == L"LightMode")
				LightMode = Current->Value == L"True";
			else if (Name == L"FirstTime")
				FirstTime = Current->Value == L"True";
		}

		delete Parser;
	}
	void Ins::Save()
	{
		SettingsParser* Parser = SettingsParser::Create(_Path);
		if (!Parser)
			return;

		SettingsItemList* Items = Parser->SettingsItems();
		if (!Items)
			return;

		Items->Add(Setting, L"LastWindowState", String(int(LastWindowState)));
		Items->Add(Setting, L"LoadLast", LoadLast ? L"True" : L"False");
		Items->Add(Setting, L"LastLoaded", LastLoaded);
		Items->Add(Setting, L"DevMode", DevMode ? L"True" : L"False");
		Items->Add(Setting, L"LightMode", LightMode ? L"True" : L"False");
		Items->Add(Setting, L"FirstTime", FirstTime ? L"True" : L"False");

		Parser->Save();
		delete Parser;
	}
}