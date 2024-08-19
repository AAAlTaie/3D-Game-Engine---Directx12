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

		sub_fence_value = 1;
		sub_fence_event = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		if (sub_fence_event == nullptr)
		{
			throw std::runtime_error("Failed to create fence event.");
		}

	};

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

}