#include "Component.h"

#include <fstream>
using namespace std;

void Line::Fill(wifstream& InFile)
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

		if (Name == L"One")
		{
			StringList CoordParts = Value.Split(L'*');
			if (CoordParts.Size() < 2)
				continue;

			if (One)
				delete One;

			One = new Coord();
			One->X = CoordParts[0].ToInt();
			One->Y = CoordParts[1].ToInt();
		}
		else if (Name == L"Two")
		{
			StringList CoordParts = Value.Split(L'*');
			if (CoordParts.Size() < 2)
				continue;

			if (Two)
				delete Two;

			Two = new Coord();
			Two->X = CoordParts[0].ToInt();
			Two->Y = CoordParts[1].ToInt();
		}
		else if (Name == L"Stroke") Stroke = AaColor(Value.ToUInt());
		else if (Name == L"StrokeWidth") StrokeWidth = Value.ToUInt();
		else if (Name == L"Fll") ThisFill = AaColor(Value.ToUInt());
		else if (Name == L"ID") _ID = Value.ToLong();
	}
}
void Line::Push(wofstream& OutFile, uint PreIndex) const
{
	String TabIndexValue;
	for (uint i = 0; i < PreIndex; i++)
		TabIndexValue += L'\t';

	String Header = TabIndexValue + L"begin~Line";
	OutFile << Header;
	OutFile << (One ? L"~One:" + (String(One->X) + L"*" + String(One->Y)) : L"") << (Two ? L"~Two:" + (String(Two->X) + L"*" + String(Two->Y)) : L"") << L"~Stroke:" << Stroke << L"~StrokeWidth:" << StrokeWidth << L"~Fill" << ThisFill << L"~end" << endl;
}
void Line::Render(HDC Dc, RECT Bounds) const
{
	HPEN Pen = CreatePen(PS_SOLID, StrokeWidth, Stroke);
	SelectObject(Dc, Pen);

	Coord* One = this->One;
	Coord* Two = this->Two;

	if (One->X == Two->X)
	{
		MoveToEx(Dc, One->X, 0, NULL);
		LineTo(Dc, One->X, Bounds.bottom);
	}
	else if (One->Y == Two->Y)
	{
		MoveToEx(Dc, 0, One->Y, NULL);
		LineTo(Dc, Bounds.right, One->Y);
	}
	else
	{
		if (Two->X > One->X || Two->Y > One->Y)
		{
			Coord* Temp = One;
			One = Two;
			Two = Temp;
		}

		double Slope = static_cast<double>(Two->Y - One->Y) / static_cast<double>(Two->X - One->X);

		double Y = static_cast<double>(One->Y);
		double X = static_cast<double>(One->X);

		double K = Y - (X * Slope); //Y Intersept
		double XI = -K / Slope; //X Intersept

		if (XI < 0 && K < 0)
			MoveToEx(Dc, static_cast<int>(XI), static_cast<int>(K), NULL);
		else if (K > 0)
			MoveToEx(Dc, 0, static_cast<int>(K), NULL);
		else if (XI > 0)
			MoveToEx(Dc, static_cast<int>(XI), 0, NULL);

		double LY = (Bounds.right * Slope) + K;
		double LX = Bounds.right;

		LineTo(Dc, static_cast<int>(LX), static_cast<int>(LY));
	}

	DeleteObject(Pen);
}

Line* LineList::Add()
{
	Line* New = new Line(_File);
	New->_ID = _File->GetNextID();
	New->Stroke = 0xFFFFFFFF;
	New->One = new Coord();
	New->Two = new Coord();

	ComponentList::InternalAdd(New);	
	return New;
}
Line* LineList::Add(Line*& Obj, bool Exact)
{
	Line* New = new Line(_File);
	New->_ID = (Exact ? Obj->ID : _File->GetNextID());
	New->Stroke = Obj->Stroke;

	ComponentList::InternalAdd(New);
	return New;
}
void LineList::Remove(Line*& Obj)
{
	Pop(Obj);

	delete Obj;
	Obj = nullptr;
}

bool LineList::Contains(Line*& Obj) const
{
	Line* Current = First;
	for (uint i = 0; i < _Size; i++, Current = Current->Next)
	{
		if (Current == Obj)
			return true;

		Current = Current->Next;
	}
}
uint LineList::IndexOf(Line*& Obj) const
{
	Line* Current = First;
	for (uint i = 0; i < _Size; i++, Current = Current->Next)
	{
		if (Current == Obj)
			return i;
	}
}

void LineList::RenderAllItems(HDC Dc, RECT Bounds) const
{
	Line* Current = First;
	for (uint i = 0; i < _Size; i++, Current = Current->Next)
		Current->Render(Dc, Bounds);
}
void LineList::RenderItemAtIndex(uint Index, HDC Dc, RECT Bounds) const
{
	Line* Item = operator[](Index);
	Item->Render(Dc, Bounds);
}

void LineList::Fill(wifstream& InFile)
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
			if (ThisLine.TabIndex() == TabIndex && ThisParts[1] == L"LineList" && ThisParts[0] == L"end")
				break;

			else if (ThisLine.TabIndex() - 1 == TabIndex)
			{
				InFile.seekg(PrePos);

				Line* Item = new Line(_File);
				Item->Fill(InFile);
				InternalAdd(Item);
			}
		}
	}
}
void LineList::Push(wofstream& OutFile, uint PreIndex) const
{
	String TabIndexValue;
	for (uint i = 0; i < PreIndex; i++)
		TabIndexValue += L'\t';

	String Header = TabIndexValue + L"begin~LineList";
	OutFile << Header;

	if (_Size != 0)
	{
		OutFile << endl;

		Line* Current = First;
		for (uint i = 0; i < _Size; i++, Current = Current->Next)
			Current->Push(OutFile, PreIndex + 1);

		OutFile << TabIndexValue << L"end~LineList" << endl;
	}
	else
		OutFile << L"~end" << endl;
}