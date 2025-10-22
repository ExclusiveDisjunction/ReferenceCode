#pragma once

#include <iostream>

#include "Address.h"
#include "..\Core\Str.h"

namespace Mason::IO
{
	class AddressBookRecord;
	class Component;
	class FileInstance;
	class Reference;
	class Sector;
	template<typename T> requires(std::is_base_of<Component, T>::value == true)
	class TypedCompListIterator;
	template<typename T> requires(std::is_base_of<Component, T>::value == true)
	class LeniarTypedCompIterator;

	using ComponentListIterator = TypedCompListIterator<Component>;
	using LeniarComponentIterator = LeniarTypedCompIterator<Component>;

	class Component
	{
	protected:
		bool _IsLoaded = false;

		FileInstance* _File;
		AddressBookRecord* Record = nullptr;
	public:
		Component() = delete;
		Component(AddressBookRecord* AddressBook, Component* Parent);
		Component(FileInstance* File, Component* Parent = nullptr); //Creates a component from scratch and adds it to the address book.
		Component(const Component& Obj) = delete;
		Component(const Component&& Obj) = delete;
		virtual ~Component();

		friend Reference;
		friend AddressBookRecord;
		friend Sector;
		friend ComponentListIterator;
		friend LeniarComponentIterator;

		FileInstance* const& ParentFile = _File;
		struct Address ID() const;

		virtual unsigned long long TypeID() const = 0;
		virtual bool ConvertType() = 0;
		virtual bool IsLoaded() const { return false; }

		virtual void Push(std::fstream& OutFile, unsigned int Index) const;
		virtual void Fill(std::fstream& InFile) = 0;

		Component& operator=(const Component& Obj) = delete;
		Component& operator=(const Component&& Obj) = delete;
	};

	class LoadedComponent : public Component
	{
	protected:
		LoadedComponent(FileInstance* File, Component* Parent);
		LoadedComponent(AddressBookRecord* Record, Component* Parent);

		bool ConvertToComponent();
	public:
		LoadedComponent() = delete;
		LoadedComponent(const LoadedComponent& Obj) = delete;
		LoadedComponent(const LoadedComponent&& Obj) = delete;
		virtual ~LoadedComponent() {}

		LoadedComponent& operator=(const LoadedComponent& Obj) = delete;
		LoadedComponent& operator=(const LoadedComponent&& Obj) = delete;

		virtual Core::String Title() const = 0;
		virtual void Title(Core::String const& Obj) = 0;
		bool IsLoaded() const override { return true; }

		virtual void Push(std::fstream& OutFile, unsigned int Index) const override = 0;
	};
}