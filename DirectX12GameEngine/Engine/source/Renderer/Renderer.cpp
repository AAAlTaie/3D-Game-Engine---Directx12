#include "Renderer.h"



namespace ENGINE 
{

	bool Renderer::InitializeRenderer(const HWND hWnd)
	{
		//EngineDebug
		X12Debug::GetX12DebugInstance().Enable();

		//factory
		DXGIFactory factory;

		//Adapter
		DXGIAdapter adapter = factory.GetAdapter();

		//Device
		m_Device.InitializeDevice(adapter.Get());

		//CommandQueueManager
		X12CommandQueueManager::GetCommandQueueInstance().InitializeCommaands(m_Device.Get());

		//CommandListManager   //CommandListInstance
		X12CommandListManager::GetCommandListIncstace().InitInitializCommaandList(m_Device.Get());

		m_SwapChain.InitializeSwapChain(X12CommandQueueManager::GetQueue(),
			m_Device.Get(),
			factory.Get(),
			hWnd,
			DXGI_FORMAT_R16G16B16A16_FLOAT,
			GNORF);


		//TODO: Transition Render Target View form Present to RTV
		X12ResStateManager::TransitionResource(
			X12CommandListManager::GetGraphicsCommandList(),
			D3D12_RESOURCE_STATE_PRESENT,
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			m_SwapChain.GetCurrentBuffer()
			);

		//Clear Buffer Color
		const float clearBufferColor[] = { 0.666f, 0.420f, 0.184f, 1.0f };
		
        X12CommandListManager::GetGraphicsCommandList()->ClearRenderTargetView(
			m_SwapChain.GetCurrentRTVHandle(),
			clearBufferColor,
			NULL, 
			NULL
			);

		X12ResStateManager::TransitionResource(
			X12CommandListManager::GetGraphicsCommandList(),
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_PRESENT,
			m_SwapChain.GetCurrentBuffer()

		);


		X12CommandListManager::GetGraphicsCommandList()->Close();


		X12CommandQueueManager::ExecuteCommandList(X12CommandListManager::GetCommandList());


		m_SwapChain.Present();

		while (X12CommandQueueManager::GetFence()->GetCompletedValue() < X12CommandQueueManager::GetCurrentFence())
		{
			PRINT_N("WAITING ON GPU!");
			_mm_pause();
		}

		
		X12CommandListManager::ResetCommandList();
		
		return false;
	}

	void Renderer::Flush()
	{
		for (UINT i = 0; i < m_SwapChain.FramesInFlight(); i++)
		{
			X12CommandQueueManager::GetQueue()->Signal(X12CommandQueueManager::GetFence(), X12CommandQueueManager::GetCurrentFence() + i);

		}
	}

	Renderer::~Renderer()
	{

		Flush();

		m_SwapChain.Release();
		//m_SwapChain.Reset();
		m_Device.Reset();

		DXGIDebug::GetInstance().GetLiveObjects();


	}

}
