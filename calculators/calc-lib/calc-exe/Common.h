#pragma once

#include "Enums.h"
#include "Math\StdCalc.h"
#include "Core\Color.h"
#include "Core\Vector.h"
#include "Core\List.h"
#include "Core\Str.h"

namespace Mason
{
	namespace IO
	{
		class FileInstance;
	}
	namespace UI
	{
		class FooterHost;
		class RibbonHost;
	}

	extern UI::FooterHost* FooterOutput; //Use to display text out.
	extern UI::RibbonHost* MasterRibbon; //Use to set the state of the MasterRibbon

	//extern Ins* InsInstance;
	//extern Recent* RecentInstance;

	extern IO::FileInstance* LoadedProject;
	extern Core::String LoadedProjectPath;
	extern int AppState;

	extern const Core::String Version;
	extern const Core::String TitleFontName;
	extern const Core::String StandardFontName;

	/*
	bool HasEdit();
	void HasEdit(bool New);
	*/
}