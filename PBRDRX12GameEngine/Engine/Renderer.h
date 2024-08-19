#pragma once

#define min_features D3D_FEATURE_LEVEL_11_0
#define frame_buffer_count 3
#define frame_buffer_format DXGI_FORMAT_R8G8B8A8_UNORM

namespace ENGINE 
{
   class ENGINE_API Renderer
   {
   public:
        Renderer();
       ~Renderer();
       void Initialize(uint cx, uint cy, HWND hund, bool vsync = true);
       void reset();



       uint64 GPUFrameSignal();
       void WaitForGPUAsync();
       //External Function
       void begin_frame();
       void end_frame();


   protected:

       ID3D12Device*                m_device{};
       IDXGIFactory7*               m_factory{};


       ID3D12CommandQueue*          m_command_queue{};
       ID3D12CommandAllocator*      m_command_allocator{};
       ID3D12GraphicsCommandList*   m_command_list{};

       //Swapchain
       IDXGISwapChain3*             m_swapchain{};

       //Render Target View Resources EX (diffuseRTVHandle, normalRTVHandle, specularRTVHandle, depthRTVHandle)
       ID3D12Resource*              m_rtv[frame_buffer_count]{}; 
       ID3D12Resource*              m_dsv{};                     //depthStencilView
       uint                         m_back_buffer_index{ 0 };

       //Descs HEAP
       ID3D12DescriptorHeap*        m_rtv_heap{};
       ID3D12DescriptorHeap*        m_dsv_heap{};
       uint                         m_rtv_offsets{};
       uint                         m_dsv_offsets{};

       //Members
       HWND                         m_hwnd{};
       D3D12_VIEWPORT               m_viewport{ 0.0f };
       D3D12_RECT                   m_scissors{ 0 };

       uint64                       m_fence_value{};
       HANDLE                       m_fence_event{};

       void CreateDevice();
       void CreateCommands();
       void CreateSwapChain(uint cx, uint cy);
       //TODO : Create RESIZE Swap chain
       void CreateDescriptorHeaps();
       void CreateRenderTargets();
       void CreateDepthStencils(uint cx, uint cy);
       void CreateFence();

       //FENCING
       ID3D12Fence*                 m_fence{};


   private:
       IDXGIAdapter4* get_suitable_gpu_adapter();
       D3D_FEATURE_LEVEL get_max_feature_level(IDXGIAdapter4* fixed_adapter);
       bool m_VSync = false;


   public:
       ID3D12Device* get_device() const;
       ID3D12GraphicsCommandList* get_command_list() const;
       //ID3D12DescriptorHeap* get_

   };


}

