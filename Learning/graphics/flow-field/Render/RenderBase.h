#ifndef RENDERBASE_H
#define RENDERBASE_H

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <wchar.h>
#include <math.h>

#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>

#include <functional>
#include <vector>
#include <map>

#pragma comment(lib, "d2d1")

template<class Interface>
inline void SafeRelease(Interface** Obj)
{
	if (*Obj)
	{
		(*Obj)->Release();
		(*Obj) = nullptr;
	}
}

#ifndef Assert

#if defined (DEBUG) || defined(_DEBUG)
#define Assert(b) do {if (!(b)) {OutputDebugStringA("Assert: " #b "\n"); }} while (0)
#else
#define Assert(b)
#endif  //Debug

#endif //Assert

#endif //RENDERBASE_H