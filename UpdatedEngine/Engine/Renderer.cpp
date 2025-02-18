#include "pch.h"
#include "Renderer.h"
#include "Graphics/DX12Context.h"

namespace ENGINE
{
    Renderer::Renderer(const renderer_create_info& info)
	{

		m_hwnd = info.hwnd;
        m_VSync = info.vsync;

        CreateDevice();
        CreateCommands();

        if (m_command_queue == nullptr)
        {
            std::cout << "Command Queue initailization failed";
            return;
        }

        CreateSwapChain(info.cx, info.cy);
        CreateDescriptorHeaps();
        CreateRenderTargets();
        CreateDepthStencils(info.cx, info.cy);
        CreateFence();

        //Subresource upload (srv, uav, cbv_ resources
        DX12Context::initialize(m_device);

	}

	void Renderer::CreateDevice()
	{
        
        DWORD factory_flag = 0;

       #if defined(_DEBUG)
        ID3D12Debug3* debug_controller{};
        HR(D3D12GetDebugInterface(IID_PPV_ARGS(&debug_controller)));
        debug_controller->EnableDebugLayer();

        factory_flag |= DXGI_CREATE_FACTORY_DEBUG;
        safe_release(debug_controller);
        #endif

        HR(CreateDXGIFactory2(factory_flag, IID_PPV_ARGS(&m_factory)));

        IDXGIAdapter4* gpu_adapter = get_suitable_gpu_adapter();

        auto max_feature = get_max_feature_level(gpu_adapter); //gtx950 _11 dx12 rtx4060 12_1

        //Device Creation
        HR(D3D12CreateDevice(gpu_adapter, max_feature, IID_PPV_ARGS(&m_device)));
        set_name(m_device, "DX12::m_device");
        safe_release(gpu_adapter)
	}

    IDXGIAdapter4* Renderer::get_suitable_gpu_adapter() 
    {
        IDXGIAdapter4* fixed_adapter{};
        for (uint i = 0; m_factory->EnumAdapterByGpuPreference(i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&fixed_adapter)) != DXGI_ERROR_NOT_FOUND; ++i)
        {
            if (SUCCEEDED(D3D12CreateDevice(fixed_adapter, min_features, __uuidof(IDXGIAdapter4), nullptr)))
            {
                return fixed_adapter;
            }
            safe_release(fixed_adapter);
        }

