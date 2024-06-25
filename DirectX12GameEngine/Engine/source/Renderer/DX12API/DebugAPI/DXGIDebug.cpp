
#include "DXGIDebug.h"

namespace ENGINE 
{
	DXGIDebug DXGIDebug::sInstance;

	DXGIDebug& DXGIDebug::GetInstance()
	{
		// TODO: insert return statement here
		return sInstance;
	}


	void DXGIDebug::Enable()
	{
		GetInterface();
		
	}

	void DXGIDebug::GetLiveObjects()
	{
		GetInterface();
		this->Get()->ReportLiveObjects(DXGI_DEBUG_ALL, (DXGI_DEBUG_RLO_FLAGS)(DXGI_DEBUG_RLO_DETAIL | DXGI_DEBUG_RLO_IGNORE_INTERNAL));

	}

	void DXGIDebug::GetInterface()
	{
		if (!GetInstance())
		{
			WTF(DXGIGetDebugInterface1(0, IID_PPV_ARGS(GetAddressOf())));

		}


	}

	

}