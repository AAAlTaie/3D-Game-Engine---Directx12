#include "Application.h"


using namespace ENGINE;

int main()
{
	Application app;
	if (app.Initialized())
	{
		while (app.Running())
		{
			//while running do something 
			app.RunWindowMessageHandler();
		}
	}


	return 0;

}
