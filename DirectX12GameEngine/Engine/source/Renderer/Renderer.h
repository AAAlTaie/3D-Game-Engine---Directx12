#pragma once
#include "pch/pch.h"
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
	
	class ENGINE_API Renderer
	{

	public:
		Renderer() = default;
		~Renderer();
		bool InitializeRenderer(const HWND hWnd);
		void Flush();

	private:
		X12Device m_Device;
		DXGISwapChain m_SwapChain;

	};

}
