#include "X12CommandQueueManager.h"



//Compy and compute  
namespace ENGINE 
{
	X12CommandQueueManager X12CommandQueueManager::mInstance;

	void X12CommandQueueManager::InitializeCommaands(ID3D12Device* pDevice)
	{

		//ComPtr : CommandQueue
		D3D12_COMMAND_QUEUE_DESC cmdQds {};
		cmdQds.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		cmdQds.Priority = D3D12_COMMAND_QUEUE_PRIORITY_HIGH;
		cmdQds.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		cmdQds.NodeMask = 0; 

		ID3D12CommandQueue* commandQ = nullptr;
		WTF(pDevice->CreateCommandQueue(
			&cmdQds, 
			IID_PPV_ARGS(mCommandQueue.GetAddressOf())));

		//ComPtr : Fence
		WTF(pDevice->CreateFence(
			mCurrentFenceValue, 
			D3D12_FENCE_FLAG_NONE, 
			IID_PPV_ARGS(mFence.GetAddressOf())));

	}
	void X12CommandQueueManager::ReleaseCQ()
	{
		while (mFence->GetCompletedValue() < mCurrentFenceValue)
		{
			PRINT_N("WAITING ON GPU!");
			_mm_pause();
		}
		mCommandQueue.Reset();
		mFence.Reset();

	}

	X12CommandQueueManager::~X12CommandQueueManager()
	{
		ReleaseCQ();
	}

	X12CommandQueueManager& X12CommandQueueManager::GetCommandQueueInstance() { return mInstance; }

	void X12CommandQueueManager::ExecuteCommandList(ID3D12CommandList* pCommandlist)
	{
		mInstance.mCommandQueue.Get()->ExecuteCommandLists(1, (ID3D12CommandList* const*)&pCommandlist);
		++mInstance.mCurrentFenceValue;
		mInstance.mCommandQueue.Get()->Signal(mInstance.mFence.Get(), mInstance.mCurrentFenceValue);
	}
	
}