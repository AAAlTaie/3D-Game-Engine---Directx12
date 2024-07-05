#pragma once
#include "pch/pch.h"

#include <wrl/client.h> 
#include <d3d12.h>
#include <dxgi1_6.h>

namespace ENGINE 
{

	class X12Device : public Microsoft::WRL::ComPtr<ID3D12Device>
	{
	public:

		X12Device() = default;
		~X12Device();
		X12Device(IDXGIAdapter* pAdapter);

		void InitializeDevice(IDXGIAdapter* pAdapter);

	};


}
