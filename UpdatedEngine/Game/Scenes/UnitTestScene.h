#pragma once
#include "Scene.h"

namespace ENGINE 
{
	class UnitTestScene : public DX12Scene
	{
	public:

		UnitTestScene(Renderer* renderer);
		void initialize_frame() override;
		
		
		
		void render_frame() override;

		void draw_geometry(
			const D3D12_VERTEX_BUFFER_VIEW& vertex_buffer_view,
			const D3D12_INDEX_BUFFER_VIEW& index_buffer_view,
			D3D12_PRIMITIVE_TOPOLOGY topology,
			UINT index_count,
			UINT start_index);

		void update_frame(float dt) override;
		void release() override;
		
		void create_grid();


		ID3D12RootSignature* m_rootSignature{};
		ID3D12PipelineState* m_pipeline{};

		//ID3D12PipelineState* m_line_pipline{};
		ID3D12PipelineState* m_model_pipeline{};

		//LINE Vertex
		ID3D12Resource* m_vertex_line_buffer{};
		D3D12_VERTEX_BUFFER_VIEW m_vertex_buffer_view{};

		//LINE Index
		ID3D12Resource* m_index_line_buffer{};
		D3D12_INDEX_BUFFER_VIEW m_index_buffer_view{};

		//QUAD VERTEX RES
		ID3D12Resource* m_vertex_Quad_buffer{};
		D3D12_VERTEX_BUFFER_VIEW m_vertex_QUAD_view{};

		//QUAD INDEX RES
		ID3D12Resource* m_index_QUAD{};
		D3D12_INDEX_BUFFER_VIEW m_index_QUAD_view{};

		//SPHERE vertex
		ID3D12Resource* m_vertex_sphere_buffer{};
		D3D12_VERTEX_BUFFER_VIEW m_vertex_sphere_view{};

		//SPERE Index
		ID3D12Resource* m_index_sphere_buffer{};
		D3D12_INDEX_BUFFER_VIEW m_index_sphere_view{};

		ID3D10Blob* m_vertexShaderBlob{};
		ID3D10Blob* m_pixelShaderBlob{};

		ID3DBlob* m_model_shader[2]{};

		std::shared_ptr<ConstantBuffer> m_cbo;
		std::shared_ptr<Camera> m_camera;

		std::shared_ptr<ConstantBuffer> m_constant_buffers;


		uint m_index_count = 0;
		uint m_index_sphere = 0;

		void create_3D_sphere(float radius, int sectorCount, int stackCount);

		Model* obj_model;
		Model* grid_model;

	};
}

