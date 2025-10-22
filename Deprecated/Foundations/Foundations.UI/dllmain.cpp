#include <Windows.h>
#include "Controls\Control.h"

bool __stdcall DllMain(HINSTANCE ins, DWORD reason, LPVOID)
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
		//Register the class, since DLL requires that all wndclasses get registered and unregistered at startup/shutdown.
		Core::UI::Controls::Control::RegisterAtom(ins);
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		UnregisterClass(MAKEINTATOM(Core::UI::Controls::Control::_ThisAtom), ins);
		break;
	}
	return TRUE;
}