#pragma once

#include "BasicObject.h"

namespace Core
{
	template<typename TKey, typename TValue>
	class Mapping;

	class CORE_API TypeRegistry
	{
	public:
		enum ErrorType
		{
			ERR_TypeNameNonUnique = 0,
			ERR_TypeSignatureNull = 2,
			ERR_TypeNameNotFound = 4,
			ERR_None = 8
		};

	private:
		TypeRegistry() = delete;
		TypeRegistry(const TypeRegistry& Obj) = delete;
		TypeRegistry(const TypeRegistry&& Obj) = delete;
		~TypeRegistry() = delete;

		TypeRegistry& operator=(const TypeRegistry& Obj) = delete;
		TypeRegistry& operator=(const TypeRegistry&& Obj) = delete;

		static Mapping<String, BasicObject*> Objects;
		static ErrorType LastError;

	public:

		static void Initiate();
		static void ShutDown();

		static bool AppendType(BasicObject* Signature);
		static bool PopType(const String& Name);

		static bool IsRegistered(BasicObject* Signature);
		static bool IsRegistered(const String& TypeName);
		static BasicObject* Retrive(const String& Name);

		static BasicObject* ConstructFrom(const String& Literal);
	};
}