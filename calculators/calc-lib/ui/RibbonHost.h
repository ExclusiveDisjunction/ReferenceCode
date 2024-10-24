#pragma once

#include "Controls\Controls.h"
#include "..\Common.h"

namespace Mason::UI
{
	class FooterHost
	{
	protected:
		Controls::Grid* Footer;
		Controls::Label* FooterText;
	public:
		void SetFooterText(Core::String Text, int Delay = 3000); //Sets footer text, then resets it after the delay.
		void SetFooterTextTillNext(Core::String Text); //Sets the footer text.
		void ResetFooterText();

		virtual HWND FooterParent() const = 0;
	};

	enum RibbonStatus
	{
		MRS_NoFile = 1,
		MRS_NoUser = 2,
		MRS_StandardUser = 4
	};

	class RibbonHost
	{
	public:
		virtual void SetRibbonStatus() = 0;
	};
}