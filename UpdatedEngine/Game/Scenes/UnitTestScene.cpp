#include "pch.h"
#include "UnitTestScene.h"
#include "Graphics/DX12Sampler.h"
#include "Graphics/DX12Context.h"
#include "Core/CoreInput.h"
#include "Components/Camera.h"
#include "Graphics/ConstantBuffer.h"
#include "Graphics/DXModel.h"
#include <filesystem>

#define PI 3.1415926536
using namespace DirectX;

namespace ENGINE 
{
	UnitTestScene::UnitTestScene(Renderer* renderer) : DX12Scene(renderer)
	{
		
	}
	void UnitTestScene::initialize_frame()
	{
		CD3DX12_ROOT_PARAMETER root_params[2]{};
		root_params[0].InitAsConstantBufferView(0); // register b0
		root_params[1].InitAsConstantBufferView(1); // register b1

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

		DX12Context::compile_from_file("shaders/VertexShader.hlsl", "vs_main", "vs_5_1", &m_vertexShaderBlob);
		DX12Context::compile_from_file("shaders/PixelShader.hlsl", "ps_main", "ps_5_1", &m_pixelShaderBlob);

		D3D12_INPUT_ELEMENT_DESC m_inputLayout[] = {

			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,0,0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,12,  D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}

		};

		
		CD3DX12_RASTERIZER_DESC rasterizerDesc(D3D12_DEFAULT);
		rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE; 
		rasterizerDesc.FrontCounterClockwise = FALSE;

		// Pipeline State
		D3D12_GRAPHICS_PIPELINE_STATE_DESC pso_desc{};
		pso_desc.pRootSignature = m_rootSignature;
		pso_desc.VS = { m_vertexShaderBlob->GetBufferPointer(), m_vertexShaderBlob->GetBufferSize() };
		pso_desc.PS = { m_pixelShaderBlob->GetBufferPointer(), m_pixelShaderBlob->GetBufferSize() };
		pso_desc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		pso_desc.SampleMask = UINT_MAX;
		pso_desc.RasterizerState = rasterizerDesc; 
		pso_desc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
		pso_desc.InputLayout = { m_inputLayout, _countof(m_inputLayout) };
		//pso_desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE; 
		pso_desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
		pso_desc.NumRenderTargets = 1;
		pso_desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		pso_desc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
		pso_desc.SampleDesc.Count = 1;
		pso_desc.NodeMask = 0;

		// Create the pipeline state
		HR(m_device->CreateGraphicsPipelineState(&pso_desc, IID_PPV_ARGS(&m_pipeline)));
		safe_release(m_vertexShaderBlob);
		safe_release(m_pixelShaderBlob);


		//Model Pipeline
		DX12Context::compile_from_file("shaders/model_vs.hlsl", "vs_main", "vs_5_1", &m_model_shader[0]);
		DX12Context::compile_from_file("shaders/model_ps.hlsl", "ps_main", "ps_5_1", &m_model_shader[1]);

		D3D12_INPUT_ELEMENT_DESC* ibo = Model::get_input_layout().data();
		D3D12_INPUT_ELEMENT_DESC aa = ibo[1];

		pso_desc.InputLayout = { Model::get_input_layout().data(), (uint)Model::get_input_layout().size()};
		pso_desc.VS = { m_model_shader[0]->GetBufferPointer(), m_model_shader[0]->GetBufferSize() };
		pso_desc.PS = { m_model_shader[1]->GetBufferPointer(), m_model_shader[1]->GetBufferSize() };
		pso_desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

		HR(m_device->CreateGraphicsPipelineState(&pso_desc, IID_PPV_ARGS(&m_model_pipeline)));

