#include "Component.h"

#include <fstream>
using namespace std;

void Ray::Fill(wifstream& InFile)
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

		if (Name == L"Root")
		{
			StringList CoordParts = Value.Split(L'*');
			if (CoordParts.Size() < 2)
				continue;

			if (Root)
				delete Root;

			Root = new Coord();
			Root->X = CoordParts[0].ToInt();
			Root->Y = CoordParts[1].ToInt();
		}
		else if (Name == L"Anchor")
		{
			StringList CoordParts = Value.Split(L'*');
			if (CoordParts.Size() < 2)
				continue;

			if (Anchor)
				delete Anchor;

			Anchor = new Coord();
			Anchor->X = CoordParts[0].ToInt();
			Anchor->Y = CoordParts[1].ToInt();
		}
		else if (Name == L"Stroke") Stroke = AaColor(Value.ToUInt());
		else if (Name == L"StrokeWidth") StrokeWidth = Value.ToUInt();
		else if (Name == L"Fll") ThisFill = AaColor(Value.ToUInt());
		else if (Name == L"ID") _ID = Value.ToLong();
	}
}
void Ray::Push(wofstream& OutFile, uint PreIndex) const
{
	String TabIndexValue;
	for (uint i = 0; i < PreIndex; i++)
		TabIndexValue += L'\t';

	String Header = TabIndexValue + L"begin~Ray";
	OutFile << Header;
	OutFile << (Root ? L"~Root:" + (String(Root->X) + L"*" + String(Root->Y)) : L"") << (Anchor ? L"~Anchor:" + (String(Anchor->X) + L"*" + String(Anchor->Y)) : L"") << L"~Stroke:" << Stroke << L"~StrokeWidth:" << StrokeWidth << L"~Fill" << ThisFill << L"~end" << endl;
}
void Ray::Render(HDC Dc, RECT Bounds) const
{
	HPEN Pen = CreatePen(PS_SOLID, StrokeWidth, Stroke);
	SelectObject(Dc, Pen);

	Coord* One = Root;
	Coord* Two = Anchor;

	if (One->X == Two->X)
	{
		MoveToEx(Dc, One->X, One->Y, NULL);
		LineTo(Dc, One->X, (One->Y < Two->Y ? Bounds.bottom : 0));
	}
	else if (One->Y == Two->Y)
	{
		MoveToEx(Dc, One->X, One->Y, NULL);
		LineTo(Dc, (One->X < Two->X ? Bounds.right : 0), One->Y);
	}
	else
	{
		double Slope = static_cast<double>(Two->Y - One->Y) / static_cast<double>(Two->X - One->X);

		double Y = static_cast<double>(One->Y);
		double X = static_cast<double>(One->X);

		double K = Y - (X * Slope); //Y Intersept
		double XI = -K / Slope; //X Intersept

		MoveToEx(Dc, One->X, One->Y, NULL);

		double LY = (Bounds.right * Slope) + K;
		double LX = Bounds.right;

		if (One->X > Two->X)
		{
			LY = K;
			LX = 0;
		}

		LineTo(Dc, static_cast<int>(LX), static_cast<int>(LY));
	}

	DeleteObject(Pen);
}

Ray* RayList::Add()
{
	Ray* New = new Ray(_File);
	New->_ID = _File->GetNextID();
	New->Anchor = new Coord();
	New->Root = new Coord();

	InternalAdd(New);
	return New;
}
Ray* RayList::Add(Ray*& Obj, bool Exact)
{
	Ray* New = new Ray(_File);
	New->_ID = (Exact ? Obj->ID : _File->GetNextID());
	New->Anchor = new Coord();
	New->Root = new Coord();

	memcpy(New->Anchor, Obj->Anchor, sizeof(Coord));
	memcpy(New->Root, Obj->Root, sizeof(Coord));

	InternalAdd(New);
	return New;
}
void RayList::Remove(Ray*& Obj)
{
	Pop(Obj);

	delete Obj;
	Obj = nullptr;
}

bool RayList::Contains(Ray*& Obj) const
{
	Ray* Current = First;
	for (uint i = 0; i < _Size; i++, Current = Current->Next)
	{
		if (Current == Obj)
			return true;
	}

	return false;
}
uint RayList::IndexOf(Ray*& Obj) const
{
	Ray* Current = First;
	for (uint i = 0; i < _Size; i++, Current = Current->Next)
	{
		if (Current == Obj)
			return i;
	}

	return uintmax;
}

void RayList::RenderAllItems(HDC Dc, RECT Bounds) const
{
	Ray* Current = First;
	for (uint i = 0; i < _Size; i++, Current = Current->Next)
		Current->Render(Dc, Bounds);
}
void RayList::RenderItemAtIndex(uint Index, HDC Dc, RECT Bounds) const
{
	Ray* Item = operator[](Index);
	Item->Render(Dc, Bounds);
}

void RayList::Fill(wifstream& InFile)
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
			if (ThisLine.TabIndex() == TabIndex && ThisParts[1] == L"RayList" && ThisParts[0] == L"end")
				break;

			else if (ThisLine.TabIndex() - 1 == TabIndex)
			{
				InFile.seekg(PrePos);

				Ray* Item = new Ray(_File);
				Item->Fill(InFile);
				InternalAdd(Item);
			}
		}
	}
}
void RayList::Push(wofstream& OutFile, uint Index) const
{
	String TabIndexValue;
	for (uint i = 0; i < Index; i++)
		TabIndexValue += L'\t';

	String Header = TabIndexValue + L"begin~RayList";
	OutFile << Header;

	if (_Size != 0)
	{
		OutFile << endl;

		Ray* Current = First;
		for (uint i = 0; i < _Size; i++, Current = Current->Next)
			Current->Push(OutFile, Index + 1);

		OutFile << TabIndexValue << L"end~RayList" << endl;
	}
	else
		OutFile << L"~end" << endl;
}