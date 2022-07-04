#include "include/Dependencies.h"
#include <iostream>
#include "include/Engine.h"


Engine gEngine;

int main(int argc, char* argv[])
{
	if (!gEngine.Init())
	{
		printf("Failed to Init Engine");
		return -1;
	}
	else
	{
		gEngine.Run();
	}

	gEngine.ShutDown();
	return 0;
}