        return nullptr;
    }

    D3D_FEATURE_LEVEL Renderer::get_max_feature_level(IDXGIAdapter4* fixed_adapter)
    {
        D3D_FEATURE_LEVEL feature_levels[] = {
            D3D_FEATURE_LEVEL_11_0,
            D3D_FEATURE_LEVEL_11_1,
            D3D_FEATURE_LEVEL_12_0,
            D3D_FEATURE_LEVEL_12_1

        };

        D3D12_FEATURE_DATA_FEATURE_LEVELS feature_level_info{};
        feature_level_info.NumFeatureLevels = _countof(feature_levels);
        feature_level_info.pFeatureLevelsRequested = feature_levels;

        ID3D12Device* tempDevice{};
        HR(D3D12CreateDevice(fixed_adapter, min_features, IID_PPV_ARGS(&tempDevice)));
        HR(tempDevice->CheckFeatureSupport(D3D12_FEATURE_FEATURE_LEVELS, &feature_level_info, sizeof(feature_level_info)));
        safe_release(tempDevice);
        return feature_level_info.MaxSupportedFeatureLevel;
    }

    ID3D12Device* Renderer::get_device() const
    {
        return m_device;
    }

    ID3D12GraphicsCommandList* Renderer::get_command_list() const
    {
        return m_command_list;
    }

    D3D12_VIEWPORT Renderer::get_viewport() const
    {
        return m_viewport;
    }

    void Renderer::CreateCommands()
    {
        D3D12_COMMAND_QUEUE_DESC desc{};
        desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

        HR(m_device->CreateCommandQueue(&desc, IID_PPV_ARGS(&m_command_queue)));
        set_name(m_command_queue, "SubRenderer::CreateCommands-> m_command_queue");
        HR(m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,IID_PPV_ARGS(&m_command_allocator)));
        set_name(m_command_allocator, "SubRenderer::CreateCommands-> m_command_allocator");
        HR(m_device->CreateCommandList(1, D3D12_COMMAND_LIST_TYPE_DIRECT, m_command_allocator, nullptr, IID_PPV_ARGS(&m_command_list)));
        set_name(m_command_list, "SubRenderer::CreateCommands-> m_command_list");
        HR(m_command_list->Close());
    }

    void Renderer::CreateSwapChain(uint cx, uint cy)
    {
        safe_release(m_swapchain);

        DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
        swapChainDesc.BufferCount = frame_buffer_count;
        swapChainDesc.Width = cx;
        swapChainDesc.Height = cy;
        swapChainDesc.Format = frame_buffer_format;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
        swapChainDesc.SampleDesc.Count = 1;                 
        swapChainDesc.SampleDesc.Quality = 0;

        IDXGISwapChain1* fixed_swapchain {};
        HR(m_factory->CreateSwapChainForHwnd(m_command_queue, m_hwnd, &swapChainDesc, nullptr, nullptr, &fixed_swapchain));
        HR(fixed_swapchain->QueryInterface(IID_PPV_ARGS(& m_swapchain)));
        safe_release(fixed_swapchain);
        m_back_buffer_index = m_swapchain->GetCurrentBackBufferIndex();

        m_viewport.Width = (float)cx;
        m_viewport.Height = (float)cy;
        m_viewport.MinDepth = 0.0f;
        m_viewport.MaxDepth = 1.0f;
        m_viewport.TopLeftX = 0.0f;
        m_viewport.TopLeftY = 0.0f;

        m_scissors.right = cx;
        m_scissors.bottom = cy;
    }

    void Renderer::CreateDescriptorHeaps()
    {
        D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
        rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        rtvHeapDesc.NumDescriptors = frame_buffer_count;
        rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        rtvHeapDesc.NodeMask = 0;

        HR(m_device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_rtv_heap)));
        m_rtv_offsets = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

        D3D12_DESCRIPTOR_HEAP_DESC m_dsv_desc{};
        m_dsv_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
        m_dsv_desc.NumDescriptors = frame_buffer_count;
        m_dsv_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        m_dsv_desc.NodeMask = 0;

        HR(m_device->CreateDescriptorHeap(&m_dsv_desc, IID_PPV_ARGS(&m_dsv_heap)));
        set_name(m_dsv_heap, "SubRenderer::CreateDescriptorHeaps-> m_dsv_heap");
        m_dsv_offsets = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

    }

    void Renderer::CreateRenderTargets()
    {
        CD3DX12_CPU_DESCRIPTOR_HANDLE rtv_handle(
            m_rtv_heap->GetCPUDescriptorHandleForHeapStart());
        for (uint i = 0; i < frame_buffer_count; ++i)
        {
            HR(m_swapchain->GetBuffer(i, IID_PPV_ARGS(&m_rtv[i])));
            m_device->CreateRenderTargetView(m_rtv[i], nullptr, rtv_handle);
            rtv_handle.Offset(1, m_rtv_offsets);


        }
    }

    void Renderer::CreateDepthStencils(uint cx, uint cy)
    {
        D3D12_DEPTH_STENCIL_VIEW_DESC depthStencilDesc{};
        depthStencilDesc.Format = DXGI_FORMAT_D32_FLOAT;
        depthStencilDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
        depthStencilDesc.Flags = D3D12_DSV_FLAG_NONE;

        D3D12_CLEAR_VALUE clearValue{};
        clearValue.Format = DXGI_FORMAT_D32_FLOAT;
        clearValue.DepthStencil.Depth = 1.0f;
        clearValue.DepthStencil.Stencil = 0;

        CD3DX12_RESOURCE_DESC depthDesc(
            D3D12_RESOURCE_DIMENSION_TEXTURE2D,
            0,
            cx,
            cy,
            1,
            1,
            DXGI_FORMAT_R32_TYPELESS,
            1,
            0,
            D3D12_TEXTURE_LAYOUT_UNKNOWN,
            D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
        );

        //GPU UPLOAD
        HR(m_device->CreateCommittedResource(
            &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
            D3D12_HEAP_FLAG_NONE,
            &depthDesc,
            D3D12_RESOURCE_STATE_DEPTH_WRITE,
            &clearValue,
            IID_PPV_ARGS(&m_dsv))
        );

        set_name(m_dsv, "SubRenderer::CreateDepthStencils->(DepthStencilView)");
        CD3DX12_CPU_DESCRIPTOR_HANDLE dsv_handle(m_dsv_heap->GetCPUDescriptorHandleForHeapStart());
        m_device->CreateDepthStencilView(m_dsv, &depthStencilDesc, dsv_handle);
        
    }

    void Renderer::CreateFence()
    {
        if (m_device == nullptr)
        {
            std::cerr << "Device is not initialized" << std::endl;
            return;
        }

        // Create a fence with initial value of 0
        HRESULT hr = m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence));
        if (FAILED(hr))
        {
            std::cerr << "Failed to create fence. HRESULT: " << hr << std::endl;
            return;
        }

        m_fence_value = 0;
        m_fence_event = CreateEvent(nullptr, FALSE, FALSE, nullptr);
        if (m_fence_event == nullptr)
        {
            std::cerr << "Failed to create event. Error: " << GetLastError() << std::endl;
            return;
        }
    }

    uint64 Renderer::GPUFrameSignal()
    {
        if (!m_command_queue || !m_fence)
        {
            std::cerr << "m_command_queue or m_fence is not initialized" << std::endl;
            return 0;
        }
        m_fence_value++;

        // Signal the command queue
        HRESULT hr = m_command_queue->Signal(m_fence, m_fence_value);
        if (FAILED(hr))
        {
            std::cerr << "Failed to signal command queue. HRESULT: " << hr << std::endl;
            return 0;
        }

        return m_fence_value;
    }

    void Renderer::WaitForGPUAsync()
    {
        if (!m_fence || !m_fence_event)
        {
            std::cerr << "m_fence or m_fence_event is not initialized" << std::endl;
            return;
        }

        const uint64 expected_fence_value = m_fence_value;

        // To check if the GPU has completed work for the current fence value
        if (m_fence->GetCompletedValue() < expected_fence_value)
        {
            // Set an event to signal when the fence value is reached
            HRESULT hr = m_fence->SetEventOnCompletion(expected_fence_value, m_fence_event);
            if (FAILED(hr))
            {
                std::cerr << "Failed to set event on completion. HRESULT: " << hr << std::endl;
                return;
            }

            // Wait for the event to be signaled
            DWORD wait_result = WaitForSingleObject(m_fence_event, INFINITE);
            if (wait_result != WAIT_OBJECT_0)
            {
                std::cerr << "WaitForSingleObject failed with error: " << GetLastError() << std::endl;
                return;
            }
        }
    }

    void Renderer::begin_frame()
    {
        m_command_allocator->Reset();
        m_command_list->Reset(m_command_allocator, nullptr);

        //Render Target
        CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(
            m_rtv_heap->GetCPUDescriptorHandleForHeapStart(),
            m_back_buffer_index,
            m_rtv_offsets         
        );
        m_command_list->ResourceBarrier(
            1,
            &CD3DX12_RESOURCE_BARRIER::Transition(
                m_rtv[m_back_buffer_index],
                D3D12_RESOURCE_STATE_PRESENT,
                D3D12_RESOURCE_STATE_RENDER_TARGET

            ));

        //Depth Stencils
        CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHanle(m_dsv_heap->GetCPUDescriptorHandleForHeapStart());
        m_command_list->RSSetViewports(1, &m_viewport);
        m_command_list->RSSetScissorRects(1, &m_scissors);

        const float clear_def_value[] = { 0.2f, 0.2f, 0.2f, 1.0f };
        m_command_list->ClearRenderTargetView(rtvHandle, clear_def_value, 0, nullptr);
        m_command_list->ClearDepthStencilView(dsvHanle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
        m_command_list->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHanle);

        //NOW WE CAN DRAW SHIT!!!!


    }

    void Renderer::end_frame()
    {
        m_command_list->ResourceBarrier(
            1,
            &CD3DX12_RESOURCE_BARRIER::Transition(
                m_rtv[m_back_buffer_index],
                D3D12_RESOURCE_STATE_RENDER_TARGET,
                D3D12_RESOURCE_STATE_PRESENT

            ));
        m_command_list->Close();

        ID3D12CommandList* cmd_list[] = { m_command_list };
        m_command_queue->ExecuteCommandLists(_countof(cmd_list), cmd_list);

        uint flag = 0;
        uint vsync_interval = m_VSync ? 1 : 0;
        m_swapchain->Present(vsync_interval, flag);
        m_back_buffer_index = m_swapchain->GetCurrentBackBufferIndex();

        GPUFrameSignal();
        WaitForGPUAsync();
        
       
         
    }

    void Renderer::reset()
    {
        //Lett'm get up -> 
        WaitForGPUAsync();

        // Check before releasing
        std::cout << "Releasing resources..." << std::endl;
        std::cout << "Command List: " << m_command_list << std::endl;
        std::cout << "Command Allocator: " << m_command_allocator << std::endl;
        std::cout << "Command Queue: " << m_command_queue << std::endl;
        std::cout << "Swapchain: " << m_swapchain << std::endl;

        // Release resources
        safe_release(m_command_list);
        safe_release(m_command_allocator);
        safe_release(m_command_queue);
        safe_release(m_swapchain);

        for (uint i = 0; i < frame_buffer_count; ++i)
        {
            std::cout << "RTV[" << i << "]: " << m_rtv[i] << std::endl;
            safe_release(m_rtv[i]);
        }
        safe_release(m_rtv_heap);
        std::cout << "DSV: " << m_dsv << std::endl;
        safe_release(m_dsv);
        safe_release(m_dsv_heap);

        std::cout << "Device: " << m_device << std::endl;
        std::cout << "Factory: " << m_factory << std::endl;
        safe_release(m_device);
        safe_release(m_factory);

        safe_release(m_fence);
        if (m_fence_event)
        {
            CloseHandle(m_fence_event);
            
        }

        DX12Context::release();

    }


    Renderer::~Renderer()
    {
        
    }

}