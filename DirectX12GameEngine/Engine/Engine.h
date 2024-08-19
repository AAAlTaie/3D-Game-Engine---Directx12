#pragma once
#include "EngineCore.h"
#include <Windows.h>

#define ENGINE_CLASS_NAME "Reaper's Engine"
#define ENGINE_VERSION "0_0_0_1V"


namespace ENGINE 
{
    class Renderer;
    class Scene;
	class ENGINE_API Engine
	{
    public:
         Engine(uint cx, uint cy);
        ~Engine();

        int ExcuteScene(Scene* scene) const;
        LRESULT LocalProcWnd(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
        static LRESULT CALLBACK GlobalWindProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);


        HWND             m_main_hwnd{};
        Renderer*        get_renderer();
        Renderer*        m_renderer{};
        

    private:
        UINT              m_width = 0;
        UINT              m_height = 0;

	};

}

