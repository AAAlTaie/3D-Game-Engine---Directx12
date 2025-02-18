#include "pch.h"
#include "DX12Sampler.h"

namespace ENGINE 
{

	D3D12_STATIC_SAMPLER_DESC DX12Sampler::get_sampler_desc(SamplerType type)
	{
		D3D12_STATIC_SAMPLER_DESC desc{};
		desc.Filter =  type == SamplerType::Liner ? 
			D3D12_FILTER_MIN_MAG_MIP_LINEAR : D3D12_FILTER_MIN_MAG_MIP_POINT;
		desc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		desc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		desc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		desc.MipLODBias = 0.0f;
		desc.MaxAnisotropy = 16;
		desc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		desc.BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE;
		desc.MinLOD = -FLT_MAX;
		desc.MaxLOD = D3D12_FLOAT32_MAX;
		desc.ShaderRegister = 0;  ///--- regisdter s0
		desc.RegisterSpace = 0;
		desc.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		return desc;
	}

}