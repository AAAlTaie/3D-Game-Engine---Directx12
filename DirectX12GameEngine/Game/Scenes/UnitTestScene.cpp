#include "pch.h"
#include "UnitTestScene.h"
#include "Graphics/DX12Sampler.h"
#include "Graphics/DX12Context.h"
namespace ENGINE 
{
	UnitTestScene::UnitTestScene(Renderer* renderer) : DX12Scene(renderer)
	{

	}

	void UnitTestScene::initialize_frame()
	{
		//1.Root signature
		CD3DX12_DESCRIPTOR_RANGE range[1] = {};
		range[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0); //this is for b0 for constant buffer
		CD3DX12_ROOT_PARAMETER root_params[1]{};
		root_params->InitAsDescriptorTable(_countof(range), range, D3D12_SHADER_VISIBILITY_ALL);
		
		D3D12_STATIC_SAMPLER_DESC samplerDesc{};
		samplerDesc = DX12Sampler::get_sampler_desc(SamplerType::Liner);

		D3D12_ROOT_SIGNATURE_DESC root_desc{};
		root_desc.NumParameters = _countof(root_params);
		root_desc.pParameters = root_params;
		root_desc.NumStaticSamplers = 1;
		root_desc.pStaticSamplers = &samplerDesc;
		root_desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

		ID3DBlob* serializedRootSignature{};
		ID3DBlob* errorBlob{};

		HR(D3D12SerializeRootSignature(
			&root_desc,
			D3D_ROOT_SIGNATURE_VERSION_1,
			&serializedRootSignature,
			&errorBlob));

		HR(m_device->CreateRootSignature(
			0,
			serializedRootSignature->GetBufferPointer(),
			serializedRootSignature->GetBufferSize(),
			IID_PPV_ARGS(&m_rootSignature)));
		safe_release(serializedRootSignature);
		safe_release(errorBlob);

		//DX12Context::compile_from_file("VertexShader.hlsl", "vs_main", "vs_6_0", &m_vertexShaderBlob);
		//DX12Context::compile_from_file("PixelShader.hlsl", "ps_main", "ps_6_0", &m_pixelShaderBlob);

		//2.piplineState
		D3D12_GRAPHICS_PIPELINE_STATE_DESC pso_desc{};
		/*pso_desc.InputLayout = { m_inputLayout.data(), static_cast<UINT>(m_inputLayout.size()) };
		pso_desc.pRootSignature = m_rootSignature;
		pso_desc.VS = { m_vertexShaderBlob->GetBufferPointer(), m_vertexShaderBlob->GetBufferSize() };
		pso_desc.PS = { m_pixelShaderBlob->GetBufferPointer(), m_pixelShaderBlob->GetBufferSize() };
		pso_desc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		pso_desc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		pso_desc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
		pso_desc.SampleMask = UINT_MAX;
		pso_desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		pso_desc.NumRenderTargets = 1;
		pso_desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		pso_desc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
		pso_desc.SampleDesc.Count = 1;*/

		//HR(m_device->CreateGraphicsPipelineState(&pso_desc, IID_PPV_ARGS(&m_pipelineState)));


		//3.VertexBuffer
		
		//4.IndexBuffer
	}


	void UnitTestScene::render_frame()
	{

	}

	void UnitTestScene::update_frame(float dt)
	{

	}
}