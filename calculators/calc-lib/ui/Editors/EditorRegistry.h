#pragma once

#include "EditorTypes.h"
#include "EditorHost.h"
#include "..\..\UICommon.h"

namespace Mason::UI
{
	class FooterHost;
	
	namespace Editors
	{
		namespace Window
		{
			class EditorPopout;
		}

		class Editor;
		class EditorButtonHost;

		class EditorRegistry
		{
		private:
			EditorRegistry() {}

			static Core::Vector<Editor*> Info;
			static EditorHost* BaseHost;
			static Core::Vector<EditorButtonHost*> ButtonHosts;
			static Core::Vector<UI::FooterHost*> FooterHosts;

			static Editor* CanMesh(int State, Editor*& Source);
		public:
			static void SetHost(EditorHost* BaseHost);
			static void Track(const Core::Vector<EditorButtonHost*>& ButtonHosts, const Core::Vector<UI::FooterHost*>& FooterHosts);
			static void Track(EditorButtonHost* ButtonHost);
			static void Track(UI::FooterHost* FooterHost);
			static void RemoveTracking(EditorButtonHost* Host);
			static void RemoveTracking(UI::FooterHost* Footer);
			static void ExitRegistry();

			static Core::Vector<Editor*> CurrentApplyableEditors();
			static Core::Vector<Editor*> CurrentOpenEditors();

			static Editor* GetEditor(unsigned long long EditorType, EditorHost* Parent = nullptr);
			static Core::Vector<Editor*> GetEditors(unsigned long long EditorType, EditorHost* Parent = nullptr);

			/// <summary>
			/// Moves an editor from one editor host to another.
			/// </summary>
			/// <param name="Source">The editor frame to move.</param>
			/// <param name="To">The EditorHost to move to. If nullptr, EditorRegistry::BaseWindow will be used in it's place.</param>
			/// <param name="ToPopout">When true, the "To" parameter will be disregarded. When true, the editor will be moved to it's own popout window.</param>
			static void MoveEditor(Editor* Source, EditorHost* To, bool ToPopout); //If ToPopout is true, then "To" is disregarded.

			/// <summary>
			/// Resets an editor that matches a specified type.
			/// </summary>
			/// <param name="Types">The editor types to reset. Can use more than one type of editor.</param>
			static void ResetEditorOfType(unsigned long long Types);
			/// <summary>
			/// Resets an editor that matches a specified type and condition.
			/// </summary>
			/// <param name="Types">The editor types to reset. Can use more than one type of editor.</param>
			/// <param name="Condition">The condition the editor must match to be reset.</param>
			static void ResetEditorOfType(unsigned long long Types, Core::Vector<void*> Condition);

			/// <summary>
			/// Evaluates if an editor is open.
			/// </summary>
			/// <param name="Types">The allowed types of editor to test for.</param>
			/// <param name="Host">The Host to search under. If nullptr, all hosts are allowed.</param>
			/// <returns>If the editor matching the specified criteria is open.</returns>
			static bool IsOpen(unsigned long long Types, EditorHost* Host = nullptr);
			/// <summary>
			/// Evaluates if an editor is open.
			/// </summary>
			/// <param name="Types">The allowed types of editor to test for.</param>
			/// <param name="Args">The condition that the editor must match.</param>
			/// <param name="Host">The Host to search under. If nullptr, all hosts are allowed.</param>
			/// <returns>If the editor matching the specified criteria is open.</returns>
			static bool IsOpen(unsigned long long Types, Core::Vector<void*> Args, EditorHost* Host = nullptr);

			/// <summary>
			/// Opens an editor, and specifies it's parent.
			/// </summary>
			/// <param name="Editor">The editor frame to open.</param>
			/// <param name="Host">The host to open the editor under. If nullptr, then the base window will be used.</param>
			/// <returns>The editor that was opened (Param: Editor), or the meshed result. </returns>
			static Editor* OpenEditor(Editor* Editor, EditorHost* Host);

			/// <summary>
			/// Closes an editor.
			/// </summary>
			/// <param name="Editor">The editor to close.</param>
			/// <param name="AskForApply">Specifies if the editor should be applied before closing.</param>
			/// <returns>If the editor was closed. When AskForApply is true, and the apply was not sucessful, the return is false.</returns>
			static bool CloseEditor(Editor* Editor, bool AskForApply = true);
			/// <summary>
			/// Closes all editors.
			/// </summary>
			/// <returns>If all editors were closed sucessfuly.</returns>
			static bool CloseAllEditors();
			static bool CloseEditors(Core::Vector<Editor*>& Editors);
			/// <summary>
			/// Closes all editors without saving.
			/// </summary>
			/// <returns>Always true.</returns>
			static bool ForceCloseAllEditors();
			/// <summary>
			/// Used to remove the targeted editor from the registry.
			/// </summary>
			/// <param name="ToRemove">The editor to stop tracking.</param>
			/// <returns>Whether or not th editor was removed.</returns>
			static bool PopEditor(Editor* ToRemove);
		};
	}
}