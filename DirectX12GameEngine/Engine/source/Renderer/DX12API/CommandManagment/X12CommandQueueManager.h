#pragma once
#include "pch/pch.h"


namespace ENGINE
{
	//Singleton BS
	class X12CommandQueueManager
	{

	public:

		//X12CommandQueueManager() = default;
		void InitializeCommaands(ID3D12Device* pDevice);
		void ReleaseCQ();

	private:
		X12CommandQueueManager() = default;
		~X12CommandQueueManager();
	public:

		static X12CommandQueueManager& GetCommandQueueInstance();
		static inline ID3D12CommandQueue* GetQueue() { return mInstance.mCommandQueue.Get(); };
		static inline ID3D12Fence* GetFence() { return mInstance.mFence.Get(); };
		static inline UINT64 GetCurrentFence() { return mInstance.mCurrentFenceValue; };
		static void ExecuteCommandList(ID3D12CommandList* pCommandlist);
		
		

	private:

		static X12CommandQueueManager mInstance;

	private:

		Microsoft::WRL::ComPtr<ID3D12CommandQueue>    mCommandQueue; //array would work for each Queue type
		Microsoft::WRL::ComPtr<ID3D12Fence>           mFence;
		UINT64 mCurrentFenceValue =                     0;


	};


}

