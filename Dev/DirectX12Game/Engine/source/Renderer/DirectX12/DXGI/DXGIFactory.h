#pragma once
#include "DXGIAdapter.h"

namespace ALTAIE
{
    class DXGIFactory : public Microsoft::WRL::ComPtr<IDXGIFactory2>
    {
    public:
        DXGIFactory();

        DXGIAdapter GetAdapter();

    private:
        Microsoft::WRL::ComPtr<IDXGIAdapter1> adapter_;
    };
}

//namespace ALTAIE
//{
//
//	class DXGIFactory : Microsoft::WRL::ComPtr<IDXGIFactory2>
//	{
//	public:
//		DXGIFactory();
//
//		void GetAdapter();
//
//	};
//
//
//
//
//}

