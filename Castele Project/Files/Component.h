#pragma once

#include "Color.h"
#include "Container.h"
#include "Str.h"
#include "Sets.h"
#include "..\Coord.h"
#include "..\TransformationMatrix.h"

enum ComponentTypes
{
	CT_None = 0,
	CT_Line = 1,
	CT_Polygon = 2,
	CT_Ray = 4,
	CT_Vector = 8,
	CT_All = CT_Line | CT_Polygon | CT_Ray | CT_Vector
};

class SessionBase;

class Component
{
protected:
	String _Title;
	long long _ID;
	SessionBase* _File;

	Component(SessionBase* File)
	{
		_File = File;
	}
public:
	virtual String Title() const { return _Title; }
	virtual void Title(String New) { _Title = New; }
	const long long& ID = _ID;

	SessionBase* ParentFile() { return _File; }

	virtual ComponentTypes ObjectType() const = 0;
	virtual void Fill(std::wifstream& InFile) = 0;
	virtual void Push(std::wofstream& OutFile, uint PreIndex = 0u) const = 0;
};

template<typename T>
class ComponentList : public NoReplaceSet<T*>
{
protected:
	SessionBase* _File = nullptr;
	T* First, * Last;
	uint _Size;

	ComponentList(SessionBase* File)
	{
		_File = File;
	}

	void InternalAdd(T* Item)
	{
		if ((!First && !Last) || _Size == 0)
		{
			First = Last = Item;
			_Size++;

			return;
		}

		T* Last = this->Last;
		if (Last)
			Last->Next = Item;
		Item->Last = Last;

		this->Last = Item;
		_Size++;
	}
	void Pop(T* Obj)
	{
		if (_Size == 1)
		{
			this->First = this->Last = nullptr;
			_Size = 0;

			return;
		}

		T* Last = Obj->Last;
		T* Next = Obj->Next;

		if (Last)
			Last->Next = Next;
		if (Next)
			Next->Last = Last;

		if (Obj == this->Last)
			this->Last = Last;
		else if (Obj == this->First)
			this->First = Next;

		_Size--;
	}
public:
	ComponentList()
	{
		_File = nullptr;
		_Size = 0;
		First = Last = nullptr;
	}
	~ComponentList()
	{
		ClearItems();
	}

	uint Count() const override
	{
		return _Size;
	}
	virtual T* operator[](uint Index) const
	{
		if (Index >= _Size)
			return nullptr;

		if (Index == 0 && _Size >= 1)
			return First;
		else if (Index == (_Size - 1) && _Size >= 1)
			return Last;

		if (Index > (_Size / 2))
		{
			Index++;
			T* Current = Last;
			for (uint i = _Size - 2; i >= 0; i++)
				Current = Current->Last;
			return Current;
		}
		else
		{
			Index++;
			T* Current = First;
			for (uint i = 1; i < Index; i++)
				Current = Current->Next;

			return Current;
		}		
	}

	void ClearItems()
	{
		if (_Size == 0)
			return;
		else if (_Size == 1)
			delete First;
		else
		{
			T* Current = First;
			for (uint i = 0; i < _Size; i++)
			{
				T* Temp = Current->Next;
				delete Current;
				Current = Temp;
			}
		}

		_Size = 0;
		First = Last = nullptr;
	}

	virtual SessionBase* ParentFile()
	{
		return _File;
	}

	virtual T* Add() = 0;
	virtual T* Add(T*& Other, bool ExactClone) = 0;

	virtual void Fill(std::wifstream& InFile) = 0;
	virtual void Push(std::wofstream& OutFile, uint PreIndex = 0u) const = 0;

	virtual operator Vector<T*>() const
	{
		Vector<T*> Data;
		T* Current = First;
		for (uint i = 0; i < _Size; i++)
		{
			Data.Add(Current);
			Current = Current->Next;
		}

		return Data;
	}
};
class ComponentParent
{
public:
	virtual SessionBase* ParentFile() = 0;
};

class RenderItem
{
public:
	virtual void Render(HDC Dc, RECT Bounds, TransformationMatrix* Obj) const = 0;

	AaColor Stroke, ThisFill;
	uint StrokeWidth;
};
class RenderList
{
public:
	virtual void RenderAllItems(HDC Dc, RECT Bounds, TransformationMatrix* Obj) const = 0;
	virtual void RenderItemAtIndex(uint Index, HDC Dc, RECT Bounds, TransformationMatrix* Obj) const = 0;
};

