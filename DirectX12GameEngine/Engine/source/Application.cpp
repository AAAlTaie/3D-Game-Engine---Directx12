#include "Application.h"

#ifndef GET_X_LPARAM
#define GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#endif

#ifndef GET_Y_LPARAM
#define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))
#endif

namespace ENGINE 
{

	LRESULT CALLBACK WindProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (msg == WM_NCCREATE) {
			LPCREATESTRUCT lPar = reinterpret_cast<LPCREATESTRUCT>(lParam);
			Application* pApp = reinterpret_cast<Application*>(lPar->lpCreateParams);
			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pApp));
		}

		Application* pApp = reinterpret_cast<Application*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

		if (pApp) {
			return pApp->MessageProc(hWnd, msg, wParam, lParam);
		}
		else {
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}
	}

	Application::Application() : mWHandle(nullptr)
	{
	}

	Application::~Application()
	{
		
	}

	bool Application::Initialized()
	{
		WNDCLASS winInfo = {};
		winInfo.lpfnWndProc = WindProc;
		winInfo.hInstance = GetModuleHandle(NULL);
		winInfo.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		winInfo.hCursor = LoadCursor(NULL, IDC_ARROW);
		winInfo.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
		winInfo.lpszClassName = L"BaseWindowClass";

		if (!RegisterClass(&winInfo)) {
			std::cerr << "Failed to register window class!" << std::endl;
			return false;
		}

		mWHandle = CreateWindow(
			L"BaseWindowClass",
			L"Abobakr Altaie - DX12 Game",
			WS_OVERLAPPEDWINDOW,
			200, 200, 800, 600,
			NULL, NULL,
			GetModuleHandle(NULL),
			this
		);

		if (!mWHandle) {
			std::cerr << "Failed to create window!" << std::endl;
			return false;
		}

		ShowWindow(mWHandle, SW_SHOW);
		UpdateWindow(mWHandle);

		return true;
	}

	void Application::RunWindowMessageHandler()
	{
		MSG msg;

		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	bool Application::Running()
	{
		return true;
	}

	LRESULT Application::MessageProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg) {
		case WM_CREATE:
			//this->OnCreate(hWnd); explicit
			OnCreate(hWnd); //implicit
			break;
		case WM_PAINT:
			OnPaint(hWnd);
			break;
		case WM_SIZE:
			OnResize(hWnd, LOWORD(lParam), HIWORD(lParam));
			break;
		case WM_DESTROY:
			OnDestroy(hWnd);
			PostQuitMessage(0);
			break;
		case WM_KEYDOWN:
			OnKeyDown(wParam);
			break;
		case WM_MOUSEMOVE:
			OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			break;
		case WM_LBUTTONDOWN:
			OnLButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			break;
		case WM_LBUTTONUP:
			OnLButtonUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			break;
		case WM_TIMER:
			OnTimer(wParam);
			break;
		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}
		return 0;
	}

	// Additional utility functions
	void Application::OnCreate(HWND hWnd)
	{
		std::cout << "Window created, Initializing Renderer: " << std::endl;
		m_Renderer.InitializeRenderer(hWnd);







	}

	void Application::OnPaint(HWND hWnd)
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);


		// Add your painting code here
		EndPaint(hWnd, &ps);
	}

	void Application::OnResize(HWND hWnd, int width, int height)
	{
		std::cout << "Window resized to: " << width << "x" << height << std::endl;
		// Handle window resizing here
	}

	void Application::OnDestroy(HWND hWnd)
	{
		PostQuitMessage(0);
		std::cout << "Window destroyed" << std::endl;
	}

	void Application::OnKeyDown(WPARAM key)
	{
		std::cout << "Key pressed: " << key << std::endl;
		// Handle key press here
	}

	void Application::OnMouseMove(int x, int y)
	{
		std::cout << "Mouse moved to: " << x << ", " << y << std::endl;
		// Handle mouse movement here
	}

	void Application::OnLButtonDown(int x, int y)
	{
		std::cout << "Left mouse button down at: " << x << ", " << y << std::endl;
		// Handle left mouse button down here
	}

	void Application::OnLButtonUp(int x, int y)
	{
		std::cout << "Left mouse button up at: " << x << ", " << y << std::endl;
		// Handle left mouse button up here
	}

	void Application::OnTimer(WPARAM timerID)
	{
		std::cout << "Timer event: " << timerID << std::endl;
		// Handle timer event here
	}


}