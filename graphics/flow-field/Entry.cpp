#include <Windows.h>

#include "RenderWindow.h"
#include "CommandWindow.h"
#include "Render/Renderer.h"
#include "Field/Field.h"

#include "Render/FillRenderable.h"

int __stdcall wWinMain(HINSTANCE ins, HINSTANCE, LPWSTR lpCmdLine, int nCmd)
{
	Renderer* rnder = new Renderer(nullptr);
	rnder->Init();

	FillRenderable* Fill = new FillRenderable(rnder);
	FlowFieldGen* Host = new FlowFieldGen(rnder);

	CommandWindow* CmdWnd = new CommandWindow(ins, Host);
	RenderWindow* RenderWnd = new RenderWindow(ins, rnder);

	CmdWnd->LoadControls();
	CmdWnd->ShowWindow(SW_SHOW);

	MSG msg = { 0 };
	while (GetMessage(&msg, nullptr, 0, 0) != 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	delete Host;
	delete rnder;

	return static_cast<int>(msg.wParam);
}