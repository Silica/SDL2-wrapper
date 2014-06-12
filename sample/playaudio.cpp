#include <stdio.h>

#include "SDL2_mixer.h"
using namespace SDL2;

struct Application : public EventHandler<Application>
{
	SDL sdl;
	Window window;
	Mix mix;
	Chunk bgm;
	Chunk se;
	Application() : sdl(), window("title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_RESIZABLE), mix()
	{
		bgm = mix.LoadWAV("bgm.ogg");
		if (!bgm)
			printf("need bgm.ogg\n");
		else
			bgm.Play(1, 1);
		se = mix.LoadWAV("se.ogg");
	}

	bool run()
	{
		return sdl.run(*this);
	}

	void onMouseButtonDown(Uint8 button, Sint32 x, Sint32 y)
	{
		se.Play();
	}
	
	void onKeyDown(SDL_Keycode sym, Uint16 mod)
	{
		if (sym == SDLK_ESCAPE)
			sdl.Quit();
		else
			se.Play();
	}
	void onFingerDown(float x, float y)
	{
		sdl.Quit();
	}
};

#pragma comment(linker, "/subsystem:console")
int main(int argc, char **argv)
{
	Application app;
	while (app.run());

	return 0;
}
