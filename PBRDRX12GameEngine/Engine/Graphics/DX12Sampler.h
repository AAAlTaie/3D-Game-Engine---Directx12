#pragma once

namespace ENGINE
{

	enum SamplerType 
	{
		Liner, Point
	};
	class ENGINE_API DX12Sampler
	{
	public:
		DX12Sampler() = default;
		static D3D12_STATIC_SAMPLER_DESC get_sampler_desc(SamplerType type);

	};
}

