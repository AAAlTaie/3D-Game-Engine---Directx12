#pragma once
#include "EngineMin.h"
#include <Windows.h>

namespace ALTAIE 
{

	class ALTAIE_API Application
	{
	public:
		Application();
		~Application();
		bool Initilized();
		void RunWindowMessageHandler();
		bool Running();

	private:
		HWND mWHandle = nullptr;


	};

}


