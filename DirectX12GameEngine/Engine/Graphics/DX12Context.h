#pragma once

namespace ENGINE 
{

	class ENGINE_API DX12Context
	{
	public:
		DX12Context() = default;
		static HRESULT compile_from_file(
			LPCSTR filename,
			LPCSTR entry,
			LPCSTR version,
			_Out_ ID3DBlob** pp_blob
		);

	};



}
