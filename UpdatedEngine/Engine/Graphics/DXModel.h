#pragma once
#include <unordered_map>
#include <vector>

namespace ENGINE 
{
	struct create_model_info {


		//void* p

	};

	struct SubModel
	{
		uint indices{};
		uint indices_offset{};
		uint base_vertex{};
	};

	

	class ENGINE_API Model
	{
	public:
		Model();
		~Model();

		//TODO : Sub model
		std::vector<SubModel> m_sub_nodes;

		D3D12_VERTEX_BUFFER_VIEW m_view_model{};
		D3D12_INDEX_BUFFER_VIEW m_index_view_model{};
		ID3D12Resource* m_vbo{};
		ID3D12Resource* m_ibo{};
		uint indices{};

		//test
		void create_buffer(std::vector<Vertex>& v, std::vector<uint>& i);
		void render_test(ID3D12GraphicsCommandList* cmdlist);

		static std::vector<D3D12_INPUT_ELEMENT_DESC>& get_input_layout();
	};

	class ENGINE_API ModelCache
	{
	public:
		ModelCache() = default;
		static Model* load_model(const char* filename);
		static std::unordered_map<std::string, std::shared_ptr<Model>> m_cache;
		inline static std::vector<std::shared_ptr<Model>> m_model_list;
	};


}


