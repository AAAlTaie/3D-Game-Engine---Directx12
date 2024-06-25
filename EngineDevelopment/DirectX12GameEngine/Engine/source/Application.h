#pragma once
#include "EngineCore.h"
#include <Windows.h>
#include  "Renderer/Renderer.h"


namespace ENGINE
{

	class ENGINE_API Application
	{

	public:

		Application();
		~Application();

		bool Initialized();
		void RunWindowMessageHandler();
		bool Running();

		LRESULT MessageProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	private:
		

		HWND mWHandle = nullptr;
		Renderer m_Renderer;

		// Additional utility functions for my window
		void OnCreate(HWND hWnd);
		void OnPaint(HWND hWnd);
		void OnResize(HWND hWnd, int width, int height);
		void OnDestroy(HWND hWnd);
		void OnKeyDown(WPARAM key);
		void OnMouseMove(int x, int y);
		void OnLButtonDown(int x, int y);
		void OnLButtonUp(int x, int y);
		void OnTimer(WPARAM timerID);



	};






}


