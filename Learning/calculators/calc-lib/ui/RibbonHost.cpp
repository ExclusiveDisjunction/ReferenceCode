#include "RibbonHost.h"

//#include "..\Ins.h"

namespace Mason::UI
{
	using namespace Core;

	void FooterHost::SetFooterText(String Text, int Delay)
	{
		if (!FooterText)
			return;

		FooterText->SetText(Text);
		SetTimer(FooterParent(), 2, Delay, NULL);
	}
	void FooterHost::SetFooterTextTillNext(String Text)
	{
		if (FooterText)
			FooterText->SetText(Text);
	}
	void FooterHost::ResetFooterText()
	{
		if (FooterText)
			FooterText->SetText(L"Ready");
	}
}