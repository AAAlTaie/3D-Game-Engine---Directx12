#pragma once
#include "Renderer.h"

namespace ENGINE
{
    class ENGINE_API Scene
    {
    public:
        Scene() = default;
        virtual ~Scene() = default;
        virtual void initialize_frame() = 0;
        virtual void update_frame(float dt) = 0;
        virtual void render_frame() = 0;
        virtual void release() {};
    };

    class ENGINE_API DX12Scene : public Scene
    {
    public:
       explicit DX12Scene(Renderer* rend); 
       virtual ~DX12Scene() {};

        virtual void initialize_frame() {};
        virtual void update_frame(float dt) {};
        virtual void render_frame() {};

        Renderer* m_rend{};
        ID3D12Device* m_device{};
        ID3D12GraphicsCommandList* m_command_list{};
    };
}



//#pragma once
//
//
//
//
//namespace ENGINE
//{
//
//	class ENGINE_API Scene
//	{
//	public:
//		Scene() = default;
//		virtual void initialize_frame() = 0;
//		virtual void update_frame(float dt) = 0;
//		virtual void render_frame() = 0;
//		virtual void release() {};
//	};
//
//	class ENGINE_API Renderer;
//	class ENGINE_API DX12Scene : Scene 
//	{
//	public:
//		DX12Scene(Renderer* rend);
//		Renderer* m_rend;
//		ID3D12Device* m_device{};
//		ID3D12GraphicsCommandList* m_command_list{};
//	};
//}