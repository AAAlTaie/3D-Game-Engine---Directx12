#include "X12Device.h"


namespace ENGINE 
{
	
	X12Device::X12Device(IDXGIAdapter* pAdapter)
	{
		InitializeDevice(pAdapter);
	}

	void X12Device::InitializeDevice(IDXGIAdapter* pAdapter)
	{
		WTF(D3D12CreateDevice(
			pAdapter, 
			D3D_FEATURE_LEVEL_11_0, 
			IID_PPV_ARGS(GetAddressOf())));
     
	}
	X12Device::~X12Device()
	{
		this->ptr_->Release();
	}

}