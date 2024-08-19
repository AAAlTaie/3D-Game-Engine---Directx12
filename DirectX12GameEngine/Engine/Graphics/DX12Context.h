#pragma once

namespace ENGINE 
{

	class ENGINE_API DX12Context
	{
	public:
		DX12Context() = default;
		void initialize(ID3D12Device* main_device);

		static HRESULT compile_from_file(
			LPCSTR filename,
			LPCSTR entry,
			LPCSTR version,
			_Out_ ID3DBlob** pp_blob
		);

		static ID3D12Device* shared_device;
		static ID3D12CommandQueue* cmd_queue;
		static ID3D12CommandAllocator* cmd_allocator;
		static ID3D12GraphicsCommandList* cmd_list;

		static ID3D12Fence* sub_fence;
		static uint64       sub_fence_value;
		static HANDLE       sub_fence_event;
	};



}
