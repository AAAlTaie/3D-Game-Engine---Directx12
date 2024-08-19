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
		void update_frame(float dt) override;

		ID3D12RootSignature* m_rootSignature{};
		ID3D12PipelineState* m_pipeline{};

		ID3D12Resource* m_vertex_line_buffer{};
		ID3D12Resource* m_index_line_buffer{};

		ID3D10Blob* m_vertexShaderBlob{};
		ID3D10Blob* m_pixelShaderBlob{};

		


		//void ComplileShaders();



	};
}

