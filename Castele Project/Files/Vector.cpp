#include "Component.h"

#include <fstream>
#include <math.h>
using namespace std;

void Vector2D::Fill(wifstream& InFile)
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
void Vector2D::Push(wofstream& OutFile, uint PreIndex) const
{
	String TabIndexValue;
	for (uint i = 0; i < PreIndex; i++)
		TabIndexValue += L'\t';

	String Header = TabIndexValue + L"begin~Ray";
	OutFile << Header;
	OutFile << (Root ? L"~Root:" + (String(Root->X) + L"*" + String(Root->Y)) : L"") << (Anchor ? L"~Anchor:" + (String(Anchor->X) + L"*" + String(Anchor->Y)) : L"") << L"~Stroke:" << Stroke << L"~StrokeWidth:" << StrokeWidth << L"~Fill" << ThisFill << L"~end" << endl;
}
void Vector2D::Render(HDC Dc, RECT Bounds) const
{
	HPEN Pen = CreatePen(PS_SOLID, StrokeWidth, Stroke);
	SelectObject(Dc, Pen);

	Coord* One = Root;
	Coord* Two = Anchor;

	double Angle = atan2(Two->Y - One->Y, Two->X - One->X);

	MoveToEx(Dc, One->X, One->Y, NULL);
	LineTo(Dc, Two->X + (40 * cos(Angle)), Two->Y + (40 * sin(Angle)));

	DeleteObject(Pen);
}

Vector2D* Vector2DList::Add()
{
	Vector2D* New = new Vector2D(_File);
	New->_ID = _File->GetNextID();
	New->Anchor = new Coord();
	New->Root = new Coord();

	InternalAdd(New);
	return New;
}
Vector2D* Vector2DList::Add(Vector2D*& Obj, bool Exact)
{
	Vector2D* New = new Vector2D(_File);
	New->_ID = (Exact ? Obj->ID : _File->GetNextID());
	New->Anchor = new Coord();
	New->Root = new Coord();

	memcpy(New->Anchor, Obj->Anchor, sizeof(Coord));
	memcpy(New->Root, Obj->Root, sizeof(Coord));

	InternalAdd(New);
	return New;
}
void Vector2DList::Remove(Vector2D*& Obj)
{
	Pop(Obj);

	delete Obj;
	Obj = nullptr;
}

bool Vector2DList::Contains(Vector2D*& Obj) const
{
	Vector2D* Current = First;
	for (uint i = 0; i < _Size; i++, Current = Current->Next)
	{
		if (Current == Obj)
			return true;
	}

	return false;
}
uint Vector2DList::IndexOf(Vector2D*& Obj) const
{
	Vector2D* Current = First;
	for (uint i = 0; i < _Size; i++, Current = Current->Next)
	{
		if (Current == Obj)
			return i;
	}

	return uintmax;
}

void Vector2DList::RenderAllItems(HDC Dc, RECT Bounds) const
{
	Vector2D* Current = First;
	for (uint i = 0; i < _Size; i++, Current = Current->Next)
		Current->Render(Dc, Bounds);
}
void Vector2DList::RenderItemAtIndex(uint Index, HDC Dc, RECT Bounds) const
{
	Vector2D* Item = operator[](Index);
	Item->Render(Dc, Bounds);
}

void Vector2DList::Fill(wifstream& InFile)
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
			if (ThisLine.TabIndex() == TabIndex && ThisParts[1] == L"Vector2DList" && ThisParts[0] == L"end")
				break;

			else if (ThisLine.TabIndex() - 1 == TabIndex)
			{
				InFile.seekg(PrePos);

				Vector2D* Item = new Vector2D(_File);
				Item->Fill(InFile);
				InternalAdd(Item);
			}
		}
	}
}
void Vector2DList::Push(wofstream& OutFile, uint Index) const
{
	String TabIndexValue;
	for (uint i = 0; i < Index; i++)
		TabIndexValue += L'\t';

	String Header = TabIndexValue + L"begin~Vector2DList";
	OutFile << Header;

	if (_Size != 0)
	{
		OutFile << endl;

		Vector2D* Current = First;
		for (uint i = 0; i < _Size; i++, Current = Current->Next)
			Current->Push(OutFile, Index + 1);

		OutFile << TabIndexValue << L"end~Vector2DList" << endl;
	}
	else
		OutFile << L"~end" << endl;
}