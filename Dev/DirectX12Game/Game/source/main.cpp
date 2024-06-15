#include "Application.h"

#include <stdio.h>

int main() 
{
	ALTAIE::Application app;
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