#include "pch.h"
#include "Engine.h"
#include "RealTimeDebug.h"
#include "Scenes/UnitTestScene.h"

int main() 
{
     #ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetDbgFlag(-1);
     #endif // _DEBUG

	ENGINE::Engine app(1240, 860);
	ENGINE::UnitTestScene scene0(app.get_renderer());


	auto shutdown = app.ExcuteScene(&scene0);
	return shutdown;
}