		//3.VertexBuffer
		LineVertex vertices[] =
		{
			// Front face (z = 0.5f)
			{ { -0.5f, -0.5f, 0.5f }, { 1.0f, 0.0f, 0.0f, 1.0f } }, // Bottom-left
			{ {  0.5f, -0.5f, 0.5f }, { 0.0f, 1.0f, 0.0f, 1.0f } }, // Bottom-right
			{ {  0.5f,  0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f, 1.0f } }, // Top-right
			{ { -0.5f,  0.5f, 0.5f }, { 1.0f, 1.0f, 0.0f, 1.0f } }, // Top-left

			// Back face (z = -0.5f)
			{ { -0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f } }, // Bottom-left
			{ {  0.5f, -0.5f, -0.5f }, { 0.0f, 1.0f, 1.0f, 1.0f } }, // Bottom-right
			{ {  0.5f,  0.5f, -0.5f }, { 1.0f, 1.0f, 1.0f, 1.0f } }, // Top-right
			{ { -0.5f,  0.5f, -0.5f }, { 0.5f, 0.5f, 0.5f, 1.0f } }, // Top-left

			// Left face (x = -0.5f)
			{ { -0.5f, -0.5f, -0.5f }, { 1.0f, 0.5f, 0.0f, 1.0f } }, // Bottom-left
			{ { -0.5f, -0.5f,  0.5f }, { 0.0f, 0.5f, 1.0f, 1.0f } }, // Bottom-right
			{ { -0.5f,  0.5f,  0.5f }, { 0.5f, 1.0f, 0.5f, 1.0f } }, // Top-right
			{ { -0.5f,  0.5f, -0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f } }, // Top-left

			// Right face (x = 0.5f)
			{ {  0.5f, -0.5f, -0.5f }, { 1.0f, 1.0f, 0.5f, 1.0f } }, // Bottom-left
			{ {  0.5f, -0.5f,  0.5f }, { 0.0f, 1.0f, 0.0f, 1.0f } }, // Bottom-right
			{ {  0.5f,  0.5f,  0.5f }, { 0.5f, 1.0f, 1.0f, 1.0f } }, // Top-right
			{ {  0.5f,  0.5f, -0.5f }, { 1.0f, 0.5f, 1.0f, 1.0f } }, // Top-left

			// Top face (y = 0.5f)
			{ { -0.5f,  0.5f, -0.5f }, { 1.0f, 1.0f, 1.0f, 1.0f } }, // Bottom-left
			{ {  0.5f,  0.5f, -0.5f }, { 0.5f, 0.5f, 1.0f, 1.0f } }, // Bottom-right
			{ {  0.5f,  0.5f,  0.5f }, { 0.0f, 1.0f, 0.0f, 1.0f } }, // Top-right
			{ { -0.5f,  0.5f,  0.5f }, { 1.0f, 0.5f, 0.0f, 1.0f } }, // Top-left

			// Bottom face (y = -0.5f)
			{ { -0.5f, -0.5f, -0.5f }, { 1.0f, 1.0f, 0.0f, 1.0f } }, // Bottom-left
			{ {  0.5f, -0.5f, -0.5f }, { 0.0f, 0.5f, 1.0f, 1.0f } }, // Bottom-right
			{ {  0.5f, -0.5f,  0.5f }, { 0.5f, 1.0f, 0.5f, 1.0f } }, // Top-right
			{ { -0.5f, -0.5f,  0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f } }, // Top-left
		};


		DX12Context::create_buffers_system_memory(vertices, sizeof(vertices), &m_vertex_Quad_buffer);
		m_vertex_QUAD_view.BufferLocation = m_vertex_Quad_buffer->GetGPUVirtualAddress();
		m_vertex_QUAD_view.SizeInBytes = sizeof(vertices);
		m_vertex_QUAD_view.StrideInBytes = sizeof(LineVertex);

		//4.IndexBuffer
		// Index buffer for a cube
		uint indices[] =
		{
			// Front face
			0, 1, 2, // First triangle
			0, 2, 3, // Second triangle

			// Back face
			4, 5, 6, // First triangle
			4, 6, 7, // Second triangle

			// Left face
			8, 9, 10, // First triangle
			8, 10, 11, // Second triangle

			// Right face
			12, 13, 14, // First triangle
			12, 14, 15, // Second triangle

			// Top face
			16, 17, 18, // First triangle
			16, 18, 19, // Second triangle

			// Bottom face
			20, 21, 22, // First triangle
			20, 22, 23  // Second triangle
		};


		DX12Context::create_buffers_system_memory(indices, sizeof(indices), &m_index_QUAD);
		m_index_QUAD_view.BufferLocation = m_index_QUAD->GetGPUVirtualAddress();
		m_index_QUAD_view.Format = DXGI_FORMAT_R32_UINT;
		m_index_QUAD_view.SizeInBytes = sizeof(indices);


