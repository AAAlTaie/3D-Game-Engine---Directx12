#pragma once
#include "pch/pch.h"
#include "DXGIAdapter.h"


namespace ENGINE 
{

	class ENGINE_API DXGIFactory : public Microsoft::WRL::ComPtr<IDXGIFactory2>
	{
	public:
		DXGIFactory();
		DXGIAdapter GetAdapter();


	};



}

