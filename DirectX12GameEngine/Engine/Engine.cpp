#include "pch.h"
#include "Engine.h"
#include "Renderer.h"

namespace ENGINE 
{
	Engine::Engine(uint cx, uint cy) : m_width(cx), m_height(cy)
	{
		WNDCLASSEXA wc{};
		wc.cbSize = sizeof(WNDCLASSEXA);
		wc.hInstance = GetModuleHandleA(nullptr);
		wc.lpfnWndProc = Engine::GlobalWindProc;
		wc.lpszClassName = ENGINE_CLASS_NAME;
		wc.hbrBackground = (HBRUSH)GetStockObject(DKGRAY_BRUSH);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.style = CS_HREDRAW | CS_VREDRAW; //| CS_OWDNC
		
		if(!RegisterClassExA(&wc)) 
		{ 
			printf("failed to create class %s\n", wc.lpszClassName);
			ExitProcess(0);
		}

		CREATESTRUCTA cs{};
		cs.cx = cx;
		cs.cy = cy;
		cs.dwExStyle = NULL;
		cs.hInstance = wc.hInstance;
		cs.hwndParent = nullptr;
		cs.lpCreateParams = this;
		cs.lpszClass = wc.lpszClassName;
		cs.lpszName = "Reaper's Engine";
		cs.hMenu = (HMENU)0;
		cs.style = WS_OVERLAPPEDWINDOW;
		cs.x = (int)((GetSystemMetrics(SM_CXSCREEN) - cx) / 2);
		cs.y = (int)((GetSystemMetrics(SM_CYSCREEN) -  cy) / 2);

		m_main_hwnd = CreateWindowExA(
			cs.dwExStyle,
			cs.lpszClass,
			cs.lpszName,
			cs.style,
			cs.x, cs.y, cs.cx, cs.cy,
			cs.hwndParent,
			cs.hMenu, cs.hInstance, cs.lpCreateParams
		);
	
	
		if (!m_main_hwnd)
		{
			printf("failed to create class %s\n", cs.lpszName);
			ExitProcess(0);

		}

		//TODO : Adjust RECT 
		RECT rc{};
		::GetClientRect(m_main_hwnd, &rc);
		//AdjustWindowRectEx()--BOOL checking 

		AdjustWindowRect(&rc, cs.style, false);
		cx = rc.right - rc.left; //width
		cy = rc.bottom - rc.top; //height

		//TODO : RENDER ini --------- 1
		m_renderer = new Renderer();
		m_renderer->Initialize(cx, cy, m_main_hwnd, 1);
		
		
        //TODO : SHOW WINDOW --------- END
		ShowWindow(m_main_hwnd, SW_SHOW);

	}

	Engine::~Engine()
	{
		safe_delete(m_renderer);
		if (m_main_hwnd) { DestroyWindow(m_main_hwnd); }
		WNDCLASSEXA wc{};
		if (GetClassInfoExA(GetModuleHandle(nullptr), ENGINE_CLASS_NAME, &wc))
		{
			UnregisterClassA(ENGINE_CLASS_NAME, GetModuleHandle(nullptr));
		}
		
	}

	LRESULT CALLBACK Engine::GlobalWindProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
	{
		ENGINE::Engine* engine = (Engine*)GetWindowLongPtrA(hwnd, GWLP_USERDATA);

		if (msg == WM_NCCREATE)
		{
			CREATESTRUCT* pcs = (LPCREATESTRUCT)lp;
			Engine* engine = (Engine*)pcs->lpCreateParams;
			engine->m_main_hwnd = hwnd;
			SetWindowLongPtrA(hwnd, GWLP_USERDATA, (LONG_PTR)engine);
		}

		if (engine) return engine->LocalProcWnd(hwnd, msg, wp, lp);
		return DefWindowProcA(hwnd, msg, wp, lp);
	}

	int Engine::ExcuteEngine(void* scene) const
	{
		MSG msg{};
		while (msg.message != WM_QUIT)
		{
			while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_KEYDOWN && msg.wParam == VK_ESCAPE)
				{
					PostQuitMessage(0);
				}
				TranslateMessage(&msg);
				DispatchMessage(&msg);
				
			}
			//TODO : update and render
			if (m_renderer) { 
			
				//TIMER SETUP (Reaper)
				m_renderer->begin_frame();

				//TODO : Scene UPDATING
				m_renderer->end_frame();
			}

			//TODO :


		}
		//TODO : Renderer uniniatilize
		m_renderer->reset();
		
		

		//TODO : release all resources ur caches
		//TODO : Dx12 Live Reporring 
		IDXGIDebug1* debug{};
		DXGI_DEBUG_RLO_FLAGS flags = (DXGI_DEBUG_RLO_FLAGS)(DXGI_DEBUG_RLO_DETAIL | DXGI_DEBUG_RLO_IGNORE_INTERNAL);
		if (SUCCEEDED(DXGIGetDebugInterface1(0,IID_PPV_ARGS(&debug))))
		{
			debug->ReportLiveObjects(DXGI_DEBUG_ALL, flags);
			safe_release(debug);
		}

		return (int)msg.lParam; 
	}


	LRESULT Engine::LocalProcWnd(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
	{
		switch (msg)
		{
		case WM_CLOSE: 
		{
			SendMessage(hWnd, WM_QUIT, 0, 0);
			PostQuitMessage(0);
			break;
		}

		default:
			break;
		}

		return DefWindowProcA(hWnd, msg, wp, lp);
	}
}