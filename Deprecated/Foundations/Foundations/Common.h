#pragma once

#ifdef CORE_EXPORTS
#define CORE_API _declspec(dllexport)
#else
#define CORE_API _declspec(dllimport)
#endif