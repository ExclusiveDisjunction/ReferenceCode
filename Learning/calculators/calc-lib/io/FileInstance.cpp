#include "FileInstance.h"

#include <fstream>
#include <sstream>
#include <Windows.h>

using namespace std;

namespace Mason::IO
{
	using namespace Core;

	FileInstance::FileInstance()
	{
		File = nullptr;
		_Path = AString();

		_RefCores.Clear();
		_CompLists.Clear();

		_CurrentID = 0;

		Book = new AddressBook(this);

		_Loaded = true;
	}
	FileInstance::FileInstance(Core::AString const& Path)
	{
		File = new std::fstream(static_cast<string>(Path), std::ios::in | std::ios::out | std::ios::beg | std::ios::app);
		_Path = !File->is_open() ? AString() : Path;
		if (!File->is_open())
			delete File;

		_RefCores.Clear();
		_CompLists.Clear();

		_CurrentID = 0;

		Book = new AddressBook(this);

		_Loaded = true;
	}
	FileInstance::~FileInstance()
	{
		Close();

		delete Book;
		Book = nullptr;
	}

	void FileInstance::ConstructFile()
	{
		File = new std::fstream(static_cast<string>(_Path), std::ios::beg | std::ios::app | std::ios::in | std::ios::out);
	}

	Core::AString FileInstance::Path() const
	{
		return _Path;
	}
	void FileInstance::Path(Core::AString const& Path)
	{
		std::fstream* File = new std::fstream(static_cast<std::string>(Path), std::ios::in | std::ios::out | std::ios::app | std::ios::beg);
		if (!File->is_open())
		{
			delete File;
			return;
		}

		if (!this->File)
		{
			this->File = File;
			_Path = Path;

			if (!_Loaded)
				Load();

			return;
		}

		delete File;
		CopyFileA(static_cast<LPCSTR>(_Path), static_cast<LPCSTR>(Path), false);

		delete this->File;
		this->File = new std::fstream(static_cast<std::string>(Path), std::ios::in | std::ios::out | std::ios::app | std::ios::beg);
	}

	bool FileInstance::Save()
	{
		return false;
	}
	bool FileInstance::Load()
	{
		if (_Loaded)
		{
			AString Path = _Path;
			Close();

			File = new std::fstream(Path, std::ios::app | std::ios::beg | std::ios::in | std::ios::out);
			_Path = Path;
		}

		if (!File)
			ConstructFile();

		if (!File->is_open())
			return false;

		char* THeader = new char[500];
		File->read(THeader, 500);

		return true;
	}
	bool FileInstance::Idle()
	{
		return false;
	}
	bool FileInstance::Close()
	{
		delete File;
		_Path = AString();
		_CurrentID = 0;

		_Loaded = false;

		for (ReferenceListCore* Obj : _RefCores)
			delete Obj;
		for (ComponentListBase* Obj : _CompLists)
			delete Obj;
		
		Book->Clear();

		return true;
	}

	Address FileInstance::ConstructAddress(unsigned int TypeID)
	{
		_CurrentID++;
		return Address(_CurrentID, TypeID);
	}

	Reference* FileInstance::SearchFor(Address ID) const
	{
		return nullptr;
	}
}