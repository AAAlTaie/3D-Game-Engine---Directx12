#pragma once
#include "pch/pch.h"


namespace ENGINE 
{
	class X12CommandListManager
	{
	
	public:
		void InitInitializCommaandList(ID3D12Device* pDevice);
		void ReleaseCL();
	private:
		X12CommandListManager() = default;
		~X12CommandListManager();

	public:
		static X12CommandListManager& GetCommandListIncstace();
		static inline ID3D12CommandList* GetCommandList() { return mInstance.mCommandList.Get(); }
		static inline ID3D12GraphicsCommandList* GetGraphicsCommandList() { return (ID3D12GraphicsCommandList*)mInstance.mCommandList.Get(); }

		static void ResetCommandList();
	private:

		static X12CommandListManager mInstance;

	private:

		Microsoft::WRL::ComPtr<ID3D12CommandList> mCommandList; //One command list for testinmg BS
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mCommandAllocator;
		UINT  mWaitingFence = 0;

	};

}

