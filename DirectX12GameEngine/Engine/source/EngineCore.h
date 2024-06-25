#pragma once
#include <Windows.h>



// Define ALTAIE_API for DLL export/import if required
#ifdef ENGINE_BUILD_DLL

#define ENGINE_API __declspec(dllexport)

#else

#define ENGINE_API __declspec(dllimport)

#endif