#pragma once
#ifndef PCH_H
#define PCH_H

//standaard Libs 

#include <stdio.h>
#include <iostream>
#include <wrl.h>
#include <wrl/client.h>


#include <d3d12.h>
#include <dxgi1_6.h>

// Debugging toys for DXGI and DX12 
#include <dxgidebug.h>


// libs
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxguid.lib")






#include <EngineMin.h>
#include "Renderer/DirectX12/Debug/DirectXDebug.h"



#endif // !PCH_H

//
//#if defined(_DEBUG)
//#pragma comment(lib, "dxgi.lib")
//#include <dxgidebug.h>
//#endif

