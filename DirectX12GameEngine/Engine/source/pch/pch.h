#pragma once
#ifndef PCH_H
#define PCH_H 

#include "EngineDebug.h"

#include <stdio.h>
#include <iostream>
#include <wrl.h>
#include <wrl/client.h>

//
#include <d3d12.h>
#include <dxgi1_6.h>

// Debugging toys for DXGI and DX12 
#include <dxgidebug.h>


// libs
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxguid.lib")

#include "EngineCore.h"
#include "EngineDebug.h"
#include "GlobalSettings.h"




#endif // !PCH_H