#pragma once

#include "DeviceResources.h"
#include "Renderer.h"

static HINSTANCE Instance;
static ATOM MainWindowClass;

class MainClass
{
private:
	HWND Wnd;
	HMENU Menu;
	RECT Rc;

	std::shared_ptr<DeviceResources> Resx;
	std::shared_ptr<Renderer> Render;
public:
	MainClass();
	~MainClass();

	HRESULT CreateDesktopWindow();
	HWND GetWindowHandle() { return Wnd; }

	static LRESULT __stdcall StaticWindowProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp);

	HRESULT Run(std::shared_ptr<DeviceResources> DeviceResources, std::shared_ptr<Renderer> Renderer);
};