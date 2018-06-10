#ifndef OM_GAME_H
#define OM_GAME_H

#include "GLIncludes.h"
#include <iostream>
#include <string>

namespace OM
{
	struct GameInitParameters
	{
		std::string title;
		int x;
		int y;
		int width;
		int height;
		bool fullscreen;
	};

	class Game
	{
	public:
		Game();
		~Game();

		int Init(int argc, char **argv);
		int SetInitParameters(GameInitParameters* gip);
		int Run();

		//int SetCameraMode();

	private:
		//GameWorld gw;
		// World

		// Camera

		// Settings?

		// InputProcessor

		// InitParameters
		GameInitParameters myGameInitParameters;
	};

	void renderScene(void);

	void keyDown(unsigned char key, int x, int y);

	void changeSize(int w, int h);
}

#endif // !OM_GAME_H



