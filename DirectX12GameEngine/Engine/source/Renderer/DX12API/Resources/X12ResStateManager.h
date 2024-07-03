#pragma once
#include "pch/pch.h"

namespace ENGINE
{
	class X12ResStateManager
	{
	public:
		static void TransitionResource(
			ID3D12GraphicsCommandList* pGraphicsCommand,
			const D3D12_RESOURCE_STATES stateBefore,
			const D3D12_RESOURCE_STATES stateAfter,
			ID3D12Resource* pResource,
			const UINT subResource = 0
		);

	private:
		X12ResStateManager() = default;

	public:
			// RESOURCES STATE MANAGER INSTANCE
		static X12ResStateManager& GetResourceInstance();

	private:
		static X12ResStateManager mInstance;
		
	


		//public:
		//	static void TransitionResource(
		//		ID3D12GraphicsCommandList* pGraphicsCommand,
		//		ID3D12Resource* pResource,
		//		const UINT subResource = 0,
		//		const D3D12_RESOURCE_STATES states_I = D3D12_RESOURCE_STATE_COMMON,
		//		const D3D12_RESOURCE_STATES states_II = D3D12_RESOURCE_STATE_COMMON
		//	);

		//private:
		//	X12ResStateManager() = default;

		//public:
		//	// RESOURCES STATE MANAGER INSTANCE
		//	static X12ResStateManager& GetRSMI();

		//private:
		//	static X12ResStateManager mInstance;
		//};
	};
}
