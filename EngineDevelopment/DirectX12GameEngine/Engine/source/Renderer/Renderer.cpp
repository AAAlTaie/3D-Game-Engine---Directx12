#include "pch/pch.h"
#include "Renderer.h"
#include "DX12API/DXGI/DXGIFactory.h"



namespace ENGINE 
{

	Renderer::~Renderer()
	{


	}

	bool Renderer::InitializRenderer()
	{

		//DXDebug::GetInstance().Enable();

		//factory
		DXGIFactory factory;

		//Adapter
		//DXGIAdapter adapter = factory.GetAdapter();

		//Device
		//DX12Device device(adapter.Get());

		//DXGIDebug::Get().GetLiveObjects();

		//Swapchains

		return false;
	}




}
