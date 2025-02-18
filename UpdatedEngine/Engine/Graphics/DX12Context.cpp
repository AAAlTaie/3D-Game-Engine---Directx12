#include "pch.h"
#include "DX12Context.h"
#include <cwchar>
#include <cstring>

namespace ENGINE 
{
	ID3D12Device* DX12Context::shared_device{};
	ID3D12CommandQueue* DX12Context::cmd_queue{};
	ID3D12CommandAllocator* DX12Context::cmd_allocator{};
	ID3D12GraphicsCommandList* DX12Context::cmd_list{};

	ID3D12Fence* DX12Context::sub_fence{};
	uint64       DX12Context::sub_fence_value{0};
	HANDLE       DX12Context::sub_fence_event{};

	void DX12Context::initialize(ID3D12Device* main_device)
	{
		shared_device = main_device;
		D3D12_COMMAND_QUEUE_DESC cmd_desc{};
		cmd_desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		cmd_desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

		HR(shared_device->CreateCommandQueue(&cmd_desc, IID_PPV_ARGS(&cmd_queue)));
		HR(shared_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&cmd_allocator)));
		HR(shared_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, cmd_allocator, nullptr, IID_PPV_ARGS(&cmd_list)));
		HR(cmd_list->Close());
		HR(shared_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&sub_fence)));

		sub_fence_value = 0;
		sub_fence_event = CreateEvent(nullptr, FALSE, FALSE, nullptr);

		if (sub_fence_event == nullptr)
		{
			throw std::runtime_error("Failed to create fence event.");
		}
	}

	void DX12Context::release()
	{
		safe_release(cmd_list);
		safe_release(cmd_allocator);
		safe_release(cmd_queue);
		CloseHandle(sub_fence_event);
		safe_release(sub_fence);
	}

	void DX12Context::fence_signal()
	{
		sub_fence_value++;
		HR(cmd_queue->Signal(sub_fence, sub_fence_value));
	}
	void DX12Context::wait_for_gpu()
	{
		const uint64 expected_fence_value = sub_fence_value;
		if (sub_fence->GetCompletedValue() < expected_fence_value)
		{
			//if completed value = 0 we are st'll in here
			sub_fence->SetEventOnCompletion(expected_fence_value, sub_fence_event);
			::WaitForSingleObject(sub_fence_event, INFINITE);
		}
	}

	HRESULT DX12Context::create_buffers_system_memory(
		void* pData, 
		uint cbSize, 
		ID3D12Resource** ppResource)
	{

		if (*ppResource) { return E_INVALIDARG; }
		if (!pData && (cbSize <= 0)) { return E_INVALIDARG; }

		HRESULT hr = S_OK;
		CD3DX12_HEAP_PROPERTIES heap_props(D3D12_HEAP_TYPE_UPLOAD);
		CD3DX12_RESOURCE_DESC res_desc(CD3DX12_RESOURCE_DESC::Buffer(cbSize));



		hr = shared_device->CreateCommittedResource(
			&heap_props,
			D3D12_HEAP_FLAG_NONE,
			&res_desc,
			D3D12_RESOURCE_STATE_GENERIC_READ, //vs buffer, textures, index buffer....
			nullptr,
			IID_PPV_ARGS(ppResource)
		);

		ID3D12Resource* mem_resource = *ppResource;
		CD3DX12_RANGE range(0, 0);

		void* data = nullptr;
		hr = mem_resource->Map(0, &range, &data);
		memcpy(data, pData, cbSize);
		mem_resource->Unmap(0, nullptr);

		return hr;
	}
	
	HRESULT DX12Context::create_constant_buffer(
		void* p_data,
		uint cb_size,
		ID3D12Resource** pp_resource)
	{
		uint fixed_buffer_size = (cb_size + 255) & ~255;

		ID3D12Resource* constant_buffer{};
		CD3DX12_HEAP_PROPERTIES heap_props(D3D12_HEAP_TYPE_UPLOAD);
		CD3DX12_RESOURCE_DESC buffer_desc = CD3DX12_RESOURCE_DESC::Buffer(fixed_buffer_size);

		HRESULT hr = S_OK;
		hr = shared_device->CreateCommittedResource(
			&heap_props,
			D3D12_HEAP_FLAG_NONE,
			&buffer_desc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constant_buffer)
		);

		void* p_mapped_data{};
		CD3DX12_RANGE read_range(0, 0);
		constant_buffer->Map(0, &read_range, &p_mapped_data);
		memcpy(p_mapped_data, p_data, cb_size);
		constant_buffer->Unmap(0, nullptr);


		*pp_resource = constant_buffer;

		return hr;
	}

	HRESULT DX12Context::compile_from_file(
		LPCSTR filename,
		LPCSTR entry,
		LPCSTR version,
		ID3DBlob** pp_blob)
	{
		const size_t len = std::strlen(filename) + 1;
		wchar_t wide_filename[128]{};
		std::mbstowcs(wide_filename, filename, len);
		uint flag = 0;


#ifdef _DEBUG
		flag |= D3DCOMPILE_DEBUG;
#endif // _DEBUG

		ID3DBlob* error_blob{};
		HRESULT res = ::D3DCompileFromFile(
			wide_filename,
			nullptr,
			nullptr,
			entry,
			version,
			flag, 
			0,
			pp_blob, 
			&error_blob
		);
		if (FAILED(res))
		{
			MessageBoxA(NULL, (LPCSTR)error_blob->GetBufferPointer(),
				"Compile Error", MB_OK);
			safe_release(error_blob);
			

		}
		return res;

		if (error_blob)
		{
			error_blob->Release();
		}
		safe_release(error_blob);
		return S_OK;
	}

	uint DX12Context::get_constant_aligned_size(uint size)
	{
		return (size + 255) & ~255;
	}

}