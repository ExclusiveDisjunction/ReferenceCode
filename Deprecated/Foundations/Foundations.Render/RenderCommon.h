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

#ifdef FOUNDATIONSRENDER_EXPORTS
#define RENDER_API __declspec(dllexport)
#else
#define RENDER_API __declspec(dllimport)
#endif

#ifndef Assert

#if defined (DEBUG) || defined(_DEBUG)
#define Assert(b) do {if (!(b)) {OutputDebugStringA("Assert: " #b "\n"); }} while (0)
#else
#define Assert(b)
#endif  //Debug

#endif //Assert

namespace Core::Render
{
	template<class Interface>
	inline void SafeRelease(Interface** Obj)
	{
		if (*Obj)
		{
			(*Obj)->Release();
			(*Obj) = nullptr;
		}
	}
}

#endif //RENDERBASE_H