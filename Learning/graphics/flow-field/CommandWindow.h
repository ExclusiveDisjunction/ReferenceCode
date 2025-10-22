#pragma once

#include <Windows.h>
#include <iostream>

#include "UI/Controls.h"
#include "Field/Field.h"

class CommandWindow
{
private:
	HWND Wnd;
	static ATOM ThisAtom;
	static void InitAtom(HINSTANCE ins);
	static LRESULT __stdcall WndProc(HWND Window, UINT Message, WPARAM wp, LPARAM lp);

	FlowFieldGen* Host;

	Label* TitleLbl = nullptr;

	Grid* PerlinSettings = nullptr;
	Label* PerlinLbl = nullptr;
	TextBox* PerlinWidth = nullptr;
	TextBox* PerlinHeight = nullptr;
	Button* PerlinSubmit = nullptr; //1

	Grid* PtsSettings = nullptr;
	Label* PtsLbl = nullptr;
	TextBox* PtsCount = nullptr;
	Button* PtsSubmit = nullptr; //4

	Grid* Commands = nullptr;
	Label* CommandsLbl = nullptr; 
	Button* BeginRender = nullptr; //7
	Button* ClearAll = nullptr; //10
	Button* Quit = nullptr; //11
	Button* Minimize = nullptr; //12
	
	std::vector<ControlBase*> MiscControls;

	void Paint();
	void Command(WPARAM wp, LPARAM lp);

public:
	CommandWindow(HINSTANCE ins, FlowFieldGen* Host);
	~CommandWindow();

	void LoadControls();
	void ShowWindow(int ShowCommand);
};