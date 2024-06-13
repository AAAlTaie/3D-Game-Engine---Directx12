#pragma once

#if ALTAIE_BUILD_DLL
#define ALTAIE_API __declspec(dllexport)
#else
#define ALTAIE_API __declspec(dllimport)
#endif