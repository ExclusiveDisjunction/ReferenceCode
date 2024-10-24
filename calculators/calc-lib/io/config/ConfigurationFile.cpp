#include "ConfigurationFile.h"

#include "Core\Color.h"
#include "Core\Vector.h"
#include "Core\Mapping.h"
#include "Core\Str.h"

#include <iostream>
#include <fstream>
#include <filesystem>

using namespace std;
using namespace std::filesystem;
using namespace Core;

namespace Mason::Configuration
{
	ConfigurationFile::ConfigurationFile(const String& Path)
	{
		if (!exists(static_cast<wstring>(Path)))
		{
			_File = nullptr;
			_Path = String();
		}

		_Path = Path;
		_File = new wfstream(static_cast<wstring>(_Path), ios::in | ios::out | ios::app | ios::beg);

		Load();
	}
	ConfigurationFile::~ConfigurationFile()
	{
		for (ConfigurationSegmentChainCore* Obj : _Cores)
			Obj->Clear();
	}

	ConfigurationFileIterator ConfigurationFile::begin()
	{
		return !First ? end() : ConfigurationFileIterator(First);
	}
	ConfigurationFileIterator ConfigurationFile::end()
	{
		return ConfigurationFileIterator(!Last ? nullptr : Last->Next);
	}
	const ConfigurationFileIterator ConfigurationFile::begin() const
	{
		return !First ? end() : ConfigurationFileIterator(First);
	}
	const ConfigurationFileIterator ConfigurationFile::end() const
	{
		return ConfigurationFileIterator(!Last ? nullptr : Last->Next);
	}

	ConfigurationSegment* ConfigurationFile::Retrive(unsigned int Index) const
	{
		if (Index == 0 && Size >= 0)
			return First;
		else if (Index == (Size - 1) && Size >= 0)
			return Last;

		ConfigurationSegment* Current = First;
		for (unsigned int i = 0; i < Index; i++)
			Current = Current->Next;

		return Current;
	}
	ConfigurationSegmentChain ConfigurationFile::Retrive(const String& Name) const
	{
		Vector<ConfigurationSegment*> Objects;
		for (ConfigurationSegment* Item = First; Item != nullptr; Item = Item->Next)
		{
			if (Item->Name == Name)
				Objects.Add(Item);
		}

		return ConfigurationSegmentChain(Objects);
	}

	ConfigurationSegment* ConfigurationFile::Append(const String& Name, BasicObject* Value, unsigned int Flags)
	{
		ConfigurationSegment* New = new ConfigurationSegment();
		New->Name = Name;
		New->Value = Value;
		New->Flags = Flags;

		if (Size == 0)
		{
			First = Last = New;
			_Size++;
			return First;
		}
		else if (Size == 1)
		{
			Last = New;
			First->Next = Last;
			Last->Last = First;

			_Size++;
			return Last;
		}
	
		New->Last = Last;
		Last->Next = New;

		_Size++;
		Last = New;
		return New;
	}
	ConfigurationSegment* ConfigurationFile::Clone(const ConfigurationSegment* const& Obj)
	{
		return Append(Obj->Name, Obj->Value, Obj->Flags);
	}
	bool ConfigurationFile::Remove(const String& Name)
	{
		for (ConfigurationSegment* Current = First; Current != nullptr; Current = Current->Next)
		{
			if (Current->Name == Name)
				return Remove(Current);
		}
			
		return false;
	}
	bool ConfigurationFile::Remove(ConfigurationSegment* const& Obj)
	{
		if (Size == 0)
			return false;
		else if (Size == 1 && Obj == First)
		{
			delete First;
			First = Last = nullptr;
			_Size = 0;

			return true;
		}
		else if (Size == 2 && (Obj == First || Obj == Last))
		{
			if (Obj == First)
			{
				First = Last;
				delete Obj;

				_Size--;
			}
			else if (Obj == Last)
			{
				Last = First;
				delete Obj;

				_Size--;
			}
			else
				return false;
			
			return true;
		}
		else if (Obj == First)
		{
			First = Obj->Next;
			delete Obj;

			_Size--;
			return true;
		}
		else if (Obj == Last)
		{
			Last = Obj->Last;
			delete Obj;

			_Size--;
			return true;
		}
		else
		{
			ConfigurationSegment* Next = Obj->Next;
			ConfigurationSegment* Last = Obj->Last;

			if (Last)
				Last->Next = Next;
			Next->Last = Last;
			
			delete Obj;
			_Size--;

			return true;
		}
	}

	void ConfigurationFile::Clear()
	{
		for (ConfigurationSegmentChainCore* Obj : _Cores)
			Obj->Clear();

		if (Size == 0)
			return;

		ConfigurationSegment* Current = First, * Temp = nullptr;
		for (unsigned int i = 0; i < Size; i++)
		{
			if (!Current)
				continue;

			Temp = Current->Next;
			delete Current;
			Current = Temp;
		}

		First = Last = nullptr;
		_Size = 0;
	}
	bool ConfigurationFile::Save()
	{
		if (!_File)
		{
			if (!exists(static_cast<wstring>(_Path)))
				return false;

			_File = new wfstream();
		}

		wfstream& OutFile = *_File;
		OutFile.open(static_cast<wstring>(_Path), ios::in | ios::out | ios::trunc);

		for (ConfigurationSegment* Item : *this)
			OutFile << Item->Name << "~" << Item->Value->ToString() << "~" << Item->Flags << endl;
		
		return true;
	}
	bool ConfigurationFile::Load()
	{
		if (!_File)
		{
			if (!exists(static_cast<wstring>(_Path)))
				return false;
			_File = new wfstream(static_cast<wstring>(_Path), ios::in | ios::out | ios::app | ios::beg);
		}

		_File->seekp(0);

		wfstream& File = *_File;

		Clear();
		ConfigurationSegment* Current = nullptr, * Last = nullptr;

		while (File && !File.eof())
		{
			wstring TLine;
			getline(File, TLine);
			if (TLine == String() || TLine[0] == L'*') //Blank or Comment Line
				continue;

			String Line(TLine);

			StringList List = Line.Split('~');
			if (List.Size < 3)
				continue;

			Current = new ConfigurationSegment();
			Current->Name = List[0];
			Current->Value = TypeRegistry::ConstructFrom(List[1]);
			Current->Flags = List[2].ToUInt();

			if (Last == nullptr) //First Node
			{
				Current->Last = nullptr;
				Current->Next = nullptr;

				First = Current;
			}
			else
			{
				Last->Next = Current;
				Current->Last = Last;
			}

			Current->Next = nullptr;
			Last = Current;
			_Size++;
		}

		this->Last = Current;

		_File->seekp(0);
		return true;
	}
}