		auto viewport = m_rend->get_viewport();
		float ratio = viewport.Width / viewport.Height;
		m_camera = std::make_shared<Camera>(45.0f, ratio, 0.01f, 1000.0f);
		m_camera->set_position({ 0.0f, 2.0f, -9.0f });

		// VP - View and proj only!!
		float4x4 matrices[2]{}; //= { m_camera->get_projection(), m_camera->get_view(), m_model };
		matrices[0] = m_camera->get_projection();
		matrices[1] = m_camera->get_view();

		ConstantBufferInfo info{};
		info.p_data = matrices;
		info.cb_size = sizeof(matrices);
		info.buffer_count = 1;

		info = {};
		float4x4 model{};
		info.p_data = &model;
		info.cb_size = sizeof(float4x4);
		info.buffer_count = 4;


		//obj_model = ModelCache::load_model("data/kratos.fbx");
		//obj_model = ModelCache::load_model("data/tree.obj");

		//if (!std::filesystem::exists("data/cube.fbx")) {
		//	log_error("not founded");
		//}
		obj_model = ModelCache::load_model("data/building_04.fbx");
		//obj_model = ModelCache::load_model("data/man.fbx");

		create_grid();
		create_3D_sphere(5, 25, 25);

		m_cbo = std::make_shared<ConstantBuffer>(&info);
		m_constant_buffers = std::make_shared<ConstantBuffer>(&info);
	}

	void UnitTestScene::render_frame()
	{
		m_command_list->SetPipelineState(m_pipeline);
		m_command_list->SetGraphicsRootSignature(m_rootSignature);

		//cbo for camera matrix
		m_command_list->SetGraphicsRootConstantBufferView( //b0
			0, m_cbo->get_gpu_address());

		//constanbuffer for transform
		m_command_list->SetGraphicsRootConstantBufferView(1, //b1
			m_constant_buffers->get_gpu_address(1)); // FOE Moedel
		
		float4x4 transform_default{};
		float4x4 transform0{};
		m_constant_buffers->map(0, &transform0, sizeof(float4x4));

		//Drawing Line 
		m_command_list->IASetVertexBuffers(0, 1, &m_vertex_buffer_view);
		m_command_list->IASetIndexBuffer(&m_index_buffer_view);
		m_command_list->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
		m_command_list->SetGraphicsRootConstantBufferView(
			1, m_constant_buffers->get_gpu_address(0)
		);
		m_command_list->DrawIndexedInstanced(m_index_count, 1, 0, 0, 0);
		
		//Drawing Quad 
		m_command_list->IASetVertexBuffers(0, 1, &obj_model->m_view_model);
		m_command_list->IASetIndexBuffer(&obj_model->m_index_view_model);
		m_command_list->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		for (int i = 0; i < 1; ++i) {
			//transform0 = float4x4::create_translate(5.f * i, 0.f, 0.f);
			m_constant_buffers->map(i, &transform0, sizeof(float4x4));
			m_command_list->SetGraphicsRootConstantBufferView(
				1, m_constant_buffers->get_gpu_address(i)
			);
			m_command_list->DrawIndexedInstanced(obj_model->indices, 1, 0, 0, 0);
		}

		m_command_list->SetPipelineState(m_model_pipeline);
		m_command_list->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		obj_model->render_test(m_command_list);

		//DRAWING Sphere
		m_command_list->IASetVertexBuffers(0, 1, &m_vertex_sphere_view);
		m_command_list->IASetIndexBuffer(&m_index_sphere_view);
		m_command_list->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);

		transform0 = float4x4::create_translate(0.f, 10.f, 0.f);
		m_constant_buffers->map(3, &transform0, sizeof(float4x4));
		m_command_list->SetGraphicsRootConstantBufferView(
			//b1 
			1, m_constant_buffers->get_gpu_address(3)
		);
		///m_command_list->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_command_list->DrawIndexedInstanced(m_index_sphere, 1, 0, 0, 0);


		
	}

	void UnitTestScene::update_frame(float dt)
	{
		m_camera->update_camera(dt);

		float4x4 matrices[2]{};
		matrices[0] = m_camera->get_projection();
		matrices[1] = m_camera->get_view();
		m_cbo->map(0, matrices, sizeof(matrices));

	}

	void UnitTestScene::release()
	{
		safe_release(m_rootSignature);
		safe_release(m_pipeline);
		safe_release(m_index_line_buffer);
		safe_release(m_vertex_line_buffer);
		//safe_delete(m_camera);
	}

	void UnitTestScene::create_grid()
	{
		const int gridSize = 80;
		const int slice = 2;

		std::vector<LineVertex> vertices;
		std::vector<uint> indices;

		for (int i = -gridSize; i <= gridSize; i += slice) 
		{

			vertices.push_back({ (float)-gridSize, 0.0f, (float)i });
			vertices.push_back({ (float)gridSize, 0.0f, (float)i });

			vertices.push_back({ (float)i, 0.0f, (float)-gridSize });
			vertices.push_back({ (float)i, 0.0f, (float)gridSize });

		}

		for (int i = 0; i < vertices.size(); i++) 
		{
			indices.push_back(i);
		}

		const UINT vertexBufferSize = static_cast<uint>(vertices.size() * sizeof(LineVertex));
		const UINT indexBufferSize = static_cast<uint>(indices.size() * sizeof(UINT));

		DX12Context::create_buffers_system_memory(vertices.data(), vertexBufferSize, &m_vertex_line_buffer);
		m_vertex_buffer_view.BufferLocation = m_vertex_line_buffer->GetGPUVirtualAddress();
		m_vertex_buffer_view.SizeInBytes = vertexBufferSize;
		m_vertex_buffer_view.StrideInBytes = sizeof(LineVertex);

		DX12Context::create_buffers_system_memory(indices.data(), indexBufferSize, &m_index_line_buffer);
		m_index_buffer_view.BufferLocation = m_index_line_buffer->GetGPUVirtualAddress();
		m_index_buffer_view.Format = DXGI_FORMAT_R32_UINT;
		m_index_buffer_view.SizeInBytes = indexBufferSize;

		m_index_count = (uint)indices.size();
	}

	void UnitTestScene::create_3D_sphere(float radius, int sectorCount, int stackCount)
	{

		std::vector<LineVertex> vertices;
		std::vector<uint> indices;

		float x, y, z, xy;                                  // vertex position
		float nx{}, ny{}, nz{}, lengthInv = 1.0f / radius;  // vertex normal
		float s{}, t{};                                     // vertex texture coordinates

		float sectorStep = 2 * (float)PI / sectorCount;
		float stackStep = (float)PI / stackCount;
		float sectorAngle, stackAngle;

		for (int i = 0; i <= stackCount; ++i)
		{
			stackAngle = (float)PI / 2 - i * stackStep; // from pi/2 to -pi/2
			xy = radius * cosf(stackAngle);      // r * cos(u)
			z = radius * sinf(stackAngle);       // r * sin(u)

			// add (sectorCount+1) vertices per stack
			// the first and last vertices have same position and normal, but different tex coords
			for (int j = 0; j <= sectorCount; ++j)
			{
				sectorAngle = j * sectorStep;               // from 0 to 2pi

				// vertex position
				x = xy * cosf(sectorAngle);                 // r * cos(u) * cos(v)
				y = xy * sinf(sectorAngle);                 // r * cos(u) * sin(v)
				vertices.push_back({ x, y, z });

				// indices
				if (i != 0 && j != 0) {
					int cur = i * (sectorCount + 1) + j;        // current vertex
					int prev = (i - 1) * (sectorCount + 1) + j; // previous vertex

					if (i != 1) {
						indices.push_back(prev);
						indices.push_back(prev - 1);
						indices.push_back(cur - 1);
					}
					if (i != stackCount) {
						indices.push_back(cur - 1);
						indices.push_back(cur);
						indices.push_back(prev);
					}
				}
			}
		}

		const UINT vertexBufferSize = static_cast<uint>(vertices.size() * sizeof(LineVertex));
		const UINT indexBufferSize = static_cast<uint>(indices.size() * sizeof(UINT));

		DX12Context::create_buffers_system_memory(vertices.data(), vertexBufferSize, &m_vertex_sphere_buffer);
		m_vertex_sphere_view.BufferLocation = m_vertex_sphere_buffer->GetGPUVirtualAddress();
		m_vertex_sphere_view.SizeInBytes = vertexBufferSize;
		m_vertex_sphere_view.StrideInBytes = sizeof(LineVertex);

		DX12Context::create_buffers_system_memory(indices.data(), indexBufferSize, &m_index_sphere_buffer);
		m_index_sphere_view.BufferLocation = m_index_sphere_buffer->GetGPUVirtualAddress();
		m_index_sphere_view.Format = DXGI_FORMAT_R32_UINT;
		m_index_sphere_view.SizeInBytes = indexBufferSize;
		m_index_sphere = static_cast<uint>(indices.size());
	}

}



