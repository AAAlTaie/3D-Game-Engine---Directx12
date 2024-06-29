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

		inline ID3D12CommandQueue* GetQueue() { return mCommandQueue.Get(); };
		inline UINT GetCurrentFence() { return mCurrentFenceValue; };

	private:
		X12CommandQueueManager() = default;

	public:
		static X12CommandQueueManager& GetCommandInstance();
		

	private:

		static X12CommandQueueManager mInstance;

	private:

		Microsoft::WRL::ComPtr<ID3D12CommandQueue>    mCommandQueue; //array would work for each Queue type
		Microsoft::WRL::ComPtr<ID3D12Fence>           mFence;
		UINT mCurrentFenceValue =                     0;


	};


}

