#pragma once
#include "pch/pch.h"


namespace ENGINE 
{
	class X12CommandListManager
	{
	
	public:

		void InitInitializCommaandList(ID3D12Device* pDevice);
		//inline ID3D12GraphicsCommandList* GetGraphicsCommandList() { return (ID3D12GraphicsCommandList*)mCommandList.Get(); };

	private:

		X12CommandListManager() = default;

	public:

		static X12CommandListManager& GetCommandListIncstace();
		static ID3D12GraphicsCommandList* GetGraphicsCommandList();
	private:

		static X12CommandListManager mInstance;

	private:

		Microsoft::WRL::ComPtr<ID3D12CommandList> mCommandList; //One command list for testinmg BS
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mCommandAllocator;
		UINT  mWaitingFence = 0;

	};

}

