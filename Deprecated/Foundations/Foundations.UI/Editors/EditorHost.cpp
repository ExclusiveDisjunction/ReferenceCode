#include "EditorHost.h"

#include "Editor.h"

namespace Core::Editors
{
	void EditorHost::ProcessOpenEditor(Editor* New)
	{
		Editor* OldCurrent = Current;
		Current = New;

		if (Current)
		{
			if (OldCurrent)
				ShowWindow(*OldCurrent, SW_HIDE);

			if (GetAncestor(GetForegroundWindow(), GA_ROOT) != operator HWND())
			{
				FLASHWINFO Info = { 0 };
				Info.cbSize = sizeof(FLASHWINFO);
				Info.hwnd = operator HWND();
				Info.dwFlags = FLASHW_ALL;
				Info.uCount = 3;
				Info.dwTimeout = 0;
				FlashWindowEx(&Info);
			}
		}
	}
}