class SessionBase
{
protected:
	String _Path;

public:
	SessionBase(String Path) { _Path = Path; }
	SessionBase() {}
	virtual ~SessionBase() {}

	String Title;

	const String& Path = _Path;

	virtual Component* GetFromID(long long ID, int Filter) const = 0;
	virtual long long GetNextID() = 0;

	virtual void Save() const = 0;
	virtual void Load() = 0;
	void ChangePath(String New) { _Path = New; }
};

class Line;
class LineList;
class LineParent;

class Polygon;
class PolygonList;
class PolygonParent;

class Ray;
class RayList;
class RayParent;

class Vector2D;
class Vector2DList;
class Vector2DParent;

class Line : public Component, public RenderItem
{
private:
	Line(SessionBase* File) : Component(File)
	{

	}
public:
	~Line()
	{
		delete One;
		delete Two;
	}

	friend LineList;
	Line* Next = nullptr, * Last = nullptr;

	Coord* One = nullptr;
	Coord* Two = nullptr;

	void Fill(std::wifstream& InFile) override;
	void Push(std::wofstream& OutFile, uint PreIndex = 0u) const override;
	void Render(HDC Dc, RECT Bounds, TransformationMatrix* Obj) const override;

	ComponentTypes ObjectType() const override { return CT_Line; }
};
class LineList : public ComponentList<Line>, public RenderList
{
public:
	LineList(SessionBase* File) : ComponentList(File), RenderList()
	{

	}

	Line* Add() override;
	Line* Add(Line*& Obj, bool Exact) override;
	void Remove(Line*& Obj) override;

	bool Contains(Line*& Obj) const override;
	uint IndexOf(Line*& Obj) const override;

	void RenderAllItems(HDC Dc, RECT Bounds, TransformationMatrix* Obj) const override;
	void RenderItemAtIndex(uint Index, HDC Dc, RECT Bounds, TransformationMatrix* Obj) const override;

	void Fill(std::wifstream& InFile) override;
	void Push(std::wofstream& OutFile, uint PreIndex = 0u) const override;
};
class LineParent : public ComponentParent
{
protected:
	LineList* _Lines;

public:
	LineParent()
	{
		_Lines = nullptr;
	}
	~LineParent()
	{
		delete _Lines;
		_Lines = nullptr;
	}

	virtual LineList* Lines()
	{
		return _Lines;
	}
};

class Polygon : public Component, public RenderItem
{
public:
	Polygon(SessionBase* File) : Component(File)
	{
	}
	~Polygon()
	{
		for (uint i = 0; i < Coordinates.Size(); i++)
			delete Coordinates[i];

		Coordinates.Clear();
	}

	friend PolygonList;

	Polygon* Next = nullptr, * Last = nullptr;

	Vector<Coord*> Coordinates;

	void Fill(std::wifstream& InFile) override;
	void Push(std::wofstream& OutFile, uint PreIndex = 0u) const override;
	void Render(HDC Dc, RECT Bounds, TransformationMatrix* Obj) const override;

	ComponentTypes ObjectType() const override { return CT_Polygon; }
};
class PolygonList : public ComponentList<class Polygon>, public RenderList
{
public:
	PolygonList(SessionBase* File) : ComponentList(File), RenderList()
	{

	}

	class Polygon* Add() override;
	class Polygon* Add(class Polygon*& Obj, bool Exact)  override;
	void Remove(class Polygon*& Obj) override;

	bool Contains(class Polygon*& Obj) const override;
	uint IndexOf(class Polygon*& Obj) const override;

	void RenderAllItems(HDC Dc, RECT Bounds, TransformationMatrix* Obj) const override;
	void RenderItemAtIndex(uint Index, HDC Dc, RECT Bounds, TransformationMatrix* Obj) const override;

	void Fill(std::wifstream& InFile) override;
	void Push(std::wofstream& OutFile, uint PreIndex) const override;
};
class PolygonParent : public ComponentParent
{
protected:
	PolygonList* _Polygons;
public:
	PolygonParent() : ComponentParent()
	{
		_Polygons = nullptr;
	}
	~PolygonParent()
	{
		delete _Polygons;
		_Polygons = nullptr;
	}

