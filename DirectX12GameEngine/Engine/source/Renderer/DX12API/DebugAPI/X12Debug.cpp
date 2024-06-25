#include "X12Debug.h"
#include <stdexcept>

namespace ENGINE 
{

	X12Debug X12Debug::sInstance;

	void X12Debug::Enable()
	{
		GetInterface();
		if (this->Get() == nullptr) {
			throw std::runtime_error("Failed to get ID3D12Debug interface.");
		}
		this->Get()->EnableDebugLayer();
		mIsEnabled = true;
	}

	void X12Debug::GetInterface()
	{
		if (!*this)
		{
			HRESULT hr = D3D12GetDebugInterface(IID_PPV_ARGS(this->GetAddressOf()));
			if (FAILED(hr))
			{
				WTF(D3D12GetDebugInterface(IID_PPV_ARGS(this->GetAddressOf())));
				throw std::runtime_error("D3D12GetDebugInterface failed.");
			}
		}

		// Create the D3D12 device with feature level 12_2
		Microsoft::WRL::ComPtr<ID3D12Device> device;
		HRESULT hr = D3D12CreateDevice(
			nullptr,                     // Use the default adapter
			D3D_FEATURE_LEVEL_12_2,      // Request D3D_FEATURE_LEVEL_12_2
			IID_PPV_ARGS(&device)
		);

		if (FAILED(hr))
		{
			throw std::runtime_error("D3D12CreateDevice failed. Feature level 12_2 not supported.");
		}
	}

	X12Debug& X12Debug::GetX12DebugInstance()  // Renamed to avoid conflict
	{
		return sInstance;
	}

}