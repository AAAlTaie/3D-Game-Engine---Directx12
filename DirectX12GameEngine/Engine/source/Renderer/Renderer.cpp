#include "pch/pch.h"
#include "Renderer.h"
#include "DX12API/DebugAPI/DXGIDebug.h"
#include "DX12API/DebugAPI/X12Debug.h"
#include "DX12API/DXGI/DXGIFactory.h"
#include "DX12API/DXGI/DXGIAdapter.h"
#include "DX12API/Output/X12Device.h"




namespace ENGINE 
{

	Renderer::~Renderer()
	{


	}

	bool Renderer::InitializRenderer()
	{
		//EngineDebug
		X12Debug::GetX12DebugInstance().Enable();

		//factory
		DXGIFactory factory;

		//Adapter
		DXGIAdapter adapter = factory.GetAdapter();

		//Device
		X12Device device(adapter.Get());

		DXGIDebug::GetInstance().GetLiveObjects();
		
		//Swapchains

		return false;
	}




}
