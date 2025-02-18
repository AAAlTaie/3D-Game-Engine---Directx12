#pragma once

namespace ENGINE 
{

	class ENGINE_API DX12Context
	{
	public:
		DX12Context() = default;
		static void initialize(ID3D12Device* main_device);
		static void release();
		static void fence_signal();
		static void wait_for_gpu();

		static HRESULT create_buffers_system_memory(
			void* pData,
			uint cbSize,
			ID3D12Resource** ppResource
		);

		static HRESULT create_constant_buffer(
			void* p_data,
			uint cb_size,
			ID3D12Resource** pp_resource);

		static HRESULT compile_from_file(
			LPCSTR filename,
			LPCSTR entry,
			LPCSTR version,
		    ID3DBlob** pp_blob
		);

		static uint get_constant_aligned_size(uint size);

		static ID3D12Device* shared_device;
		static ID3D12CommandQueue* cmd_queue;
		static ID3D12CommandAllocator* cmd_allocator;
		static ID3D12GraphicsCommandList* cmd_list;

		static ID3D12Fence* sub_fence;
		static uint64       sub_fence_value;
		static HANDLE       sub_fence_event;
	};



}
