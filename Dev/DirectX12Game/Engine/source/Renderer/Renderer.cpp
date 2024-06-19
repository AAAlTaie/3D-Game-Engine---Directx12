#include "pch.h"
#include "Renderer.h"
#include "DirectX12/DXGI/DXGIFactory.h"
#include "DirectX12/Debug/DXGIDebug.h"

namespace ALTAIE 
{

	Renderer::~Renderer()
	{


	}

	bool Renderer::InitializRenderer()
	{
		DXGIFactory factory;

		//Adapter
		//Device
		//Swapchains
		
		DXGIDebug::Get().GetLiveObjects();


		return false;
	}




}


