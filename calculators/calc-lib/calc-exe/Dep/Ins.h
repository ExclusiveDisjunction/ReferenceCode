#pragma once

#include <filesystem>
#include <Windows.h>

#include "Common.h"

namespace Mason
{
	class Ins
	{
	public:
		Ins()
		{
			LastWindowState = 0;
			DevMode = LightMode = LoadLast = false;
		}
		Ins(const Ins& Other) = delete;
		Ins(const Ins&& Other) = delete;

		static void InitiateBase(HINSTANCE Ins, LPWSTR CmdLine);
		static void Shutdown();

		static String BaseDir();
		static bool IsLoaded();

		void LoadData();
		void Save();

		int LastWindowState;
		bool LoadLast;
		String LastLoaded;
		bool DevMode;
		bool LightMode;
		bool FirstTime;

		Ins& operator=(const Ins& Other) = delete;
		Ins& operator=(const Ins&& Other) = delete;
	};
}