#include "X12ResStateManager.h"

namespace ENGINE
{
	X12ResStateManager X12ResStateManager::mInstance;

	void X12ResStateManager::TransitionResource(
		ID3D12GraphicsCommandList* pGraphicsCommand,
		const D3D12_RESOURCE_STATES stateBefore,
		const D3D12_RESOURCE_STATES stateAfter,
		ID3D12Resource* pResource,
		const UINT subResource
	)
	{
		D3D12_RESOURCE_BARRIER barrier; //{}
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier.Transition.pResource = pResource;
		barrier.Transition.Subresource = subResource;
		barrier.Transition.StateBefore = stateBefore;
		barrier.Transition.StateAfter = stateAfter;

		pGraphicsCommand->ResourceBarrier(1, &barrier);
	}

	X12ResStateManager& X12ResStateManager::GetResourceInstance()
	{
		return mInstance;
	}
}
