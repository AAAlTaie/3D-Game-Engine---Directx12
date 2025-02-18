#include "pch.h"
#include "DXModel.h"

//#ifdef _DEBUG
//#undef new
//#include "assimp/Importer.hpp"
//#define new new(NORMALBLOCK, FILE, __LINE)
//#endif

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "graphics/DX12Context.h"

namespace ENGINE 
{
	std::unordered_map<std::string, std::shared_ptr<Model>> ModelCache::m_cache{};
	Model::Model()
	{

	}

	Model::~Model()
	{
	}

	void Model::create_buffer(std::vector<Vertex>& v, std::vector<uint>& i)
	{
		indices = (uint)i.size();
		DX12Context::create_buffers_system_memory(v.data(), v.size() * sizeof(Vertex),
			&m_vbo);

		

		m_view_model.BufferLocation = m_vbo->GetGPUVirtualAddress();
		m_view_model.SizeInBytes = sizeof(Vertex) * (uint)v.size();
		m_view_model.StrideInBytes = sizeof(Vertex);

		DX12Context::create_buffers_system_memory(i.data(), i.size() * sizeof(uint),
			&m_ibo);

		m_index_view_model.BufferLocation = m_ibo->GetGPUVirtualAddress();
		m_index_view_model.Format = DXGI_FORMAT_R32_UINT;
		m_index_view_model.SizeInBytes = i.size() * sizeof(uint);
		
	}

	void Model::render_test(ID3D12GraphicsCommandList* cmdlist)
	{
		uint stride = sizeof(Vertex);
		uint offset = 0;

		cmdlist->IASetVertexBuffers(0, 1, &m_view_model);

		cmdlist->IASetIndexBuffer(&m_index_view_model);
		cmdlist->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		uint cur_pos = 0;
		for (size_t i = 0; i < m_sub_nodes.size(); ++i) {
			SubModel& node = m_sub_nodes[i];
			
			cmdlist->DrawIndexedInstanced(
				node.indices,		//indexcount per instance
				1,		//instance count
				cur_pos,		//start index location
				0,			//base vertex location
				0		//start instance location
				
			);
			cur_pos += node.indices + node.indices_offset;
		}
	}

	inline std::vector<D3D12_INPUT_ELEMENT_DESC>& Model::get_input_layout()
	{
		static std::vector<D3D12_INPUT_ELEMENT_DESC> layout = {

			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,0,0,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,offsetof(Vertex, Vertex::normal),
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
			{ "ST", 0, DXGI_FORMAT_R32G32_FLOAT, 0,  offsetof(Vertex, Vertex::st),
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
			{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,offsetof(Vertex, Vertex::binormal),
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,offsetof(Vertex, Vertex::tangent),
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
		};
		return layout;
	}

	//std::unordered_map<std::string, std::shared_ptr<Model>> ModelCache::m_cache;
	void import_assimp_model(const char* filename, Model* pp_model);

	Model* ModelCache::load_model(const char* filename)
	{
		std::shared_ptr<Model> model = std::make_shared<Model>();
		
		import_assimp_model(filename, model.get());

		//// Add model to cache
		//m_cache.insert(std::pair<std::string, std::shared_ptr<Model>>(filename, model));
		m_model_list.emplace_back(model);

		return model.get();

		//return nullptr;
	}

	void import_assimp_model(const char* filename, Model* pp_model)
	{
		Assimp::Importer importer;


		const aiScene* scene = importer.ReadFile(
			filename, 
			aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
		
			return;
		}

		uint model_nums = scene->mNumMeshes;

		Model* model = pp_model;
		//resize sub model
		model->m_sub_nodes.resize(model_nums);

		std::vector<Vertex> vertices;
		std::vector<uint> indices;

		//float3 min(FTL_MAX)
		for (uint32_t index = 0; index < model_nums; ++index)
		{
			aiMesh* mesh = scene->mMeshes[index];

			// Process vertices
			for (uint32_t i = 0; i < mesh->mNumVertices; ++i)
			{
				Vertex vertex{};

				memcpy(&vertex.pos, &mesh->mVertices[i], sizeof(float3));
				memcpy(&vertex.normal, &mesh->mNormals[i], sizeof(float3));

				//st
				if (mesh->mTextureCoords[0]) {
					memcpy(&vertex.st, &mesh->mTextureCoords[0][i], sizeof(float) * 2);
				}
				else {
					vertex.st = float2(0.f, 0.f);
				}

				memcpy(&vertex.binormal, &mesh->mBitangents[i], sizeof(float3));
				memcpy(&vertex.tangent, &mesh->mTangents[i], sizeof(float3));

				vertices.emplace_back(vertex);
				
			}

			SubModel* node = model->m_sub_nodes.data();

			node[index].base_vertex += mesh->mNumVertices;
			node[index].indices_offset +=
				(index > 0) ? node[index - 1].indices : 0;
			node[index].indices = mesh->mNumFaces * 3;
			// Process indices

			printf("subnode %d indice %d offset %d\n", index, node[index].indices, node[index].indices_offset);

			for (uint32_t i = 0; i < mesh->mNumFaces; ++i)
			{
				aiFace face = mesh->mFaces[i];
				if (face.mNumIndices == 3)
				{
					//if shared vertex we need to pass it
					indices.emplace_back(face.mIndices[0] + node[index].indices_offset);
					indices.emplace_back(face.mIndices[1] + node[index].indices_offset);
					indices.emplace_back(face.mIndices[2] + node[index].indices_offset);
				}
			}

			// Process material, if needed
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			// Example: Load material properties into p_model->m_models[i].material
		}

		model->create_buffer(vertices, indices);

	}
}