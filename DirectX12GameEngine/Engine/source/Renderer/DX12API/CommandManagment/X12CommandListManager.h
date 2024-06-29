#pragma once
#include "pch/pch.h"


namespace ENGINE 
{
	class X12CommandListManager
	{
	
	public:

		void InitInitializCommaandList(ID3D12Device* pDevice);





	private:

		X12CommandListManager() = default;

	public:

		static X12CommandListManager& GetCLIncstace();

	private:

		static X12CommandListManager mInstance;

	private:

		Microsoft::WRL::ComPtr<ID3D12CommandList> mCommandList; //One command list for testinmg BS
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mCommandAllocator;
		UINT  mWaitingFence = 0;

	};

}

