#pragma once
#include "pch/pch.h"



namespace ENGINE 
{

	class ENGINE_API DXGIFactory : public Microsoft::WRL::ComPtr<IDXGIFactory2>
	{
	public:
		DXGIFactory();



	};



}

