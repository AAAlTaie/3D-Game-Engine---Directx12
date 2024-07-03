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
#include "DX12API/Resources/X12ResStateManager.h"


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
		X12CommandQueueManager::GetCommandQueueInstance().InitializeCommaands(device.Get());

		//CommandListManager   //CommandListInstance
		X12CommandListManager::GetCommandListIncstace().InitInitializCommaandList(device.Get());

		//Swapchain
		DXGISwapChain swapChain(
			X12CommandQueueManager::GetCommandQueueInstance().GetQueue(),
			device.Get(), 
			factory.Get(), 
			hWnd, 
			DXGI_FORMAT_R16G16B16A16_FLOAT, 
			GNORF);

		//TODO: Transition Render Target View form Present to RTV
		X12ResStateManager::TransitionResource(
			X12CommandListManager::GetGraphicsCommandList(),
			D3D12_RESOURCE_STATE_PRESENT,
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			swapChain.GetCurrentBuffer()
			);

		//Clear Buffer Color
		const float clearBufferColor[] = { 0.333f, 0.420f, 0.184f, 1.0f };
		
        X12CommandListManager::GetGraphicsCommandList()->ClearRenderTargetView(
			swapChain.GetCurrentRTVHandle(), 
			clearBufferColor,
			NULL, 
			NULL
			);
		//swapChain.Present();

		DXGIDebug::GetInstance().GetLiveObjects();
		return false;
	}


}
