#include "pch.h"
#include "Renderer.h"

#include "DirectX12/Debug/DXGIDebug.h"
#include "DirectX12/Debug/DXDebug.h"

#include "DirectX12/DXGI/DXGIFactory.h"
#include "DirectX12/DXGI/DXGIAdapter.h"


#include "DirectX12/DeviceOutput/DX12Device.h"


namespace ALTAIE 
{

	Renderer::~Renderer()
	{


	}

	bool Renderer::InitializRenderer()
	{

		DXDebug::GetInstance().Enable();

		//factory
		DXGIFactory factory;

		//Adapter
		DXGIAdapter adapter = factory.GetAdapter();

		//Device
		DX12Device device(adapter.Get());

		DXGIDebug::Get().GetLiveObjects();

		//Swapchains

		return false;
	}




}


