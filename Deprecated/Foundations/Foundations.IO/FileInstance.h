#pragma once

#include "IOCommon.h"
#include <fstream>

#include "Element.h"
#include "ElementList.h"
#include "Reference.h"

namespace Core::IO
{
	class IO_API FileInstance
	{
	public: //FIX
		std::fstream* fileInst = nullptr;
		std::string _ThisPath;

		Element* Root = nullptr;
		ElementIdle* RootIdle = nullptr;
		void ConstructRoot(bool DeleteIfValid = true);

		unsigned int CurrID = 0;

		bool WriteNode(std::ostream& out, Element& Root, int TabIndex);

		bool GetNodeLocation(std::istream& in, Element& Target);
		bool ReadNodeHeader(std::istream& in, Element& Target);
		bool ReadNodeChildren(std::istream& in, Element& Target);

	private:
		ElementIdle* IdleNode(const Element& Obj) const;


		void OpenStream();
	public:
		FileInstance();
		FileInstance(const std::string& Path);
		FileInstance(const FileInstance& Obj) = delete;
		FileInstance(FileInstance&& Obj) = delete;
		~FileInstance();

		FileInstance& operator=(const FileInstance& Obj) = delete;
		FileInstance& operator=(FileInstance&& Obj) = delete;

		bool WriteToFile();
		bool LoadFromFile();

		void Idle();
		bool IsIdling() const;
		void Awaken();

		Reference getRoot() const;
		unsigned int GetNextID() { return ++CurrID; }
	};
}