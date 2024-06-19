#include "pch.h"
#include "DXGIFactory.h"
#include "Renderer/DirectX12/Debug/DirectXDebug.h"

namespace ALTAIE
{
	using namespace Microsoft::WRL;

	DXGIFactory::DXGIFactory() : ComPtr<IDXGIFactory2>()
	{

		//HRESULT res  = CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&ptr_));
		WTF(CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&ptr_)));

	}

}