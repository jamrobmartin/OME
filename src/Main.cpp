#include <iostream>

#include "Game.h"

int main(int argc, char **argv)
{
	printf("Starting...\n");

	try
	{
		int errorCode = 0;

		printf("Instantiate Game...");
		OM::Game* game = new OM::Game();
		printf("Pass.\n");

		printf("Game Init...");
		errorCode = game->Init(argc, argv);
		printf("Pass.\n");

		printf("Game Running...");
		errorCode = game->Run();
		printf("Finished running");
	}
	catch (std::exception e)
	{
		printf("%s\n", e.what());
	}
	
	printf("Ending...\n");
	system("pause");
	return 0;
}
