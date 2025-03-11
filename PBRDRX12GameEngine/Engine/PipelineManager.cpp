#include "pch.h"
#include "PipelineManager.h"

// Singleton instance retrieval
PipelineManager& PipelineManager::GetInstance()
{
    static PipelineManager instance;
    return instance;
}

// Initialize the pipeline manager with device and root signature
void PipelineManager::Initialize(ID3D12Device* device, ID3D12RootSignature* rootSignature)
{
    m_device = device;
    m_rootSignature = rootSignature;
}

// Destroys all pipeline states
void PipelineManager::Destroy()
{
    m_pipelineStates.clear();
}

// Retrieve or create a pipeline state
ComPtr<ID3D12PipelineState> PipelineManager::GetPipelineState(PipelineType type)
{
    auto it = m_pipelineStates.find(type);
    if (it != m_pipelineStates.end())
    {
        return it->second;
    }

    // Create a new pipeline state if it doesn't exist
    ComPtr<ID3D12PipelineState> pipelineState = CreatePipelineState(type);
    m_pipelineStates[type] = pipelineState;
    return pipelineState;
}

// Create a new pipeline state based on the type
ComPtr<ID3D12PipelineState> PipelineManager::CreatePipelineState(PipelineType type)
{
    if (!m_device || !m_rootSignature)
    {
        throw std::runtime_error("PipelineManager not initialized with device and root signature.");
    }

    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
    psoDesc.pRootSignature = m_rootSignature;
    psoDesc.SampleMask = UINT_MAX;
    psoDesc.SampleDesc.Count = 1;
    psoDesc.NumRenderTargets = 1;
    psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    psoDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
    psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

    // Load shaders
    ComPtr<ID3DBlob> vertexShader;
    ComPtr<ID3DBlob> pixelShader;

    // Different shaders for different pipeline types
    std::wstring vsFile, psFile;

    switch (type)
    {
    case PipelineType::IMAGE_2D:
        vsFile = L"Shaders/Image2D_VS.cso";
        psFile = L"Shaders/Image2D_PS.cso";
        break;
    case PipelineType::SCENE_3D:
        vsFile = L"Shaders/Scene3D_VS.cso";
        psFile = L"Shaders/Scene3D_PS.cso";
        break;
    case PipelineType::PARTICLE_2D:
        vsFile = L"Shaders/Particle2D_VS.cso";
        psFile = L"Shaders/Particle2D_PS.cso";
        break;
    case PipelineType::PARTICLE_3D:
        vsFile = L"Shaders/Particle3D_VS.cso";
        psFile = L"Shaders/Particle3D_PS.cso";
        break;
    case PipelineType::LINE_DRAWING:
        vsFile = L"Shaders/Line_VS.cso";
        psFile = L"Shaders/Line_PS.cso";
        psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
        break;
    case PipelineType::RANDOM_GEOMETRY:
        vsFile = L"Shaders/RandomGeometry_VS.cso";
        psFile = L"Shaders/RandomGeometry_PS.cso";
        break;
    default:
        throw std::runtime_error("Unknown pipeline type.");
    }

    // Load compiled shader files
    D3DReadFileToBlob(vsFile.c_str(), &vertexShader);
    D3DReadFileToBlob(psFile.c_str(), &pixelShader);

    psoDesc.VS = { vertexShader->GetBufferPointer(), vertexShader->GetBufferSize() };
    psoDesc.PS = { pixelShader->GetBufferPointer(), pixelShader->GetBufferSize() };

    // Create pipeline state object
    ComPtr<ID3D12PipelineState> pipelineState;
    HRESULT hr = m_device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pipelineState));
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to create graphics pipeline state.");
    }

    return pipelineState;
}