#pragma once
#include "pch/pch.h"


namespace ENGINE
{
	class X12ResStateManager
	{
	public:
		X12ResStateManager() = default;
		~X12ResStateManager();

		static void TransitionResource(
			ID3D12GraphicsCommandList* pGraphicsCommand,
			const D3D12_RESOURCE_STATES stateBefore,
			const D3D12_RESOURCE_STATES stateAfter,
			ID3D12Resource* pResource,
			const UINT subResource = 0
		);

	private:
		//X12ResStateManager() = default;
		//~X12ResStateManager();

	public:
			// RESOURCES STATE MANAGER INSTANCE
		static X12ResStateManager& GetResourceInstance();

	private:
		static X12ResStateManager mInstance;

	};
}
