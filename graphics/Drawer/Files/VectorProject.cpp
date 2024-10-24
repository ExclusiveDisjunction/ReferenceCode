#include "Component.h"

#include <fstream>
using namespace std;

Component* VectorProject::GetFromID(long long ID, int Filter) const
{
	Component* Return = nullptr;

	if (_Lines)
	{
		for (uint i = 0; i < _Lines->Count(); i++)
		{
			Line* Current = _Lines->Item(i);
			if (Current->ID == ID)
				Return = Current;
		}
	}
	
	if (_Rays)
	{
		for (uint i = 0; i < _Rays->Count(); i++)
		{
			Ray* Current = _Rays->Item(i);
			if (Current->ID == ID)
				Return = Current;
		}
	}

	if (_Polygons)
	{
		for (uint i = 0; i < _Polygons->Count(); i++)
		{
			class Polygon* Current = _Polygons->Item(i);
			if (Current->ID == ID)
				Return = Current;
		}
	}

	if (_Vectors)
	{
		for (uint i = 0; i < _Vectors->Count(); i++)
		{
			Vector2D* Current = _Vectors->Item(i);
			if (Current->ID == ID)
				Return = Current;
		}
	}

	if (Return && (Filter & Return->ObjectType()))
		return Return;
	else
		return nullptr;
}

void VectorProject::Save() const
{
	wofstream OutFile(_Path);
	if (!OutFile)
		return;

	OutFile << L"begin~VectorProject~ID:" << _ID << endl;
	if (_Rays)
		_Rays->Push(OutFile, 1);
	if (_Polygons)
		_Polygons->Push(OutFile, 1);
	if (_Lines)
		_Lines->Push(OutFile, 1);
	if (_Vectors)
		_Vectors->Push(OutFile, 1);
	OutFile << L"end~VectorProject";

	OutFile.close();
}
void VectorProject::Load()
{
	wifstream InFile(_Path);

	String Header;
	getline(InFile, Header);

	StringList Parts = Header.Split(L'~');
	bool Multiline = Parts[Parts.Size() - 1] != L"end";

	_ID = Parts[2].ToLong();

	_Rays = new RayList(this);
	_Polygons = new PolygonList(this);
	_Lines = new LineList(this);
	_Vectors = new Vector2DList(this);

	if (Multiline)
	{
		while (!InFile.eof())
		{
			String ThisLine;
			streampos PrePos = InFile.tellg();
			getline(InFile, ThisLine);

			String Temp = ThisLine;
			Temp.RemoveAllOf(L'\t');
			if (Temp == L"")
				continue;

			StringList ThisParts = Temp.Split(L'~');
			if (ThisLine.TabIndex() == 0 && ThisParts[1] == L"VectorProject" && ThisParts[0] == L"end")
				break;

			else if (ThisLine.TabIndex() - 1 == 0)
			{
				streampos ThisPos = InFile.tellg();
				if (Parts[0] != L"begin")
					continue;

				InFile.seekg(PrePos);

				if (ThisParts[1] == L"LineList")
					_Lines->Fill(InFile);
				else if (ThisParts[1] == L"PolygonList")
					_Polygons->Fill(InFile);
				else if (ThisParts[1] == L"RayList")
					_Rays->Fill(InFile);
				else if (ThisParts[1] == L"Vector2DList")
					_Vectors->Fill(InFile);
			}
		}
	}
}

VectorProject* VectorProject::Create(String Path)
{
	wofstream OutFile(Path);
	if (!OutFile)
		return nullptr;

	OutFile << L"begin~VectorProject~ID:0" << endl;
	OutFile << L"\tbegin~LineList~end" << endl;
	OutFile << L"\tbegin~PolygonList~end" << endl;
	OutFile << L"\tbegin~RayList~end" << endl;
	OutFile << L"\tbegin~Vector2DList~end" << endl;
	OutFile << L"end~VectorProject";

	OutFile.close();

	return new VectorProject(Path);
}