#include <Windows.h>
#include <gdiplus.h>
#include <filesystem>
#pragma comment(lib, "gdiplus.lib")

#include "Common.h"
#include "UICommon.h"
#include "UI\Editors\EditorRegistry.h"
#include "UI\Main.h"
#include "UI\Startup.h"
#include "UI\Welcome.h"

using namespace Gdiplus;
using namespace std;
using namespace std::filesystem;

using namespace Mason;
using namespace Mason;
using namespace Mason::IO;
using namespace Mason::UI;
using namespace Mason::UI::Editors;

int __stdcall wWinMain(HINSTANCE ins, HINSTANCE, LPWSTR lpCmdLine, int nCmdShow)
{
	ULONG_PTR Handle;
	GdiplusStartupInput In;
	GdiplusStartup(&Handle, &In, nullptr);

	/*{
		Ins::InitiateBase(ins, lpCmdLine);
		Recent::Initalize();

		InsInstance = new Ins();
		InsInstance->LoadData();

		RecentInstance = new Recent();
		RecentInstance->LoadData();
	}*/

	/*Main* main = new Main(ins);
	MasterRibbon = main;
	FooterOutput = main;

	main->Open();
	delete main;*/

	Startup* Start = new Startup(ins);
	Start->Open();

	Welcome* WelcomeWindow = new Welcome(ins);
	WelcomeWindow->Open(); //Assumes the launching part of the program. All of the program runs through this function, by concept. 

	delete WelcomeWindow;

	{
		/*InsInstance->Save();
		RecentInstance->Save();

		delete InsInstance;
		InsInstance = nullptr;
	
		delete RecentInstance;
		RecentInstance = nullptr;

		Ins::Shutdown();*/
		EditorRegistry::ExitRegistry();
	}

	GdiplusShutdown(Handle);

	DestroyIcon(MasonIcon);
	DestroyCursor(MasonCursor);

	return 0;
}