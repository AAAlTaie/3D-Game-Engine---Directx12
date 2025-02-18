//#pragma once
//#include <memory>
//#include <map>
//#include <string>
//#include <iostream>
//#include <d3d12.h>        // For DirectX 12 API
//#include <wrl.h> 
//#include <3DMath.h>
//
//
///// <REsourses For All Rendering Scens by Default>
///// 1. Create Device, CommandQueue, CommandAllocator, CommandList, SwapChain
///// 2. Setup Render Target Views
///// 3. Setup Depth Stencil Views
///// 4. 
///// </summary>
//
//
//namespace ENGINE 
//{
//    using Microsoft::WRL::ComPtr;
//
//    class Scene;
//
//    //SceneData to hold scene-specific data
//    struct SceneData {
//        //PipelineManager(ID3D12Device* device) : device(device) {}
//        ENGINE::float4x4 viewMatrix;
//        ENGINE::float4x4 projMatrix;
//    };
//
//    // Abstract Scene class
//    class Scene {
//    public:
//        virtual ~Scene() = default;
//        virtual void initialize(const SceneData& data) = 0;
//        virtual void update(float deltaTime) = 0;
//        virtual void render(ID3D12GraphicsCommandList* commandList) const = 0;
//        virtual void SafeRelease() = 0;
//
//        // Clone method for deep copy of scenes
//        virtual std::unique_ptr<Scene> clone() const = 0;
//
//        // Accessors for view and projection matrices
//        const ENGINE::float4x4& getViewMatrix() const { return m_viewMatrix; }
//        const ENGINE::float4x4& getProjMatrix() const { return m_projMatrix; }
//
//    protected:
//        ENGINE::float4x4 m_viewMatrix;
//        ENGINE::float4x4 m_projMatrix;
//    };
//
//    // Template Scene class
//    template<typename T>
//    class SceneTemplate : public Scene {
//    public:
//        SceneTemplate() = default;
//        virtual ~SceneTemplate() = default;
//
//        void initialize(const SceneData& data) override {
//            m_viewMatrix = data.viewMatrix;
//            m_projMatrix = data.projMatrix;
//            static_cast<T*>(this)->onInitialize();
//        }
//        void update(float deltaTime) override {
//            static_cast<T*>(this)->onUpdate(deltaTime);
//        }
//        void render(ID3D12GraphicsCommandList* commandList) const override {
//            static_cast<const T*>(this)->onRender(commandList);
//        }
//        void SafeRelease() override {
//            static_cast<T*>(this)->onCleanup();
//        }
//        std::unique_ptr<Scene> clone() const override {
//            return std::make_unique<SceneTemplate<T>>(*this);
//        }
//    };
//
//    class SceneManager {
//    public:
//        template<typename T>
//        void registerScene(const std::string& sceneName) {
//            m_scenes[sceneName] = std::make_unique<SceneTemplate<T>>();
//        }
//        void setScene(const std::string& sceneName, const SceneData& data) {
//            if (m_currentScene) { m_currentScene->SafeRelease(); }
//            auto it = m_scenes.find(sceneName);
//            if (it != m_scenes.end()) {
//                m_currentScene = it->second->clone();
//                m_currentScene->initialize(data);
//                m_currentSceneName = sceneName;
//            }
//            else {
//                std::cerr << "Scene not found: " << sceneName << std::endl;
//            }
//        }
//        void update(float deltaTime) {
//            if (m_currentScene) {
//                m_currentScene->update(deltaTime);
//            }
//        }
//        void render(ID3D12GraphicsCommandList* commandList) const {
//            if (m_currentScene) {
//                m_currentScene->render(commandList);
//            }
//        }
//        void switchScene(const std::string& newSceneName, const SceneData& data) {
//            if (m_currentScene) {
//                m_currentScene->SafeRelease();
//            }
//            auto it = m_scenes.find(newSceneName);
//            if (it != m_scenes.end()) {
//                m_currentScene = it->second->clone();
//                m_currentScene->initialize(data);
//                m_currentSceneName = newSceneName;
//            }
//            else {
//                std::cerr << "Scene not found: " << newSceneName << std::endl;
//            }
//        }
//
//    private:
//        std::map<std::string, std::unique_ptr<Scene>> m_scenes;
//        std::unique_ptr<Scene> m_currentScene;
//        std::string m_currentSceneName;
//    };
//}