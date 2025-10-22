#include "FileInstance.h"
#include <filesystem>

using namespace std;

namespace Core::IO
{
	FileInstance::FileInstance()
	{
		ConstructRoot();
	}
	FileInstance::FileInstance(const std::string& Path) : FileInstance()
	{
		_ThisPath = Path;
		OpenStream();
	}
	FileInstance::~FileInstance()
	{
		if (fileInst)
		{
			fileInst->close();
			delete fileInst;
		}

		if (Root)
			delete Root;
		if (RootIdle)
			delete RootIdle;

		_ThisPath = std::string();
	}

	void FileInstance::ConstructRoot(bool Delete)
	{
		if (Delete && Root)
			delete Root;

		Root = new Element(this, nullptr, true);
		Root->TypeID = 1; //ROOT_ID
	}

	Reference FileInstance::getRoot() const
	{
		return !Root ? Reference(nullptr) : Root->getReference();
	}

	void FileInstance::OpenStream()
	{
		fileInst = new std::fstream(_ThisPath, std::ios::app | std::ios::beg | std::ios::in | std::ios::out);

		if (fileInst->bad())
		{
			fileInst->close();
			delete fileInst;
			_ThisPath = std::string();
		}
	}

	bool FileInstance::WriteToFile()
	{
		std::ofstream outFile(_ThisPath + ".temp", std::ios::trunc);
		if (!outFile)
			return false;

		outFile << "DECLARE ElementIO V:1 !!DECLARE" << endl;

		outFile << "SECTION TYPELIST" << endl
			<< "!!SECTION TYPELIST" << endl;

		outFile << "SECTION CONFIGURATION" << endl
			<< "!!SECTION CONFIGURATION" << endl;

		outFile << "SECTION BLOBS" << endl
			<< "!!SECTION BLOBS" << endl;

		outFile << "SECTION ELEMENTS" << endl;
		bool Return = WriteNode(outFile, *Root, 1);
		outFile << "!!SECTION ELEMENTS" << endl;

		//Release handles
		outFile.close();

		if (Return)
		{
			if (std::filesystem::exists(_ThisPath))
			{
				if (fileInst)
				{
					fileInst->close();
					delete fileInst;
					fileInst = nullptr;
				}

				std::filesystem::remove(_ThisPath);
				std::filesystem::copy_file(_ThisPath + ".temp", _ThisPath); //Copy file over
				OpenStream(); //Opens the file again.

				std::filesystem::remove(_ThisPath + ".temp");
			}
			else
				std::filesystem::rename(_ThisPath + ".temp", _ThisPath);
		}
		else
			std::filesystem::remove(_ThisPath + ".temp");		

		return Return;
	}
	bool FileInstance::WriteNode(std::ostream& out, Element& Target, int TabIndex)
	{
		std::string TabIndexStr;
		for (int i = 0; i < TabIndex; i++)
			TabIndexStr += '\t';

		out << TabIndexStr;
		Target.Begin = out.tellp();
		out << "ELEMENT " << Target.ID << ' ' << Target.TypeID;

		//Atributes
		{
			/*
			* The node must be 'scratch-written' IF
			* 1. The node is modified
			* 2. The node's header locations are not known
			* 3. The fileinstance of the previous load is not valid.
			* 
			* The node can be copied IF
			* 1. The last three critera are all false
			* 2. The node is not loaded AND the header locations are known AND the fileinstance of the previous load is valid.
			*/

			bool HeaderLocIsKnown = Target.Begin != 0 && Target.HEnd != 0,
				PrevFileExists = fileInst != nullptr;

			bool scratchWrite = (Target.State & ElementState::ES_Modified) || !HeaderLocIsKnown || !PrevFileExists;
			bool copy = (!(Target.State & ES_Loaded) && HeaderLocIsKnown && PrevFileExists) || !scratchWrite;

			if (scratchWrite)
			{
				Target.AttrBegin = out.tellp();
				AttributesWriter wr(out); //Export attributes.
				try
				{
					Target.WriteAttributes(wr);
					Target.State &= ~ES_Modified; //Removes the modified state.
				}
				catch (...) //If an element writes an invalid key, it could crash the program. The functions are supposed to throw any errors that AttributesWriter throws, so this will catch it and return false. 
				{
					return false;
				}

				Target.HEnd = out.tellp();
				out << ' ';
			}
			else if (copy)
			{
				std::streampos BegTemp = out.tellp();
				
				//In this, we will copy the attributes from the previously written Element.
				std::fstream& prev = *(this->fileInst);

				//Move to the attributes begin location, copy the data.
				prev.seekg(Target.AttrBegin);
				size_t ArrSize = Target.HEnd - Target.AttrBegin + 1;
				char* Data = new char[ArrSize];
				memset(Data, 0, ArrSize);

				prev.read(Data, ArrSize - 1);

				//Take the copied data and write it to the new stream.
				out.write(Data, ArrSize-1);
				delete[] Data;

				Target.AttrBegin = BegTemp;
				Target.HEnd = out.tellp();
				out << ' ';
			}		
			else
				return false; //Dont know what to do :(. Shouldnt ever happen. 
		}		

		if (Target.HasChildren())
		{
			out << '\n';
			//Output each child with a tab index one greater.

			try
			{
				ElementList& Children = Target.getChildren();
				for (Element& Child : Children)
				{
					if (!WriteNode(out, Child, TabIndex + 1))
						return false;
				}
			}
			catch (...)
			{
				return false;
			}

			out << TabIndexStr << "!!ELEMENT " << Target.ID;
			Target.End = out.tellp();
			out << endl;
		}
		else
		{
			out << "!!ELEMENT " << Target.ID;
			Target.End = out.tellp();

			out << endl;
		}

		return true;
	}
	bool FileInstance::LoadFromFile()
	{
		return false;
	}

