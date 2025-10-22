#pragma once

#include "IOCommon.h"
#include "Element.h"

namespace Core::IO
{
	/// <summary>
	/// Represents the data shared by multiple instances of Reference, and is owned by an Element.
	/// </summary>
	class IO_API ReferenceCore
	{
	private:
		ReferenceCore(Element* Target) noexcept;

		int RefCount = 0;
		Element* Target = nullptr;
		bool ElementIsValid = true;

		void Bind(Element* Target) noexcept;

		void Decrement() noexcept;
		void Increment() noexcept;
	public:
		ReferenceCore(const ReferenceCore& Obj) noexcept = delete;
		ReferenceCore(ReferenceCore&& Obj) noexcept = delete;
		~ReferenceCore() noexcept;

		friend Reference;
		friend Element;

		ReferenceCore& operator=(const ReferenceCore& Obj) noexcept = delete;
		ReferenceCore& operator=(ReferenceCore&& Obj) noexcept = delete;

		void Unbind() noexcept;
	};

	/// <summary>
	/// A reference object. This object binds to a core, and will allow safe access to the 
	/// </summary>
	class IO_API Reference
	{
	private:
		ReferenceCore* TargetCore = nullptr;
	public:
		Reference(ReferenceCore* Target);
		Reference(const Reference& Obj) noexcept;
		Reference(Reference&& Obj) noexcept;
		~Reference() noexcept;

		Reference& operator=(const Reference& Obj) noexcept;
		Reference& operator=(Reference&& Obj) noexcept;

		template<typename T> requires (std::is_base_of<Element, T>::value)
		T& Retrive() const
		{
			try
			{
				Element& Target = Retrive();
				return dynamic_cast<T&>(Target);
			}
			catch (std::bad_cast& e)
			{
				throw e;
			}
			catch (std::logic_error& e)
			{
				throw e;
			}
		}
		Element& Retrive() const;
		bool IsValid() const noexcept;

		Element& operator*() const;
		Element* operator->() const;
		operator bool() const { return IsValid(); }
	};
}