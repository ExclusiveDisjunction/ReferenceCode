#pragma once

#include "..\..\UICommon.h"

namespace Mason::UI
{
	namespace Controls
	{
		class EditorButton;
	}

	namespace Editors
	{
		class Editor;
		class EditorRegistry;

		/**
		* When a class wants to be able to host editors, they must inherit this class.
		*/
		class EditorHost
		{
		protected:
			Editor* Current;
			virtual void ProcessOpenEditor(Editor* Frame);
			virtual void ProcessCloseCurrent()
			{
				Current = nullptr;
				if (SingleHostEditor())
					DestroyWindow(operator HWND());

			}
			RECT EditorPlacement;

			friend EditorRegistry;
			friend Controls::EditorButton;

			EditorHost() : Current(nullptr), EditorPlacement(RECT()) {}
		public:
			EditorHost(const EditorHost& Obj) = delete;
			EditorHost(const EditorHost&& Obj) = delete;
			virtual ~EditorHost() {}

			virtual HWND EditorParent() const = 0;
			operator HWND() const
			{
				return GetAncestor(EditorParent(), GA_ROOT);
			}
			/// <summary>
			/// When true, this editor host is only allowed to have one editor open, and once it closes so does this host.
			/// </summary>
			/// <returns></returns>
			virtual bool SingleHostEditor() const { return true; }
		};
	}
}