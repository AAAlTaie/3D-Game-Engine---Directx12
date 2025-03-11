#pragma once
#include "pch.h"
#include <unordered_map>

using namespace Microsoft::WRL;

enum class PipelineType
{
	IMAGE_2D, 
	SCENE_3D,
	PARTICLE_2D,
	PARTICLE_3D,
	LINE_DRAWING,
	RANDOM_GEOMETRY

};


class PipelineManager
{
public:
	static PipelineManager& GetInstance();
	ComPtr<ID3D12PipelineState> GetPipelineState(PipelineType type);

	void Initialize(ID3D12Device* device, ID3D12RootSignature* rootSignature);
	void Destroy();


private:

	PipelineManager() = default;
	~PipelineManager() = default;

	PipelineManager(const PipelineManager&) = delete;
	PipelineManager& operator = (const PipelineManager&) = delete;

	ComPtr<ID3D12PipelineState> CreatePipelineState(PipelineType type);

	ID3D12Device* m_device = nullptr;
	ID3D12RootSignature* m_rootSignature = nullptr; 
	std::unordered_map<PipelineType, ComPtr<ID3D12PipelineState>> m_pipelineStates;






};

