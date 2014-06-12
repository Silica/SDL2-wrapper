#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "SDL2.h"
using namespace SDL2;

template<class T>class Array2D
{
public:
	Array2D(size_t width, size_t height)
	{
		t = new T[width * height];
		w = width;
		h = height;
	}
	~Array2D()
	{
		delete[] t;
	}
	Array2D(const Array2D &a)
	{
		size_t aw = a.width();
		size_t ah = a.height();
		t = new T[aw * ah];
		w = aw;
		h = ah;
		for (size_t y = 0; y < ah; y++)
		{
			T *l = t + (w * y);
			T *al = a[y];
			for (size_t x = 0; x < aw; x++)
			{
				l[x] = al[x];
			}
		}
	}
	Array2D &operator=(const Array2D &a)
	{
		size_t mw = w < a.width() ? w : a.width();
		size_t mh = h < a.height() ? h : a.height();
		for (size_t y = 0; y < mh; y++)
		{
			T *l = t + (w * y);
			T *al = a[y];
			for (size_t x = 0; x < mw; x++)
			{
				l[x] = al[x];
			}
		}
		return *this;
	}
	T *operator[](size_t index)const
	{
		return t + (w * index);
	}
	size_t width()	const{return w;}
	size_t height()	const{return h;}
private:
	T *t; // vector<bool> is ...
	size_t w;
	size_t h;
};

class Field
{
public:
	Field(size_t width, size_t height) : f(width, height)
	{
		srand(time(NULL));
		for (size_t y = 0; y < height; y++)
		{
			for (size_t x = 0; x < width; x++)
			{
				f[y][x] = (rand() & 1) ? true : false;
			}
		}
	}
	bool get(int x, int y) const
	{
		if (x < 0)				return false;
		if (x >= f.width())		return false;
		if (y < 0)				return false;
		if (y >= f.height())	return false;
		return f[y][x];
	}
	Field Update() const
	{
		int w = (int)f.width();
		int h = (int)f.height();
		Field New(w, h);
		for (int y = 0; y < h; y++)
		{
			for (int x = 0; x < w; x++)
			{
				int count = 0;
				if (get(x-1, y-1))	count++;
				if (get(x+0, y-1))	count++;
				if (get(x+1, y-1))	count++;
				if (get(x-1, y+0))	count++;
				if (get(x+1, y+0))	count++;
				if (get(x-1, y+1))	count++;
				if (get(x+0, y+1))	count++;
				if (get(x+1, y+1))	count++;

				if (get(x, y))
				{
					if (count == 2 || count == 3)
						New.f[y][x] = true;
					else
						New.f[y][x] = false;
				}
				else
				{
					if (count == 3)
						New.f[y][x] = true;
					else
						New.f[y][x] = false;
				}
			}
		}
		return New;
	}
	size_t width()	const{return f.width();}
	size_t height()	const{return f.height();}
private:
	Array2D<bool> f;
};

struct Application : public EventHandler<Application>
{
	const static int cell_size = 16;
	Field f;
	SDL *sdl;
	Window window;
	Application(SDL *s, Window &w) : f(w.width()/cell_size, w.height()/cell_size)
	{
		sdl = s;
		window = w;
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
		f = f.Update();
		draw();
	}

	void draw()
	{
		auto s = window.GetSurface();
		auto l = Color(128, 0, 128);
		auto d = Color(0, 0, 0);
		for (size_t y = 0; y < f.height(); y++)
		{
			for (size_t x = 0; x < f.width(); x++)
			{
				Rect rect(x * cell_size, y * cell_size, cell_size, cell_size);
				s.FillRect(rect, f.get(x, y) ? l : d);
			}
		}
		window.UpdateSurface();
	}
};

//int WinMain(int,int,int,int)
#pragma comment(linker, "/subsystem:console")
int main(int argc, char **argv)
{
	SDL sdl;
	Window w = sdl.CreateWindow("title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_RESIZABLE);

	sdl.AddTimer(250);

	Application app(&sdl, w);
	while (sdl.run(app));

	return 0;
}