	virtual PolygonList* Polygons()
	{
		return _Polygons;
	}
};

class Ray : public Component, public RenderItem
{
private:
	Ray(SessionBase* File) : Component(File)
	{

	}
public:
	~Ray()
	{
		delete Root;
		delete Anchor;
	}

	friend RayList;
	Ray* Next = nullptr, * Last = nullptr;

	String Title() const override { return L""; }
	void Title(String New) override { return; }

	Coord* Root = nullptr;
	Coord* Anchor = nullptr;

	void Fill(std::wifstream& InFile) override;
	void Push(std::wofstream& OutFile, uint PreIndex = 0u) const override;
	void Render(HDC Dc, RECT Bounds, TransformationMatrix* Obj) const override;

	ComponentTypes ObjectType() const override { return CT_Ray; }
};
class RayList : public ComponentList<Ray>, public RenderList
{
public:
	RayList(SessionBase* File) : ComponentList(File), RenderList()
	{

	}

	Ray* Add() override;
	Ray* Add(Ray*& Obj, bool Exact) override;
	void Remove(Ray*& Obj) override;

	bool Contains(Ray*& Obj) const override;
	uint IndexOf(Ray*& Obj) const override;

	void RenderAllItems(HDC Dc, RECT Bounds, TransformationMatrix* Obj) const override;
	void RenderItemAtIndex(uint Index, HDC Dc, RECT Bounds, TransformationMatrix* Obj) const override;

	void Fill(std::wifstream& InFile) override;
	void Push(std::wofstream& OutFile, uint Index) const override;
};
class RayParent : public ComponentParent
{
protected:
	RayList* _Rays;
public:
	RayParent() : ComponentParent()
	{
		_Rays = nullptr;
	}
	~RayParent()
	{
		delete _Rays;
		_Rays = nullptr;
	}

	virtual RayList* Rays()
	{
		return _Rays;
	}
};

class Vector2D : public Component, public RenderItem
{
private:
	Vector2D(SessionBase* File) : Component(File)
	{

	}
public:
	~Vector2D()
	{
		delete Root;
		delete Anchor;
	}

	friend Vector2DList;
	Vector2D* Next = nullptr, * Last = nullptr;

	String Title() const override { return L""; }
	void Title(String New) override { return; }

	Coord* Root = nullptr;
	Coord* Anchor = nullptr;

	void Fill(std::wifstream& InFile) override;
	void Push(std::wofstream& OutFile, uint PreIndex = 0u) const override;
	void Render(HDC Dc, RECT Bounds, TransformationMatrix* Obj) const override;

	ComponentTypes ObjectType() const override { return CT_Ray; }
};
class Vector2DList : public ComponentList<Vector2D>, public RenderList
{
public:
	Vector2DList(SessionBase* File) : ComponentList(File), RenderList()
	{

	}

	Vector2D* Add() override;
	Vector2D* Add(Vector2D*& Obj, bool Exact) override;
	void Remove(Vector2D*& Obj) override;

	bool Contains(Vector2D*& Obj) const override;
	uint IndexOf(Vector2D*& Obj) const override;

	void RenderAllItems(HDC Dc, RECT Bounds, TransformationMatrix* Obj) const override;
	void RenderItemAtIndex(uint Index, HDC Dc, RECT Bounds, TransformationMatrix* Obj) const override;

	void Fill(std::wifstream& InFile) override;
	void Push(std::wofstream& OutFile, uint Index) const override;
};
class Vector2DParent : public ComponentParent
{
protected:
	Vector2DList* _Vectors;
public:
	Vector2DParent() : ComponentParent()
	{
		_Vectors = nullptr;
	}
	~Vector2DParent()
	{
		delete _Vectors;
		_Vectors = nullptr;
	}

	virtual Vector2DList* Vectors()
	{
		return _Vectors;
	}
};

class VectorProject : public SessionBase, public RayParent, public PolygonParent, public LineParent, public Vector2DParent
{
private:
	long long _ID = 0;

public:
	VectorProject(String Path) : SessionBase(Path)
	{
		Load();
	}
	VectorProject() : SessionBase() {}

	static VectorProject* Create(String Path);

	SessionBase* ParentFile() { return this; }

	Component* GetFromID(long long ID, int Filter) const override;
	long long GetNextID() override
	{
		_ID++;
		return _ID;
	}

	void Save() const override;
	void Load() override;
};