	bool FileInstance::GetNodeLocation(std::istream& in, Element& Target)
	{
		if (!in || Target.Begin == 0)
			return false;

		Target.AttrBegin = 0;
		Target.HEnd = 0;
		Target.End = 0;

		/*
		* In the header line, we expect three things. 
		* 1. ELEMENT
		* 2. [ID]
		* 3. [TypeID]
		* 
		* Essentially, this is three strings, seperated by spaces.
		* 
		* ELEMENT [ID] [TypeID] [[AttrName]:[AttrValue]...] !!ELEMENT [ID]
		* 
		* OR
		* 
		* ELEMENT [ID] [TypeID] [[AttrName]:[AttrValue]...] 
		*	...ELEMENT [ID != this->ID]
		* !!ELEMENT [ID]
		*/

		in.seekg(Target.Begin);
		std::string ElementTag;
		unsigned int ID;
		unsigned int TypeID;
		in >> ElementTag >> ID >> TypeID;

		if (!in || ElementTag != "ELEMENT")
			return false;

		Target.ID = ID;
		Target.TypeID = TypeID;

		Target.AttrBegin = in.tellg();
		std::string Curr;
		std::streamoff CurrLoc;
		bool IsRunning = true;
		do
		{
			CurrLoc = in.tellg();
			in >> Curr;

			if (!in)
				return false;

			if (Curr == "!!ELEMENT")
			{
				//We need to ensure that the ID following the closing tag matches.
				in >> ID;
				if (in && ID == Target.ID)
				{
					Target.HEnd = CurrLoc;
					Target.End = in.tellg();
					return true;
				}
				else
					return false; //This is another element, so it is valid format.
			}
			else if (Curr == "ELEMENT")
			{
				//We need to ensure that the ID following the tag is different to mark it as another node.
				in >> ID;
				if (in && ID != Target.ID)
				{
					Target.HEnd = CurrLoc;
					return true;
				}
				else
					return false; //Invalid format, exit function.
			}
		} while (IsRunning);
	}
	bool FileInstance::ReadNodeHeader(std::istream& in, Element& Target)
	{
		if (!in || Target.AttrBegin == 0 || Target.HEnd == 0)
			return false;

		size_t BlockSize = Target.HEnd - Target.AttrBegin + 1;
		char* Block = new char[BlockSize];
		memset(Block, 0, BlockSize);

		in.read(Block, BlockSize - 1); //Get the data out of the stream.

		std::map<std::string, std::string> Attributes;
		std::string key, value;
		bool IsWritingValues = false,
			IsInQuotes = false,
			IsKey = false,
			IsError = false,
			IsSlashBeforeQuote = false;
		for (size_t i = 0; i < BlockSize; i++)
		{
			char current = Block[i];
			switch (current)
			{
			case ' ':
			{
				if (IsWritingValues && !IsInQuotes)
				{
					Attributes[key] = value;
					IsWritingValues = false;
				}

				IsSlashBeforeQuote = false;
				continue;
			}
			case ':':
			{
				if (IsKey) //Invalid format!
				{
					IsError = true;
					break;
				}
				else if (IsInQuotes) //Skip
					continue;
				else
					IsKey = false;

				IsSlashBeforeQuote = false;
				continue;
			}
			case '\\':
			{
				IsSlashBeforeQuote = true;
				continue;
			}
			case '\"':
			{
				if (IsInQuotes)
				{
					if (IsSlashBeforeQuote)
					{
						value += current;
						IsSlashBeforeQuote = false;
						continue;
					}
					else //Finish the value
					{
						IsWritingValues = false;
						IsKey = false;
						IsSlashBeforeQuote = false;
						continue;
					}
				}
				else
				{
					if (IsKey || IsWritingValues) //Invalid format.
					{
						IsError = true;
						break;
					}

					IsWritingValues = true;
					IsInQuotes = true;
				}

				IsSlashBeforeQuote = false;
				continue;
			}
			default:
				(IsKey ? key : value) += current;
				IsWritingValues = true;
				IsSlashBeforeQuote = false;
				continue;
			}

			if (IsError)
			{
				delete[] Block;
				return false;
			}
		}

		try
		{
			Target.ReadAttributes(Attributes);
		}
		catch (...)
		{
			return false;
		}

		return true;
	}
	bool FileInstance::ReadNodeChildren(std::istream& in, Element& Target)
	{
		return false;
	}

	void FileInstance::Idle()
	{
		if (IsIdling())
			return;

		RootIdle = IdleNode(*Root);
		delete Root;
		Root = nullptr;
	}
	ElementIdle* FileInstance::IdleNode(const Element& Obj) const
	{ 
		ElementIdle* Return = new ElementIdle();
		Return->Begin = Obj.Begin;
		Return->End = Obj.End;
		Return->AttrBegin = Obj.AttrBegin;
		Return->HEnd = Obj.HEnd;

		if (Obj.HasChildren())
		{
			const ElementList& Children = Obj.getChildren();
			ElementIdle* Last = nullptr;
			for (const Element& Child : Children)
			{
				ElementIdle* Result = IdleNode(Child);
				if (Return->FirstChild == nullptr)
					Return->FirstChild = Result;

				if (Last)
					Last->Next = Result;
				Last = Result;
			}
		}

		return Return;
	}
	bool FileInstance::IsIdling() const
	{
		return RootIdle != nullptr;
	}
	void FileInstance::Awaken()
	{

	}
}