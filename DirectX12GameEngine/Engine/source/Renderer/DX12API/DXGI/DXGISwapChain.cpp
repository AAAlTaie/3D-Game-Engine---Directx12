#include "DXGISwapChain.h"

/*
    enum DXGI_SWAP_EFFECT
    {
        DXGI_SWAP_EFFECT_DISCARD	= 0,
        DXGI_SWAP_EFFECT_SEQUENTIAL	= 1,
        DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL	= 3,
        DXGI_SWAP_EFFECT_FLIP_DISCARD	= 4
    } 	DXGI_SWAP_EFFECT;
	
*/


namespace ENGINE 
{
	DXGISwapChain::DXGISwapChain(
		      ID3D12CommandQueue* pCommandQueue,
		      ID3D12Device* pDevice,
		      IDXGIFactory2* pFactory,
		const HWND hWnd,
		const DXGI_FORMAT format,
		const UINT bufferCount
	)
	{

	
		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDescriptor = {};
		rtvHeapDescriptor.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtvHeapDescriptor.NumDescriptors = bufferCount;
		rtvHeapDescriptor.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		rtvHeapDescriptor.NodeMask = 0;

		//Create the Desc Heap 
		WTF(pDevice->CreateDescriptorHeap(&rtvHeapDescriptor, IID_PPV_ARGS(mRtvHeap.GetAddressOf())));

		//Specify the incremented values.
		mHeapIncrement = pDevice->GetDescriptorHandleIncrementSize(rtvHeapDescriptor.Type);


		mBufferCount = bufferCount;
		RECT wClientRect = {};
		GetWindowRect(hWnd, &wClientRect);

		//Describe the swap chain
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };
		swapChainDesc.Width = wClientRect.right - wClientRect.left;
		swapChainDesc.Height = wClientRect.bottom - wClientRect.top;
		swapChainDesc.Format = format;
		swapChainDesc.Stereo = false;
		swapChainDesc.SampleDesc = { 1, 0 };
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = bufferCount;
		swapChainDesc.Scaling = DXGI_SCALING_NONE;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
		swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
		swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		WTF(pFactory->CreateSwapChainForHwnd(
			pCommandQueue,
			hWnd,
			&swapChainDesc,
			NULL,
			NULL,
			&ptr_
		));

		mDevice = pDevice;
		CreateBuffers();
	}

	DXGISwapChain::~DXGISwapChain()
	{
		Release();

	}

	void DXGISwapChain::CreateBuffers()
	{
		//this will be an asset for later
		if (!ptr_)
		{
			THROWUP_AN_ERROR("Trying to create buffers on an empty SwapChain!");
			for (UINT i = 0; i < mBufferCount; ++i)
			{
				mBufferArray[i].Reset();
				WTF(Get()->GetBuffer(i, IID_PPV_ARGS(mBufferArray[i].GetAddressOf())));

				//CPUHandles
				D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = mRtvHeap->GetCPUDescriptorHandleForHeapStart();
				cpuHandle.ptr += (size_t)mHeapIncrement * i;
				mDevice->CreateRenderTargetView(mBufferArray[i].Get(), NULL, cpuHandle);

			}


		}
	}

	void DXGISwapChain::DropBuffers()
	{

		for (UINT i = 0; i < mBufferCount; ++i)
		{
			mBufferArray[i].Reset();

		}



	}

	void DXGISwapChain::Present() //// buffer
	{
		ptr_->Present(NULL, NULL);
		mCurrentBuffer = (mCurrentBuffer + 1) % mBufferCount; ///increas for more buffers
	}

	void DXGISwapChain::Release()
	{
		DropBuffers();
		mRtvHeap.Reset();
		Reset();
	}


}