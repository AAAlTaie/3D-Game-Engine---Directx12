#pragma once
#include "pch/pch.h"


namespace ENGINE 
{

    class DXGISwapChain : public Microsoft::WRL::ComPtr<IDXGISwapChain1>
    {
    public:


        DXGISwapChain() = default;
        DXGISwapChain(ID3D12CommandQueue* pCommandQueue, 
                  ID3D12Device* pDevice, 
                  IDXGIFactory2* pFactory, 
            const HWND hWnd, 
            const DXGI_FORMAT format, 
            const UINT bufferCount = GNORF);
        ~DXGISwapChain();

        void InitializeSwapChain(ID3D12CommandQueue* pCommandQueue,
            ID3D12Device* pDevice,
            IDXGIFactory2* pFactory,
            const HWND hWnd,
            const DXGI_FORMAT format,
            const UINT bufferCount = GNORF
        );

        void CreateBuffers();
        void DropBuffers();

        ID3D12Resource* GetCurrentBuffer() { return mBufferArray[mCurrentBuffer].Get(); };
        D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentRTVHandle();

        inline UINT FramesInFlight() const { return  mBufferCount; };

        void Present();
        void Release();

    private:

        //ID3D12Device* mDevice = nullptr; try ID3D12Device* mDevice = 
        ID3D12Device* mDevice = nullptr;

        Microsoft::WRL::ComPtr<ID3D12Resource> mBufferArray[GNORF] = {}; //te be finished later
        Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mRtvHeap;   //render target heap

        UINT mBufferCount = 0;
        UINT mCurrentBuffer = 0;
        UINT mHeapIncrement = 0;


    };






}


