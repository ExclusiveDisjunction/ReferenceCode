#include "FooterHost.h"

namespace Core::UI::Windows
{
	void FooterHost::SetFooterText(const String& Text, int Delay)
	{
		if (!FooterText)
			return;

		FooterText->SetText(Text);
		SetTimer(FooterParent(), 2, Delay, nullptr);
	}
	void FooterHost::SetFooterTextTillNext(const String& Text)
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