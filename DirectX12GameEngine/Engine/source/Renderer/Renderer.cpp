#include "pch/pch.h"
#include "Renderer.h"
#include "DX12API/DebugAPI/DXGIDebug.h"
#include "DX12API/DebugAPI/X12Debug.h"
#include "DX12API/DXGI/DXGIFactory.h"
#include "DX12API/DXGI/DXGIAdapter.h"
#include "DX12API/Output/X12Device.h"
#include "DX12API/DXGI/DXGISwapChain.h"
#include "DX12API/CommandManagment/X12CommandQueueManager.h"
#include "DX12API//CommandManagment/X12CommandListManager.h"

namespace ENGINE 
{

	Renderer::~Renderer() {}

	bool Renderer::InitializRenderer(const HWND hWnd)
	{
		//EngineDebug
		X12Debug::GetX12DebugInstance().Enable();

		//factory
		DXGIFactory factory;

		//Adapter
		DXGIAdapter adapter = factory.GetAdapter();

		//Device
		X12Device device = (adapter.Get());
	
		//CommandQueueManager
		X12CommandQueueManager::GetCommandInstance().InitializeCommaands(device.Get());

		//CommandListManager   //CommandListInstance
		X12CommandListManager::GetCLIncstace().InitInitializCommaandList(device.Get());

		//Swapchain
		DXGISwapChain swapChain(
			X12CommandQueueManager::GetCommandInstance().GetQueue(),
			device.Get(), 
			factory.Get(), 
			hWnd, 
			DXGI_FORMAT_R16G16B16A16_FLOAT, 
			GNORF);

		//swapChain.Present();

		DXGIDebug::GetInstance().GetLiveObjects();
		return false;
	}


}
