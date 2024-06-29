#include "X12CommandListManager.h"

namespace ENGINE
{
	X12CommandListManager X12CommandListManager::mInstance;

	void X12CommandListManager::InitInitializCommaandList(ID3D12Device* pDevice)
	{
		//Create Command Allocator 
		WTF(pDevice->CreateCommandAllocator(
			D3D12_COMMAND_LIST_TYPE_DIRECT, 
			IID_PPV_ARGS(mCommandAllocator.GetAddressOf())));

		WTF(pDevice->CreateCommandList(
			0,
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			mCommandAllocator.Get(),
			NULL,
			IID_PPV_ARGS(mCommandList.GetAddressOf())));
	}

	X12CommandListManager& X12CommandListManager::GetCLIncstace() { return mInstance; }






}