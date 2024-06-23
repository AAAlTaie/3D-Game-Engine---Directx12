#include "pch.h"
#include "DXGIFactory.h"
#include "Renderer/DirectX12/Debug/DirectXDebug.h"



namespace ALTAIE
{
    using namespace Microsoft::WRL;

    DXGIFactory::DXGIFactory() : ComPtr<IDXGIFactory2>()
    {
        // Create the DXGIFactory with debug flag
        WTF(CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&ptr_)));
    }
    DXGIAdapter DXGIFactory::GetAdapter()
    {
        ComPtr<IDXGIFactory6> tempFactory_6;
        ComPtr<IDXGIAdapter> gpuAdapter;

        if(Get()->QueryInterface(IID_PPV_ARGS(&tempFactory_6)) == S_OK )
        {
            //PRINT_N(" Getting Adapter was Succesful");
            HRESULT res = tempFactory_6->EnumAdapterByGpuPreference(0, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&gpuAdapter));

            switch (res)
            {
            case S_OK:
                return DXGIAdapter(gpuAdapter.Get());
                break;
            case DXGI_ERROR_NOT_FOUND:
                THROWUP_AN_ERROR("DXGI Adapter not found!");
                break;

            default:
                THROWUP_AN_ERROR("Unknown DXGI Adapter error!");
                break;
            }
        }

        //Create a Handle if it breaks passes back the addapter 
        WTF(Get()->EnumAdapters(0, &gpuAdapter));
        return DXGIAdapter(gpuAdapter.Get());

    }

    //DXGIAdapter DXGIFactory::GetAdapter()
    //{
    //    HRESULT hr;
    //    ComPtr<IDXGIAdapter1> adapter;

    //    for (UINT adapterIndex = 0;
    //        DXGI_ERROR_NOT_FOUND != ptr_->EnumAdapters1(adapterIndex, &adapter);
    //        ++adapterIndex)
    //    {
    //        DXGI_ADAPTER_DESC1 desc;
    //        adapter->GetDesc1(&desc);

    //        // Skip software adapters
    //        if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
    //            continue;

    //        // Check for a compatible Direct3D 12 device
    //        hr = D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr);
    //        if (SUCCEEDED(hr))
    //        {
    //            // Adapter is suitable, store it and break
    //            adapter_ = adapter;
    //            break;
    //        }
    //    }

    //    if (!adapter_)
    //    {
    //        std::cerr << "No suitable adapter found." << std::endl;
    //        throw std::runtime_error("Failed to find a suitable adapter");
    //    }

    //    // Assuming DXGIAdapter has a constructor that takes a ComPtr<IDXGIAdapter1>
    //    return DXGIAdapter(adapter_.Get());
    //}
}


    //DXGIAdapter::DXGIFactory::GetAdapter()
    //{
    //    HRESULT hr;
    //    ComPtr<IDXGIAdapter1> adapter;

    //    for (UINT adapterIndex = 0;
    //        DXGI_ERROR_NOT_FOUND != ptr_->EnumAdapters1(adapterIndex, &adapter);
    //        ++adapterIndex)
    //    {
    //        DXGI_ADAPTER_DESC1 desc;
    //        adapter->GetDesc1(&desc);

    //        // Skip software adapters
    //        if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
    //            continue;

    //        // Check for a compatible Direct3D 12 device
    //        hr = D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr);
    //        if (SUCCEEDED(hr))
    //        {
    //            // Adapter is suitable, store it and break
    //            adapter_ = adapter;
    //            break;
    //        }
    //    }

    //    if (!adapter_)
    //    {
    //        std::cerr << "No suitable adapter found." << std::endl;
    //        throw std::runtime_error("Failed to find a suitable adapter");
    //    }
    //}


//namespace ALTAIE
//{
//	using namespace Microsoft::WRL;
//
//	DXGIFactory::DXGIFactory() : ComPtr<IDXGIFactory2>()
//	{
//
//		//HRESULT res  = CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&ptr_));
//		WTF(CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&ptr_)));
//
//	}
//
//	void DXGIFactory::GetAdapter()
//	{
//
//
//	}
//
//}