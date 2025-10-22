#include "ComponentViewer.h"

#include "UI\Controls.h"
#include "..\Editors\EditorFrame.h"
#include "..\Editors\EditorRegistry.h"
#include "..\Files\ProjectIO.h"
#include "..\UI\Tool\ComponentListRenderer.h"

namespace Mason::UI
{
	using namespace Editors;
	using namespace Files;
	using namespace UI::Tool;

	ComponentViewerList::ComponentViewerList(Grid* Viewer, ScrollViewer* Scroller)
	{
		First = Last = nullptr;
		_Size = 0;

		this->Viewer = Viewer;
		this->Scroller = Scroller;
	}
	ComponentViewerList::~ComponentViewerList()
	{
		Clear();
	}

	ComponentViewer* ComponentViewerList::operator[](uint Index) const
	{
		if (Index >= _Size)
			return nullptr;

		if (Index == 0 && Size >= 1)
			return First;
		else if (Index == (Size - 1) && Size >= 1)
			return Last;

		Index++;
		ComponentViewer* Current = First;
		for (uint i = 1; i < Index; i++)
			Current = Current->Next;
		
		return Current;
	}

	void ComponentViewerList::Append(ComponentViewer* Item)
	{
		if ((!First && !Last) || Size == 0)
		{
			First = Last = Item;
			_Size++;

			return;
		}

		ComponentViewer* Last = this->Last;

		Item->Last = Last;

		if (_Size == 0)
			First = this->Last = Item;
		else
			Last->Next = Item;

		_Size++;
		this->Last = Item;
	}
	void ComponentViewerList::Pop(ComponentViewer* Obj)
	{
		if (Size == 1)
		{
			this->First = this->Last = nullptr;
			_Size = 0;

			return;
		}

		ComponentViewer*& Last = Obj->Last;
		ComponentViewer*& Next = Obj->Next;

		if (Last)
			Last->Next = Next;
		if (Next)
			Next->Last = Last;

		if (Obj == this->Last)
			this->Last = Last;
		else if (Obj == this->First)
			this->First = Next;

		Next = Last = nullptr;
		_Size--;
	}
	void ComponentViewerList::Clear()
	{
		if (Size == 0)
			return;

		IsDeleting = true;

		if (Size == 1)
		{
			delete First;
			First = Last = nullptr;
			_Size = 0;
		}
		else if (Size == 2)
		{
			delete First;
			delete Last;
			First = Last = nullptr;
			_Size = 0;
		}
		else
		{
			ComponentViewer* Current = First;
			for (int i = 0; i < Size; i++)
			{
				ComponentViewer* Temp = Current->Last;
				delete Current;
				Current = Temp;
			}

			_Size = 0;
			First = Last = nullptr;
		}

		IsDeleting = false;
	}

	Vector<Component*> ComponentViewerList::RetriveFromList() const
	{
		Vector<Component*> Return;
		ComponentViewer* This = First;
		for (uint i = 0; i < Size; i++, This = This->Next)
		{
			if (This->CheckState())
				Return.Add(This->Target());
		}

		return Return;
	}
	Vector<Component*> ComponentViewerList::RetriveFromList(Vector<ComponentViewer*>& Selected) const
	{
		Selected.Clear();

		Vector<Component*> Return;
		for (ComponentViewer* This = First; This != nullptr; This = This->Next)
		{
			if (This->CheckState())
			{
				Return.Add(This->Target());
				Selected.Add(This);
			}
		}

		return Return;
	}
	Vector<Component*> ComponentViewerList::GetAllComponents() const
	{
		Vector<Component*> Return;
		for (ComponentViewer* Item = First; Item != nullptr; Item = Item->Next)
			Return.Add(Item->Target());

		return Return;
	}
	void ComponentViewerList::GenerateListBase(const Vector<Component*>& Objects,HMENU ID, bool Multiselect, bool CanEdit, int XCoord, int YCoord, int Width, int Height, int Spacing)
	{
		HINSTANCE ins = reinterpret_cast<HINSTANCE>(GetWindowLongPtrW(*Viewer, GWLP_HINSTANCE));
		Clear();

		if (Scroller)
		{
			RECT WndRect;
			GetClientRect(Scroller, &WndRect);
			Width = max(Width, WndRect.right - 10);

			Viewer->Move(0, 0, Width + 10, static_cast<int>((Height + Spacing) * (Objects.Size + 1)));
			Scroller->Reset();
		}

		for (uint i = 0; i < Objects.Size; i++, YCoord += Spacing + Height)
			new ComponentViewer(XCoord, YCoord, Width, Height, Viewer, ins, ID, Objects[i], Multiselect, this, CanEdit);
	}
	void ComponentViewerList::GenerateListRef(const Vector<Files::ComponentReference*>& Objects, HMENU ID, bool Multiselect, bool CanEdit, int XCoord, int YCoord, int Width, int Height, int Spacing)
	{
		Vector<Component*> Components;
		for (ComponentReference* Obj : Objects)
		{
			Component* Current = Obj->operator Component * ();
			if (Current)
				Components.Add(Current);
		}

		GenerateListBase(Components, ID, Multiselect, CanEdit, XCoord, YCoord, Width, Height, Spacing);
	}

	void ComponentViewerList::ReSizeList(int XCoord, int YCoord, int Width, int Height, int Spacing)
	{
		if (Viewer && Scroller)
		{
			RECT WndRect;
			GetClientRect(Scroller, &WndRect);
			Width = max(Width, WndRect.right - 10);

			Viewer->Move(0, 0, Width + 10, static_cast<int>((Height + Spacing) * (Size + 1)));
			Scroller->Reset();
		}

		for (ComponentViewer* Current = First; Current != nullptr; Current = Current->Next, YCoord += Height + Spacing)
			Current->Move(XCoord, YCoord, Width, Height);
	}
	void ComponentViewerList::OpenSelectedForEditView(bool EditMode)
	{
		if (EditMode)
		{
			Vector<Component*> Items = RetriveFromList();
			for (uint i = 0; i < Items.Size; i++)
			{
				Component* Current = Items[i];
				bool Open = false;
				switch (Current->ObjectType())
				{
				default:
					Open = false;
					break;
				}

				if (Open)
				{
					ComponentTypes Type = Current->ObjectType();
	
				}
			}
		}
		else
		{
			Vector<Component*> Items = RetriveFromList();
			for (uint i = 0; i < Items.Size; i++)
			{
				Component* Current = Items[i];
				bool Open = false;
				switch (Current->ObjectType())
				{
				case CT_ConfigItem:
				default:
					Open = false;
					break;
				}

				if (Open)
				{
					ComponentTypes Type = Current->ObjectType();
				}
			}
		}
	}
	void ComponentViewerList::PopoutObjects(String Title, HINSTANCE ins)
	{
		Vector<Component*> Targets = GetAllComponents();

		new ComponentListRenderer(Targets, ins, Title);
	}
}