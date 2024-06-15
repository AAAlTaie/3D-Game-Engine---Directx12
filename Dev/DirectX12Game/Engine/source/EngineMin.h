#pragma once

// Include necessary standard or project-specific headers
#include <windows.h>

// Define ALTAIE_API for DLL export/import if required
#ifdef ALTAIE_EXPORTS
#define ALTAIE_API __declspec(dllexport)
#else
#define ALTAIE_API __declspec(dllimport)
#endif




//#pragma once
//#include <Windows.h>
//
//#if ALTAIE_BUILD_DLL
//#define ALTAIE_API __declspec(dllexport)
//#else
//#define ALTAIE_API __declspec(dllimport)
//#endif