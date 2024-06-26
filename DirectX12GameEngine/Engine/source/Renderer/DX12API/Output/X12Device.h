#pragma once
#include "pch/pch.h"




namespace ENGINE 
{

	class X12Device : public Microsoft::WRL::ComPtr<ID3D12Device>
	{
	public:

		X12Device() = default;

		X12Device(IDXGIAdapter* pAdapter);



	};


}


