#pragma once
#include "pch/pch.h"


namespace ENGINE
{
	
	class DXGIAdapter : public Microsoft::WRL::ComPtr<IDXGIAdapter>
	{
	public:

		DXGIAdapter() noexcept {};
		~DXGIAdapter();

		DXGIAdapter(IDXGIAdapter* pAdapter)  noexcept;


	};

}


