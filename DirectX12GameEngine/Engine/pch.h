#pragma once

#define WIN32_LEAN_AND_MEAN

//TODO : MEMORY LEAK DET
#ifdef _DEBUG  
#define _CRTDBG_MAP_ALLOC
        #include <crtdbg.h>
        #define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif // _DEBUG

typedef unsigned char              byte;
typedef unsigned char              uint8;
typedef unsigned int               uint;
typedef unsigned int               uint32;
typedef unsigned __int64           uint64;


#define HR(x) __hr(x, __FILE__, __LINE__)
#define safe_delete(x) if(x) { delete x; x = nullptr; }
#define safe_release(x) if(x) { x->Release(); x = nullptr; }

#include <Windows.h>
#include <iostream>
#include <vector>
#include <string>
#include <bitset>
#include "RealTimeDebug.h"
#include <stdexcept> 


#include <d3dx12.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxgidebug.h>
#include <d3dcompiler.h>



#include <DirectXMath.h>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "D3DCompiler.lib")



#include "EngineCore.h"
#ifdef _DEBUG
#define set_name(DX12_obj, name) if(DX12_obj) { DX12_obj->SetName(L#name); }
#else //Release config
#define set_name(DX12_obj, name) //chilling 
#endif // _DEBUG

inline void __hr(HRESULT hr, LPCSTR filename, int line)
{

    if (FAILED(hr)) {
        char* buffer = {};
        FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPSTR)&buffer, 0, NULL
        );
        LocalFree(buffer);
        char totalCuffer[256];
        filename = (::strrchr(filename, '\\') + 1);
        sprintf_s(totalCuffer, "%s\nfile: %s\n line %d\n", buffer, filename, line);

        if (MessageBoxA(NULL, totalCuffer, "HRESULT Error", MB_OK)) {
            ExitProcess(0);
        }

    }
}

//fdl renderer
namespace ENGINE
{
    class ENGINE_API Renderer;

}