#pragma once

#include "..\..\Core\Color.h"
#include "UI\Controls.h"
#include "..\..\Common.h"
#include "..\..\UI\MasonControls.h"

namespace Mason
{
	namespace Files
	{
		class Component;
		class ComponentRenderer;
		class ComponentReference;
	}

	namespace UI::Tool
	{
		class ComponentListRenderer
		{
		private:
			Vector<Files::Component*> _Source;

			Button* View, * Edit;
			ScrollViewer* ObjectScroll;
			Grid* ObjectView;
			ComponentViewerList* Objects;

			bool MultiselectMode = true, _Loaded = false, _CanEdit = true;
			HWND _Base;

			void LoadControls();
			void ResizeObjects();
			void FillObjects();

			LRESULT Paint();
			LRESULT Destroy();
			LRESULT Command(WPARAM wp, LPARAM lp);
			LRESULT Size();

			static LRESULT __stdcall WndProc(HWND, UINT, WPARAM, LPARAM);
			static ATOM _ThisAtom;
			static void InitBase(HINSTANCE ins);

		public:
			ComponentListRenderer(const Vector<Files::Component*>& Source, HINSTANCE ins, String WindowTitle = L"Component List", bool CanEdit = true);
			//ComponentListRenderer(const Files::ReferenceList& Source, HINSTANCE ins, String WindowTitle = L"Component List", bool CanEdit = true);
			ComponentListRenderer(const Vector<Files::ComponentReference*>& Source, HINSTANCE Ins, String WindowTitle = L"Component List", bool CanEdit = true);
			ComponentListRenderer(const ComponentListRenderer& Obj) = delete;
			ComponentListRenderer(const ComponentListRenderer&& Obj) = delete;
			~ComponentListRenderer();

			ComponentListRenderer& operator=(const ComponentListRenderer& Obj) = delete;
			ComponentListRenderer& operator=(const ComponentListRenderer&& Obj) = delete;

			void ChangeSource(const Vector<Files::Component*>& Source);
			void ChangeSource(const Vector<Files::ComponentReference*>& Source);
		};
	}
}