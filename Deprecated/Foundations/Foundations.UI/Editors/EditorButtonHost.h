#pragma once

#include "EditorHost.h"
#include "EditorCommon.h"
#include "..\Color.h"
#include "..\UI\UICommon.h"
#include "..\UI\Controls.h"

namespace Core::Editors
{
	class CORE_API Editor;
	class CORE_API EditorRegistry;

	class CORE_API EditorButtonHost : public EditorHost
	{
	protected:
		UI::Controls::Grid* EditorButtonsBase;
		UI::Controls::ScrollViewer* EditorButtonsScroll;
		Vector<EditorButton*> EditorButtons;

		void ProcessOpenEditor(Editor* New) override;
		void ProcessCloseCurrent() override
		{
			Current = nullptr;

			OpenLastEditor();
		}

		Editor* LastEditor;

		int CurrentEdtBttnX = 10;
		virtual void AddEditorButton(Editor* Editor);
		virtual void RemoveEditorButton(Editor* Editor);
		virtual void ResizeEditorButtons();

		virtual void LoadEditorButtonsBase(HWND Parent, HINSTANCE ins, int XCoord, int YCoord, int Width, int Height, AaColor BaseBk);
		virtual void MoveEditorButtonsBase(int XCoord, int YCoord, int Width, int Height);
	public:
		EditorButtonHost() : EditorButtonsBase(nullptr), EditorButtonsScroll(nullptr), LastEditor(nullptr) {}
		EditorButtonHost(const EditorButtonHost& Obj) = delete;
		EditorButtonHost(const EditorButtonHost&& Obj) = delete;
		virtual ~EditorButtonHost();
		friend EditorRegistry;
		friend EditorButton;

		EditorButtonHost& operator=(const EditorButtonHost& Obj) = delete;
		EditorButtonHost& operator=(const EditorButtonHost&& Obj) = delete;

		virtual bool SingleHostEditor() const override { return false; }
		void OpenLastEditor();
	};
}