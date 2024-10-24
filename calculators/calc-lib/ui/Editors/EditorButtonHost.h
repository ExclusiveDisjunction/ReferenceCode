#pragma once

#include "Core\Vector.h"

#include "EditorHost.h"
#include "..\Controls\Controls.h"
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

		class EditorButtonHost : public EditorHost
		{
		protected:
			Controls::Grid* EditorButtonsBase;
			Controls::ScrollViewer* EditorButtonsScroll;
			Core::Vector<Controls::EditorButton*> EditorButtons;

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

			virtual void LoadEditorButtonsBase(HWND Parent, HINSTANCE ins, int XCoord, int YCoord, int Width, int Height, Core::Color BaseBk);
			virtual void MoveEditorButtonsBase(int XCoord, int YCoord, int Width, int Height);
		public:
			EditorButtonHost() : EditorButtonsBase(nullptr), EditorButtonsScroll(nullptr), LastEditor(nullptr) {}
			EditorButtonHost(const EditorButtonHost& Obj) = delete;
			EditorButtonHost(const EditorButtonHost&& Obj) = delete;
			virtual ~EditorButtonHost();
			friend EditorRegistry;
			friend Controls::EditorButton;

			EditorButtonHost& operator=(const EditorButtonHost& Obj) = delete;
			EditorButtonHost& operator=(const EditorButtonHost&& Obj) = delete;

			virtual bool SingleHostEditor() const override { return false; }
			void OpenLastEditor();
		};
	}
}