//
//void UnitTestScene::initialize_frame()
//{
//	// Root Signature
//	CD3DX12_ROOT_PARAMETER root_params[2]{};
//	root_params[0].InitAsConstantBufferView(0); // b0: Camera
//	root_params[1].InitAsConstantBufferView(1); // b1: Transform
//
//	D3D12_STATIC_SAMPLER_DESC samplerDesc = DX12Sampler::get_sampler_desc(SamplerType::Liner);
//
//	D3D12_ROOT_SIGNATURE_DESC root_desc{};
//	root_desc.NumParameters = _countof(root_params);
//	root_desc.pParameters = root_params;
//	root_desc.NumStaticSamplers = 1;
//	root_desc.pStaticSamplers = &samplerDesc;
//	root_desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
//
//	ID3DBlob* serializedRootSignature = nullptr;
//	ID3DBlob* errorBlob = nullptr;
//
//	HR(D3D12SerializeRootSignature(&root_desc, D3D_ROOT_SIGNATURE_VERSION_1, &serializedRootSignature, &errorBlob));
//	HR(m_device->CreateRootSignature(0, serializedRootSignature->GetBufferPointer(), serializedRootSignature->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature)));
//
//	safe_release(serializedRootSignature);
//	safe_release(errorBlob);
//
//	// Compile Shaders
//	DX12Context::compile_from_file("shaders/VertexShader.hlsl", "vs_main", "vs_5_1", &m_vertexShaderBlob);
//	DX12Context::compile_from_file("shaders/PixelShader.hlsl", "ps_main", "ps_5_1", &m_pixelShaderBlob);
//
//	// Input Layout for Lines
//	D3D12_INPUT_ELEMENT_DESC m_inputLayout[] = {
//		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
//		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
//	};
//
//	// Rasterizer State
//	CD3DX12_RASTERIZER_DESC rasterizerDesc(D3D12_DEFAULT);
//	rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE; // Disable culling for lines
//
//	// Pipeline State for Lines
//	D3D12_GRAPHICS_PIPELINE_STATE_DESC pso_desc{};
//	pso_desc.pRootSignature = m_rootSignature;
//	pso_desc.VS = { m_vertexShaderBlob->GetBufferPointer(), m_vertexShaderBlob->GetBufferSize() };
//	pso_desc.PS = { m_pixelShaderBlob->GetBufferPointer(), m_pixelShaderBlob->GetBufferSize() };
//	pso_desc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
//	pso_desc.SampleMask = UINT_MAX;
//	pso_desc.RasterizerState = rasterizerDesc;
//	pso_desc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
//	pso_desc.InputLayout = { m_inputLayout, _countof(m_inputLayout) };
//	pso_desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE; // Set to LINE for line rendering
//	pso_desc.NumRenderTargets = 1;
//	pso_desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
//	pso_desc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
//	pso_desc.SampleDesc.Count = 1;
//
//	HR(m_device->CreateGraphicsPipelineState(&pso_desc, IID_PPV_ARGS(&m_pipeline)));
//	safe_release(m_vertexShaderBlob);
//	safe_release(m_pixelShaderBlob);
//
//	// Initialize Vertex and Index Buffers for Lines
//	LineVertex lineVertices[] = {
//		{ { -1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } }, // Start point (red)
//		{ {  1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } }  // End point (green)
//	};
//
//	uint lineIndices[] = { 0, 1 }; // Indices for a single line
//
//	DX12Context::create_buffers_system_memory(lineVertices, sizeof(lineVertices), &m_vertex_buffer);
//	m_vertex_buffer_view.BufferLocation = m_vertex_buffer->GetGPUVirtualAddress();
//	m_vertex_buffer_view.SizeInBytes = sizeof(lineVertices);
//	m_vertex_buffer_view.StrideInBytes = sizeof(LineVertex);
//
//	DX12Context::create_buffers_system_memory(lineIndices, sizeof(lineIndices), &m_index_buffer);
//	m_index_buffer_view.BufferLocation = m_index_buffer->GetGPUVirtualAddress();
//	m_index_buffer_view.Format = DXGI_FORMAT_R32_UINT;
//	m_index_buffer_view.SizeInBytes = sizeof(lineIndices);
//
//	// Initialize Camera and Constant Buffers
//	auto viewport = m_rend->get_viewport();
//	float ratio = viewport.Width / viewport.Height;
//	m_camera = std::make_shared<Camera>(45.0f, ratio, 0.01f, 1000.0f);
//	m_camera->set_position({ 0.0f, 2.0f, -9.0f });
//
//	float4x4 matrices[2] = { m_camera->get_projection(), m_camera->get_view() };
//	ConstantBufferInfo info{ matrices, sizeof(matrices), 1 };
//	m_cbo = std::make_shared<ConstantBuffer>(&info);
//
//	float4x4 model = float4x4::identity_static();
//	ConstantBufferInfo transformInfo{ &model, sizeof(float4x4), 4 };
//	m_constant_buffers = std::make_shared<ConstantBuffer>(&transformInfo);
//}
//
//void UnitTestScene::render_frame()
//{
//	// Set root signature and camera constant buffer (b0)
//	m_command_list->SetGraphicsRootSignature(m_rootSignature);
//	m_command_list->SetGraphicsRootConstantBufferView(0, m_cbo->get_gpu_address());
//
//	// Map and set the transform constant buffer (b1)
//	float4x4 transform0 = float4x4::identity_static(); // Ensure identity transform if no specific transform is needed
//	m_constant_buffers->map(0, &transform0, sizeof(float4x4));
//
//	// Draw Line
//	m_command_list->SetGraphicsRootConstantBufferView(1, m_constant_buffers->get_gpu_address(0));
//	draw_geometry(m_vertex_buffer_view, m_index_buffer_view, D3D_PRIMITIVE_TOPOLOGY_LINELIST, m_index_count, 0);
//
//	// Draw Quad (4 instances with different transforms)
//	for (int i = 0; i < 4; ++i) {
//		transform0 = float4x4::create_translate(5.f * i, 0.f, 0.f);
//		m_constant_buffers->map(i, &transform0, sizeof(float4x4));
//		m_command_list->SetGraphicsRootConstantBufferView(1, m_constant_buffers->get_gpu_address(i));
//		draw_geometry(obj_model->m_view_model, obj_model->m_index_view_model, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, obj_model->indices, 0);
//	}
//
//	// Switch to the model pipeline and render the model
//	m_command_list->SetPipelineState(m_model_pipeline);
//	obj_model->render_test(m_command_list);
//}
//
//void UnitTestScene::draw_geometry(
//	const D3D12_VERTEX_BUFFER_VIEW& vertex_buffer_view,
//	const D3D12_INDEX_BUFFER_VIEW& index_buffer_view,
//	D3D12_PRIMITIVE_TOPOLOGY topology,
//	UINT index_count,
//	UINT start_index)
//{
//	// Validate vertex and index buffers
//	if (vertex_buffer_view.BufferLocation == 0 || index_buffer_view.BufferLocation == 0) {
//		OutputDebugStringA("Error: Vertex or index buffer is not initialized.\n");
//		return;
//	}
//
//	// Set vertex and index buffers
//	m_command_list->IASetVertexBuffers(0, 1, &vertex_buffer_view);
//	m_command_list->IASetIndexBuffer(&index_buffer_view);
//	m_command_list->IASetPrimitiveTopology(topology);
//
//	// Draw call
//	m_command_list->DrawIndexedInstanced(index_count, 1, start_index, 0, 0);
//}