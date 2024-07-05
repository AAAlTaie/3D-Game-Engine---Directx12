#include "DXGIAdapter.h"


namespace ENGINE 
{
	DXGIAdapter::~DXGIAdapter()
	{
		this->ptr_->Release();
	}
	DXGIAdapter::DXGIAdapter(IDXGIAdapter* pAdapter)  noexcept : ComPtr<IDXGIAdapter>(pAdapter)
	{


	}
}