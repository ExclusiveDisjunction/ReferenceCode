#pragma once

#include "..\UICommon.h"
#include "..\Controls\Label.h"
#include "..\Controls\Grid.h"
#include "..\..\Str.h"

namespace Core::UI::Windows
{
	class CORE_API FooterHost
	{
	protected:
		Controls::Grid* Footer;
		Controls::Label* FooterText;

	public:
		void SetFooterText(const String& Text, int Delay = 3000);
		void SetFooterTextTillNext(const String& Text);
		void ResetFooterText();

		virtual HWND FooterParent() const = 0;
	};

	enum RibbonStatus
	{
		NoFile = 1,
		NoUser = 2,
		StandardUser = 4
	};

	class RibbonHost
	{
	public:
		virtual void SetRibbonStatus() = 0;
	};
}