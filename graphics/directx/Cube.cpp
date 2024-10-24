#include <string>
#include <memory>

#include "DeviceResources.h"
#include "Renderer.h"
#include "Main.h"

int __stdcall wWinMain(HINSTANCE ins, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	HRESULT hr = S_OK;

#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	std::shared_ptr<MainClass> Main = std::shared_ptr<MainClass>(new MainClass());
	hr = Main->CreateDesktopWindow();

	if (SUCCEEDED(hr))
	{
		std::shared_ptr<DeviceResources> Resx = std::shared_ptr<DeviceResources>(new DeviceResources());
		Resx->CreateDeviceResources();

		std::shared_ptr<Renderer> Render = std::shared_ptr<Renderer>(new Renderer(Resx));
		Render->CreateDeviceDependentResources();

		Resx->CreateWindowResources(Main->GetWindowHandle());
		Render->CreateWindowSizeDependentResources();
		
		hr = Main->Run(Resx, Render);
	}

	return hr;
}