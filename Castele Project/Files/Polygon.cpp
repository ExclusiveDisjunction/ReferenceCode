#include "Component.h"

#include <fstream>
using namespace std;

void Polygon::Fill(wifstream& InFile)
{
	String Header;
	getline(InFile, Header);

	StringList Parts = Header.Split(L'~');
	bool Multiline = Parts[Parts.Size() - 1] != L"end";

	if (Multiline)
		return;

	for (uint i = 2; i < Parts.Size() - 1; i++)
	{
		StringList ArgPart = Parts[i].Split(L':');
		if (ArgPart.Size() < 2)
			continue;

		String Name = ArgPart[0], Value = ArgPart[1];
		for (uint j = 2; j < ArgPart.Size(); j++)
			Value += L':' + ArgPart[j];

		if (Name == L"StrokeWidth") StrokeWidth = Value.ToUInt();
		else if (Name == L"Stroke") Stroke = AaColor(Value.ToUInt());
		else if (Name == L"Fill") ThisFill = AaColor(Value.ToUInt());
		else if (Name == L"Coords")
		{
			StringList CoordParts = Value.Split(L'*');
			for (uint j = 0; j < CoordParts.Size(); j++)
			{
				StringList NumParts = CoordParts[i].Split(L',');
				if (NumParts.Size() < 2)
					continue;

				Coord* New = new Coord();
				New->X = NumParts[0].ToInt();
				New->Y = NumParts[1].ToInt();

				Coordinates.Add(New);
			}
		}
		else if (Name == L"ID") _ID = Value.ToLong();
	}
}
void Polygon::Push(wofstream& OutFile, uint PreIndex) const
{
	String TabIndexValue;
	for (uint i = 0; i < PreIndex; i++)
		TabIndexValue += L'\t';

	String Header = TabIndexValue + L"begin~Polygon";
	OutFile << Header;
	OutFile << L"~StrokeWidth:" << StrokeWidth << L"~Stroke:" << Stroke << L"~Fill:" << ThisFill << L"~Coords:";
	
	if (Coordinates.Size() > 0)
	{
		OutFile << Coordinates[0]->X << L',' << Coordinates[0]->Y;

		for (uint i = 1; i < Coordinates.Size(); i++)
			OutFile << L'*' << Coordinates[i]->X << L',' << Coordinates[i]->Y;
	}

	OutFile << L"~end" << endl;
}
void Polygon::Render(HDC Dc, RECT Bounds, TransformationMatrix* Obj) const
{
	HPEN Pen = CreatePen(PS_SOLID, StrokeWidth, Stroke);
	SelectObject(Dc, Pen);

	{
		int PointCount = static_cast<int>(Coordinates.Size());
		POINT* Items = new POINT[PointCount];
		for (int i = 0; i < PointCount; i++)
		{
			Items[i].x = Coordinates[i]->X;
			Items[i].y = Coordinates[i]->Y;
		}

		HRGN Region = CreatePolygonRgn(Items, PointCount, WINDING);

		HBRUSH Brush = CreateSolidBrush(ThisFill);
		SelectObject(Dc, Brush);

		FillRgn(Dc, Region, Brush);

		DeleteObject(Brush);
		DeleteObject(Region);
		delete[] Items;
	}

	DeleteObject(Pen);
}

class Polygon* PolygonList::Add()
{
	class Polygon* New = new class Polygon(_File);
	New->_ID = _File->GetNextID();

	InternalAdd(New);
	return New;
}
class Polygon* PolygonList::Add(class Polygon*& Obj, bool Exact)
{
	class Polygon* New = new class Polygon(_File);
	New->_ID = (Exact ? Obj->ID : _File->GetNextID());
	New->Stroke = Obj->Stroke;
	New->StrokeWidth = Obj->StrokeWidth;
	New->ThisFill = Obj->ThisFill;
	New->Coordinates.Clear();

	for (uint i = 0; i < Obj->Coordinates.Size(); i++)
	{
		Coord* Target = Obj->Coordinates[i];
		Coord* NewCoord = new Coord();
		NewCoord->X = Target->X;
		NewCoord->Y = Target->Y;

		New->Coordinates.Add(NewCoord);
	}

	InternalAdd(New);
	return New;
}
void PolygonList::Remove(class Polygon*& Obj)
{
	Pop(Obj);

	delete Obj;
	Obj = nullptr;
}

bool PolygonList::Contains(class Polygon*& Obj) const
{
	class Polygon* Current = First;
	for (uint i = 0; i < _Size; i++, Current = Current->Next)
	{
		if (Current == Obj)
			return true;
	}

	return false;
}
uint PolygonList::IndexOf(class Polygon*& Obj) const
{
	class Polygon* Current = First;
	for (uint i = 0; i < _Size; i++, Current = Current->Next)
	{
		if (Current == Obj)
			return i;
	}

	return uintmax;
}

void PolygonList::RenderAllItems(HDC Dc, RECT Bounds) const
{
	class Polygon* Current = First;
	for (uint i = 0; i < _Size; i++, Current = Current->Next)
		Current->Render(Dc, Bounds);
}
void PolygonList::RenderItemAtIndex(uint Index, HDC Dc, RECT Bounds) const
{
	class Polygon* Obj = operator[](Index);
	Obj->Render(Dc, Bounds);
}

void PolygonList::Fill(wifstream& InFile)
{
	String Header;
	getline(InFile, Header);

	StringList Parts = Header.Split(L'~');
	bool Multiline = Parts[Parts.Size() - 1] != L"end";

	if (Multiline)
	{
		int TabIndex = Header.TabIndex();

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
			if (ThisLine.TabIndex() == TabIndex && ThisParts[1] == L"PolygonList" && ThisParts[0] == L"end")
				break;

			else if (ThisLine.TabIndex() - 1 == TabIndex)
			{
				InFile.seekg(PrePos);

				class Polygon* Item = new class Polygon(_File);
				Item->Fill(InFile);
				InternalAdd(Item);
			}
		}
	}
}
void PolygonList::Push(wofstream& OutFile, uint PreIndex) const
{
	String TabIndexValue;
	for (uint i = 0; i < PreIndex; i++)
		TabIndexValue += L'\t';

	String Header = TabIndexValue + L"begin~PolygonList";
	OutFile << Header;

	if (_Size != 0)
	{
		OutFile << endl;

		class Polygon* Current = First;
		for (uint i = 0; i < _Size; i++, Current = Current->Next)
			Current->Push(OutFile, PreIndex + 1);

		OutFile << TabIndexValue << L"end~PolygonList" << endl;
	}
	else
		OutFile << L"~end" << endl;
}