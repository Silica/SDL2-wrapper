#include <stdio.h>
#include "SDL2_ttf.h"
using namespace SDL2;

struct Application : public EventHandler<Application>
{
	SDL *sdl;
	Window window;
	Surface text;
	Application(SDL *s, Window &w, Surface &t)
	{
		sdl = s;
		window = w;
		text = t;
		draw();
	}
	void onKeyDown(SDL_Keycode sym, Uint16 mod)
	{
		if (sym == SDLK_ESCAPE)
			sdl->Quit();
	}
	void onFingerDown(float x, float y)
	{
		sdl->Quit();
	}

	void onTimer()
	{
		draw();
	}

	void draw()
	{
		static int i = 0;
		auto r = window.CreateRenderer();
		r.SetDrawColor(Color(0, 0, 0));
		r.FillRect(Rect(0, 0, 640, 480));
		auto t = text.CreateTexture(r);
		r.Copy(t, Rect(0, 0, text.width(), text.height()), Rect(i, i, text.width()*2, text.height()*2));
		i++;
		r.Present();
	}
};

//int WinMain(int,int,int,int)
#pragma comment(linker, "/subsystem:console")
int main(int argc, char **argv)
{
	SDL sdl;
	TTF ttf;
	Font f = ttf.OpenFont("arial.ttf", 16); // replace your ttf file
	if (!f)
	{
		printf("need arial.ttf on this directory\n");
		return 1;
	}
	f.SetStyle(TTF_STYLE_ITALIC);
	f.SetOutline(1);
	auto text = f.RenderText_Solid("hello world", Color(255,255,255));
	Window w = sdl.CreateWindow("title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_RESIZABLE);

	sdl.AddTimer(250);

	Application app(&sdl, w, text);
	while (sdl.run(app));

	return 0;
}
