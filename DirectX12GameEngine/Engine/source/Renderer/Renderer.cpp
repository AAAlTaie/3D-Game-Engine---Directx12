#include "pch/pch.h"
#include "Renderer.h"
#include "DX12API/DebugAPI/DXGIDebug.h"
#include "DX12API/DebugAPI/X12Debug.h"
#include "DX12API/DXGI/DXGIFactory.h"
#include "DX12API/DXGI/DXGIAdapter.h"
#include "DX12API/Output/X12Device.h"
#include "DX12API/DXGI/DXGISwapChain.h"




namespace ENGINE 
{

	Renderer::~Renderer()
	{


	}

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
		

		
		

		D3D12_COMMAND_QUEUE_DESC cmdQds; 
		cmdQds.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		cmdQds.Priority = D3D12_COMMAND_QUEUE_PRIORITY_HIGH;
		cmdQds.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		cmdQds.NodeMask = 0;

		ID3D12CommandQueue* commandQ;
		device->CreateCommandQueue(&cmdQds, IID_PPV_ARGS(&commandQ));

		//// Swapchain
		DXGISwapChain swapChain(commandQ,device.Get(), factory.Get(), hWnd, DXGI_FORMAT_R16G16B16A16_FLOAT, 3);

		//swapChain.Present();

		DXGIDebug::GetInstance().GetLiveObjects();
		return false;
	}




}
