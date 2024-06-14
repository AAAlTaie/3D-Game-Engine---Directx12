#include "pch.h"
#include "Application.h"

namespace ALTAIE 
{
	LRESULT CALLBACK WindProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}


	Application::Application()
	{

	}

	Application::~Application()
	{
	}

	bool Application::Initilized()
	{
		WNDCLASS winInfo = { 0 };
		winInfo.style = NULL;
		winInfo.lpfnWndProc= WindProc;
		winInfo.cbClsExtra = NULL;
		winInfo.cbWndExtra = NULL;
		winInfo.hInstance = NULL;
		winInfo.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		winInfo.hCursor = LoadCursor(NULL, IDC_ARROW);
		winInfo.hbrBackground = (HBRUSH)COLOR_WINDOW;
		winInfo.lpszMenuName = NULL;
		winInfo.lpszClassName = L"BaseWindowClass";

		RegisterClass(&winInfo);

		mWHandle = CreateWindow(L"BaseWindowClass", L"Abobakr Altaie - DX12 Game ", WS_OVERLAPPEDWINDOW, 200, 200, 800, 600, NULL, NULL, NULL, NULL);

		ShowWindow(mWHandle, SW_SHOW);
		UpdateWindow(mWHandle);
 

		return 1;
	}

	bool Application::Running()
	{
		return 1;
	}

}

