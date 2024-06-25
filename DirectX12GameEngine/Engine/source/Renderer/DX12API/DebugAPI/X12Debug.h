#pragma once
#include "pch/pch.h"



namespace ENGINE 
{

	class X12Debug : public Microsoft::WRL::ComPtr<ID3D12Debug>
	{

	public:
		void Enable();

	private:
		void GetInterface();

	public:
		static X12Debug& GetX12DebugInstance();  // Renamed to avoid conflict

	private:
		X12Debug() {};
		X12Debug(const X12Debug&) = delete;
		void operator=(const X12Debug&) = delete;
		static X12Debug sInstance;

	private:
		bool mIsEnabled = false;



	};





}

