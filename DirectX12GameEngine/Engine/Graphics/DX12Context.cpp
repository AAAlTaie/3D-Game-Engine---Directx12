#include "pch.h"
#include <cwchar>
#include <cstring>
#include "DX12Context.h"

namespace ENGINE 
{




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
			flag, 0,
			pp_blob, &error_blob
		);
		if (FAILED(res))
		{
			MessageBoxA(NULL, (LPCSTR)error_blob->GetBufferPointer(),
				"Compile Error", MB_OK);
			safe_release(error_blob);
			return res;

		}
		safe_release(error_blob);
		return S_OK;
	}

}