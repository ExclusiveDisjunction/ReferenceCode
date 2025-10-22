#pragma once

#include "AddressBook.h"
#include "Component.h"
#include "ComponentList.h"
#include "ReferenceList.h"

namespace Mason::IO
{
	class FileInstance
	{
	private:
		std::fstream* File = nullptr;
		Core::String _Path;
		bool _Loaded = false;

		Core::Vector<ReferenceListCore*> _RefCores;
		Core::Vector<ComponentListBase*> _CompLists;

		unsigned long long _CurrentID = 0;

		AddressBook* Book = nullptr;

		void ConstructFile();
	public:
		FileInstance();
		FileInstance(Core::String const& Path);
		FileInstance(const FileInstance& Obj) = delete;
		FileInstance(const FileInstance&& Obj) = delete;
		virtual ~FileInstance();

		FileInstance& operator=(const FileInstance& Obj) = delete;
		FileInstance& operator=(const FileInstance&& Obj) = delete;

		friend AddressBook;
		friend ComponentListBase;
		friend ReferenceListCore;

		Core::String Path() const;
		void Path(Core::String const& New);

		bool Save();
		bool Load();
		bool Idle();
		bool Close();

		Address ConstructAddress(unsigned int TypeID);
		Reference* SearchFor(Address ID) const;
	};
}