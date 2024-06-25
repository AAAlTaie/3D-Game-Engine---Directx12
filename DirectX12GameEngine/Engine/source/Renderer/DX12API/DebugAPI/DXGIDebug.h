#pragma once
#include "pch/pch.h"

namespace ENGINE 
{

	class DXGIDebug : public Microsoft::WRL::ComPtr<IDXGIDebug>
	{
	public:
		void Enable();
		void GetLiveObjects();

	private:
		void GetInterface();

	public:
		 static DXGIDebug& GetInstance();

	private:
		DXGIDebug() {};
		DXGIDebug(const DXGIDebug&) = delete;
		void operator = (const DXGIDebug&) = delete;
		static DXGIDebug sInstance;


	};


}
