#include <stdio.h>

#include "SDL2_image.h"
using namespace SDL2;

struct Application : public EventHandler<Application>
{
	SDL sdl;
	Window window;
	IMG img;
	Surface surface;
	Application() : sdl(), window("title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_RESIZABLE), img()
	{
		surface = img.Load("sample.png");
		if (!surface)
		{
			printf("need sample.png\n");
		}
		auto s = window.GetSurface();
		s.BlitScaled(surface);
		window.UpdateSurface();
	}

	bool run()
	{
		return sdl.run(*this);
	}

	void onMouseButtonDown(Uint8 button, Sint32 x, Sint32 y)
	{
		auto s = window.GetSurface();
		s.Blit(surface, x, y);
		window.UpdateSurface();
	}
	
	void onKeyDown(SDL_Keycode sym, Uint16 mod)
	{
		if (sym == SDLK_ESCAPE)
			sdl.Quit();
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
