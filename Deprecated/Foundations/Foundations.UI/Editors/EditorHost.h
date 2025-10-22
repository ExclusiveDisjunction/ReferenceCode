#pragma once

#include <Windows.h>
#include "EditorCommon.h"
#include "..\Common.h"
#include "..\UI\UICommon.h"

namespace Core::Editors
{
	class CORE_API EditorButton;

	class CORE_API Editor;
	class CORE_API EditorRegistry;

	/**
	* When a class wants to be able to host editors, they must inherit this class.
	*/
	class CORE_API EditorHost
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
		friend EditorButton;

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