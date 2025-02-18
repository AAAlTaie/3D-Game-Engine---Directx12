#include "pch.h"
#include "ConstantBuffer.h"
#include "Graphics/DX12Context.h"

namespace ENGINE 
{
	ConstantBuffer::ConstantBuffer(const ConstantBufferInfo* info)
	{

		m_aligment_size = DX12Context::get_constant_aligned_size(info->cb_size);
		uint total_size = info->buffer_count * m_aligment_size;

		auto device = DX12Context::shared_device;

		HR(device->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(total_size),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&m_buffer)
		));

		//open the map
		m_buffer->Map(0, nullptr, (void**)&m_mapped_data);


	}

	ConstantBuffer::~ConstantBuffer()
	{
		if (m_buffer)
		{
			m_buffer->Unmap(0, nullptr);
		}
		safe_release(m_buffer);
	}

	void ConstantBuffer::map(uint index, void* p_data, uint size)
	{

		memcpy(&m_mapped_data[index * m_aligment_size], p_data, size);

	}

	D3D12_GPU_VIRTUAL_ADDRESS ConstantBuffer::get_gpu_address(uint index)
	{
		return m_buffer->GetGPUVirtualAddress() + (index * m_aligment_size);
	}


	/*void ConstantBuffer::map(void* p_data, uint size)
	{
		memcpy(m_mapped_data, p_data, size);
	}

	D3D12_GPU_VIRTUAL_ADDRESS ConstantBuffer::get_gpu_address()
	{
		return m_buffer->GetGPUVirtualAddress();
	}*/

}