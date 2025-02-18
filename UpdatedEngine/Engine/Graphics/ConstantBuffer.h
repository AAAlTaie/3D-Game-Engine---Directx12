#pragma once

namespace ENGINE 
{
	struct ConstantBufferInfo 
	{
	
		void* p_data;
		uint cb_size;
		uint buffer_count;
	
	};

	class ENGINE_API ConstantBuffer 
	{
	
	public:

		 ConstantBuffer(const ConstantBufferInfo* info);
		~ConstantBuffer();

		void map(uint index, void* p_data, uint size);
		D3D12_GPU_VIRTUAL_ADDRESS get_gpu_address(uint index = 0);

	private:
		ID3D12Resource* m_buffer {};
		byte* m_mapped_data{};
		uint m_aligment_size{};
	};
	
	//TODO

}



