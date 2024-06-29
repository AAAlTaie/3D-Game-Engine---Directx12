#include "X12CommandQueueManager.h"



//Compy and compute  
namespace ENGINE 
{
	X12CommandQueueManager X12CommandQueueManager::mInstance;

	void X12CommandQueueManager::InitializeCommaands(ID3D12Device* pDevice)
	{

		//ComPtr : CommandQueue
		D3D12_COMMAND_QUEUE_DESC cmdQds; 
		cmdQds.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		cmdQds.Priority = D3D12_COMMAND_QUEUE_PRIORITY_HIGH;
		cmdQds.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		cmdQds.NodeMask = 0; 

		ID3D12CommandQueue* commandQ;
		pDevice->CreateCommandQueue(
			&cmdQds, 
			IID_PPV_ARGS(mCommandQueue.GetAddressOf()));

		//ComPtr : Fence
		pDevice->CreateFence(
			mCurrentFenceValue, 
			D3D12_FENCE_FLAG_NONE, 
			IID_PPV_ARGS(mFence.GetAddressOf()));

	}
	X12CommandQueueManager& X12CommandQueueManager::GetCommand() { return mInstance; }
}