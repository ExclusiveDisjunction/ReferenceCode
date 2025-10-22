#pragma once

#include "Component.h"

namespace Mason::IO
{
	class Reference
	{
	private:
		Component* _Target = nullptr;
		bool _IsValid = false;
		AddressBookRecord* _Record = nullptr;
		unsigned int _RefCount = 0;

		Reference(AddressBookRecord* Record);
		Reference(Component* Record);

	public:
		Reference() = delete;
		Reference(const Reference& Obj) = delete;
		Reference(const Reference&& Obj) = delete;
		~Reference();

		friend class AddressBook;
		friend Component;
		friend FileInstance;
		friend class ReferenceList;
		friend class ReferenceListCore;
		friend class Socket;

		Reference& operator=(const Reference& Obj) = delete;
		Reference& operator=(const Reference&& Obj) = delete;

		void DecrementSelf();
		void IncrementSelf();

		operator Component* () const;
		Component* Evaluate() const;

		friend bool operator==(const Reference& A, const Reference& B);
		friend bool operator!=(const Reference& A, const Reference& B);
	};	
}