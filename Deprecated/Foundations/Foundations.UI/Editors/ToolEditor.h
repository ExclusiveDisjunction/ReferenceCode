#pragma once

#include "..\UI\UICommon.h"
#include <Windows.h>

namespace Mason::UI::Editors
{
	class ToolEditor
	{
	private:
		HWND _Base;

		void GenerateBase(HINSTANCE Ins);

		class ToolEditorHost* _Parent = nullptr;

		void Paint();
		virtual void Size() = 0;
		//TODO: Write keyboard shortcuts configuration and add it to tooleditor.
		virtual void CommandSent(WPARAM Identification, LPARAM Message) = 0;

		static LRESULT __stdcall PlaceholderProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp);
		/// <summary>
		/// Used to define the 4 basic functions. 
		/// </summary>
		/// <param name="Window"></param>
		/// <param name="Message"></param>
		/// <param name="wp"></param>
		/// <param name="lp"></param>
		/// <returns></returns>
		static LRESULT __stdcall SimpleProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp);

		static ATOM _ThisAtom;
		static void InitBase(HINSTANCE ins);
	public:
		ToolEditor();
		ToolEditor(const ToolEditor& Obj) = delete;
		ToolEditor(const ToolEditor&& Obj) = delete;
		virtual ~ToolEditor();

		ToolEditor& operator=(const ToolEditor& Obj) = delete;
		ToolEditor& operator=(const ToolEditor&& Obj) = delete;

		virtual bool EquatableTo(ToolEditor* Other) const = 0;
		virtual Core::String& GetName() const = 0;
		virtual Core::String& GetNotes() const = 0;
		virtual bool IsSubmitable() const = 0; //If this returns true, then the EditorHost will show a submit button. 
		virtual void Reset() = 0;
		virtual WNDPROC ThisProc() const = 0;

		operator HWND() const { return _Base; }
		HWND ToHWND() const { return _Base; }
	};

	template<typename TOutput, std::convertible_to<ToolEditor*> ToolEditor, typename... Args>
	TOutput ExecuteToolEditor(ToolEditor* Editor, Args... Input)
	{

	}
}