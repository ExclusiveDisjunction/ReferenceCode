#include "Common.h"
#include "UICommon.h"

//#include "Ins.h"
//#include "Recent.h"
//#include "IO\FileInstance.h"

namespace Mason
{
	using namespace Core;

	namespace Colors
	{
		//This group is for the base values. It is done in here so that the data will not be modifyed by anyone outside of this namespace.

		Color Background1 = 0xFF2d2530;
		Color Background2 = 0xFF69517D;

		Color Accent1 = 0xFF5A2F9A;
		Color Accent2 = 0xFF835389;
		Color Accent3 = 0xFF9C3EA6; //0xFF9A00D8;

		Color Grey1 = 0xFF282828;
		Color Grey2 = 0xFF444444;
		Color Grey3 = 0xFF141414;
		Color FontColor = 0xFFFFFFFF;
		Color EditorGrey = 0xFF313131;
	}

	//The colors in this section are refrenced to the ones in the Colors namespace, but cannot be modifed. This was intentional.

	const Color& Background1 = Colors::Background1;
	const Color& Background2 = Colors::Background2;

	const Color& Grey1 = Colors::Grey1;
	const Color& Grey2 = Colors::Grey2;
	const Color& Grey3 = Colors::Grey3;
	const Color& FontColor = Colors::FontColor;
	const Color& EditorGrey = Colors::EditorGrey;
	
	const Color& Accent1 = Colors::Accent1;
	const Color& Accent2 = Colors::Accent2;
	const Color& Accent3 = Colors::Accent3;

	UI::FooterHost* FooterOutput;
	UI::RibbonHost* MasterRibbon;

	HICON MasonIcon = NULL;
	HCURSOR MasonCursor = LoadCursorW(NULL, IDC_ARROW);

	//Ins* InsInstance = nullptr;
	//Recent* RecentInstance = nullptr;

	IO::FileInstance* LoadedProject;
	String LoadedProjectPath;
	int AppState = 0;

	const String Version = L"Pilot";
	const String TitleFontName = L"Baskerville Old Face";
	const String StandardFontName = L"Candara";
}