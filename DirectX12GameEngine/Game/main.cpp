#include "pch.h"
#include "Engine.h"
#include "RealTimeDebug.h"


int main() 
{
     #ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetDbgFlag(-1);
     #endif // _DEBUG

	ENGINE::Engine app(1240, 860);

	////EngineCreate
	////TODO : 

	auto shutdown = app.ExcuteEngine(nullptr);
	return shutdown;
}