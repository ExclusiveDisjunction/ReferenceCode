#pragma once

#include "..\Controls\Controls.h"
#include "..\..\UICommon.h"

namespace Mason::UI
{
	class NewFile
	{
	private:
		HWND _Base;
		bool _Loaded;

		Core::String _Return, _Username, _Password;
		int _Config;

		Core::Vector<Controls::ControlBase*> MiscControls; //Labels and such
		Controls::TextBox* Name = nullptr, * Directory = nullptr; //Location & File Name
		Controls::Button* SelectDirectory = nullptr, * Submit = nullptr, * Cancel = nullptr; //Buttons for Commands
		Controls::CheckableButton* InventorySys = nullptr, * TaskSys = nullptr, * UserSys = nullptr, * ResourceSys = nullptr; //Options for different systems in armin.
		Controls::TextBox* Username = nullptr, * Password = nullptr; //Username and Password for Root User.
		Controls::Button* StdProj = nullptr, * InvProj = nullptr, * TaskProj = nullptr;

		void CreateBase(HINSTANCE ins);

		LRESULT Paint();
		LRESULT Size();
		LRESULT KeyDown(WPARAM Key);
		LRESULT Command(WPARAM wp, LPARAM lp);

		static LRESULT __stdcall WndProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp);
		static ATOM _ThisAtom;
		static HICON _ThisIcon;
		static void InitBase(HINSTANCE ins);
	public:
		NewFile();
		NewFile(const NewFile& Obj) = delete;
		NewFile(const NewFile&& Obj) = delete;
		~NewFile()
		{
			SetWindowLongPtr(_Base, GWLP_USERDATA, 0);
			DestroyWindow(_Base);

			DeleteObject(_ThisIcon);
		}

		NewFile& operator=(const NewFile& Obj) = delete;
		NewFile& operator=(const NewFile&& Obj) = delete;

		static LRESULT RunMessageLoop(NewFile* Object, HINSTANCE ins, WindowState* Running);
	
		Core::String const& Return = _Return;
		static Core::String Execute(HINSTANCE ins, Core::String& Username, Core::String& Password, int& Config);
	};
}