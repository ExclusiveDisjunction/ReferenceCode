#pragma once

#include <Windows.h>
#include "Render/Renderer.h"

#include <memory>

class RenderWindow
{
public:
	RenderWindow(HINSTANCE ins, Renderer* Render);
	~RenderWindow();

	void Bind(Renderer* Render);
	void Unbind();

	HWND GetHWND() const { return Window; }

private:
	HWND Window;
	Renderer* Render;

	void Paint();
	void Command(WPARAM wp, LPARAM lp);
	void Size(UINT Width, UINT Height);

	static ATOM _ThisAtom;
	static void RegisterAtom(HINSTANCE ins);
	static LRESULT __stdcall WndProc(HWND, UINT, WPARAM, LPARAM);
};