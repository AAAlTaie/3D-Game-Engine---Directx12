#pragma once

#include "EngineMin.h" // Ensure this file exists and is correctly set up
#include <Windows.h>

namespace ALTAIE
{
	class ALTAIE_API Application
	{
	public:
		Application();
		~Application();

		bool Initialized(); // Corrected spelling
		void RunWindowMessageHandler();
		bool Running();

		LRESULT MessageProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	private:
		HWND mWHandle = nullptr;
		int mTestInt = 666;

		// Additional utility functions
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
