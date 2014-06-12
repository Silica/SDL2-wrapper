#ifndef SDL2_WRAPPER_H
#define SDL2_WRAPPER_H
/*Copyright (c) 2014 Silica
This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.*/

#ifdef __ANDROID__
#include "SDL.h"
#else
#include "SDL2/SDL.h"
#endif

#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2main.lib")

namespace SDL2
{
	template<class T>struct Deleter{void operator()(T *t){delete t;}};
	template<class T, class D = Deleter<T> >class shptr
	{
	public:
		shptr(){x = NULL;rc = NULL;}
		shptr(T *t){x = t;setrc();}
		shptr(const shptr &s)
		{
			x = s.x;
			inc(s.rc);
		}
		shptr &operator=(const shptr &s)
		{
			dec();
			x = s.x;
			inc(s.rc);
			return *this;
		}
		shptr &operator=(T *t)
		{
			dec();
			x = t;
			setrc();
			return *this;
		}
		      T *get()            {return x;}
		const T *get()       const{return x;}
		      T *operator->()     {return x;}
		const T *operator->()const{return x;}
		      T &operator*()      {return *x;}
		const T &operator*() const{return *x;}
		operator       T*()       {return x;}
		operator const T*()  const{return x;}
		operator       T&()       {return *x;}
		operator const T&()  const{return *x;}
		operator bool()const{return x != NULL;}
		~shptr(){dec();}
		void release(){dec();}
	private:
		void setrc()
		{
			if (x)
				rc = new int(1);
			else
				rc = NULL;
		}
		void inc(int *r)
		{
			rc = r;
			if (rc)
				++*rc;
		}
		void dec()
		{
			if (rc)
			{
				if (!--*rc)
				{
					delete rc;
					D()(x);
				}
				rc = NULL;
				x = NULL;
			}
		}
		T *x;
		int *rc;
	};

	struct Rect
	{
		Rect(){}
		Rect(int x, int y, int w, int h)
		{
			rect.x = x;
			rect.y = y;
			rect.w = w;
			rect.h = h;
		}
		int X()     const{return rect.x;}
		int Y()     const{return rect.y;}
		int width() const{return rect.w;}
		int height()const{return rect.h;}
		operator       SDL_Rect*()     {return &rect;}
		operator const SDL_Rect*()const{return &rect;}
		operator       SDL_Rect&()     {return rect;}
		operator const SDL_Rect&()const{return rect;}
		bool HasIntersection(const Rect &r){return SDL_HasIntersection(&rect, r);}
		Rect IntersectRect(const Rect &r)
		{
			Rect result;
			SDL_IntersectRect(&rect, r, result);
			return result;
		}
		Rect UnionRect(const Rect &r)
		{
			Rect result;
			SDL_UnionRect(&rect, r, result);
			return result;
		}
		SDL_Rect rect;
	};

	struct Color
	{
		Color(){}
		Color(int r, int g, int b, int a = 255)
		{
			color.r = r;
			color.g = g;
			color.b = b;
			color.a = a;
		}
		int A()const{return color.a;}
		int R()const{return color.r;}
		int G()const{return color.g;}
		int B()const{return color.b;}
		operator       SDL_Color*()     {return &color;}
		operator const SDL_Color*()const{return &color;}
		operator       SDL_Color&()     {return color;}
		operator const SDL_Color&()const{return color;}
		operator Uint32()const{return (color.a << 24) | (color.r << 16) | (color.g << 8) | color.b;}
		SDL_Color color;
	};

	struct Point
	{
		Point(int x = 0, int y = 0)
		{
			point.x = x;
			point.y = y;
		}
		int X()const{return point.x;}
		int Y()const{return point.y;}
		operator       SDL_Point*()     {return &point;}
		operator const SDL_Point*()const{return &point;}
		operator       SDL_Point&()     {return point;}
		operator const SDL_Point&()const{return point;}
		SDL_Point point;
	};

	struct DisplayMode
	{
		Uint32 format()      {return mode.format;}
		int    width()  const{return mode.w;}
		int    height() const{return mode.h;}
		int    refresh_rate(){return mode.refresh_rate;}
		operator       SDL_DisplayMode*()     {return &mode;}
		operator const SDL_DisplayMode*()const{return &mode;}
		operator       SDL_DisplayMode&()     {return mode;}
		operator const SDL_DisplayMode&()const{return mode;}
		SDL_DisplayMode mode;
	};

	class Texture
	{
	public:
		Texture(){}
		Texture(SDL_Texture *t):texture(t){}
		      SDL_Texture *get()          {return texture;}
		const SDL_Texture *get()     const{return texture;}
		operator       SDL_Texture*()     {return texture;}
		operator const SDL_Texture*()const{return texture;}
		operator bool(){return texture;}

		Color         GetColorMod()                        {Color c;SDL_GetTextureColorMod(texture, &c.color.r, &c.color.g, &c.color.b);return c;}
		int           SetColorMod(const Color &color)      {return SDL_SetTextureColorMod(texture, color.R(), color.G(), color.B());}
		Uint8         GetAlphaMod()                        {Uint8 alpha;SDL_GetTextureAlphaMod(texture, &alpha);return alpha;}
		int           SetAlphaMod(Uint8 alpha)             {return SDL_SetTextureAlphaMod(texture, alpha);}
		SDL_BlendMode GetBlendMode()                       {SDL_BlendMode blendMode;SDL_GetTextureBlendMode(texture, &blendMode);return blendMode;}
		int           SetBlendMode(SDL_BlendMode blendMode){return SDL_SetTextureBlendMode(texture, blendMode);}
	private:
		struct Deleter{void operator()(SDL_Texture *t){SDL_DestroyTexture(t);}};
		shptr<SDL_Texture, Deleter> texture;
	};

	class Renderer
	{
	public:
		Renderer(){}
		Renderer(SDL_Renderer *r):renderer(r){}
		      SDL_Renderer *get()          {return renderer;}
		const SDL_Renderer *get()     const{return renderer;}
		operator       SDL_Renderer*()     {return renderer;}
		operator const SDL_Renderer*()const{return renderer;}
		operator bool(){return renderer;}

		Color         GetDrawColor()                           {Color c;SDL_GetRenderDrawColor(renderer, &c.color.r, &c.color.g, &c.color.b, &c.color.a);return c;}
		int           SetDrawColor(const Color &color)         {return SDL_SetRenderDrawColor(renderer, color.R(), color.G(), color.B(), color.A());}
		SDL_BlendMode GetDrawBlendMode()                       {SDL_BlendMode blendMode;SDL_GetRenderDrawBlendMode(renderer, &blendMode);return blendMode;}
		int           SetDrawBlendMode(SDL_BlendMode blendMode){return SDL_SetRenderDrawBlendMode(renderer, blendMode);}

		int Clear(){return SDL_RenderClear(renderer);}
		void Present(){SDL_RenderPresent(renderer);}

		Texture CreateTexture(Uint32 format, int access, int w, int h){return SDL_CreateTexture(renderer, format, access, w, h);}
		Texture GetTarget()                                           {return SDL_GetRenderTarget(renderer);}
		int     SetTarget(Texture &texture)                           {return SDL_SetRenderTarget(renderer, texture);}

		int Copy  (Texture &texture, const Rect &srcrect, const Rect &dstrect)	{return SDL_RenderCopy(renderer, texture, srcrect, dstrect);}
		int CopyEx(Texture &texture, const Rect &srcrect, const Rect &dstrect,
					const double angle = 0, const SDL_Point *center = nullptr, const SDL_RendererFlip flip = SDL_FLIP_NONE)	{return SDL_RenderCopyEx(renderer, texture, srcrect, dstrect, angle, center, flip);}

		int DrawLine(int x1, int y1, int x2, int y2)      {return SDL_RenderDrawLine(renderer, x1, y1, x2, y2);}
		int DrawLines(const SDL_Point* points, int count) {return SDL_RenderDrawLines(renderer, points, count);}
		int DrawPoint(int x, int y)                       {return SDL_RenderDrawPoint(renderer, x, y);}
		int DrawPoints(const SDL_Point* points, int count){return SDL_RenderDrawPoints(renderer, points, count);}
		int DrawRect(const Rect &rect)                    {return SDL_RenderDrawRect(renderer, rect);}
		int DrawRects(const SDL_Rect* rects, int count)   {return SDL_RenderDrawRects(renderer, rects, count);}
		int FillRect(const Rect &rect)                    {return SDL_RenderFillRect(renderer, rect);}
		int FillRects(const SDL_Rect* rects, int count)   {return SDL_RenderFillRects(renderer, rects, count);}
	private:
		struct Deleter{void operator()(SDL_Renderer *r){SDL_DestroyRenderer(r);}};
		shptr<SDL_Renderer, Deleter> renderer;
	};

	class Cursor
	{
	public:
		Cursor(){}
		Cursor(SDL_Cursor *c):cursor(c){}
		      SDL_Cursor *get()          {return cursor;}
		const SDL_Cursor *get()     const{return cursor;}
		operator       SDL_Cursor*()     {return cursor;}
		operator const SDL_Cursor*()const{return cursor;}
		operator bool(){return cursor;}
	private:
		struct Deleter{void operator()(SDL_Cursor *c){SDL_FreeCursor(c);}};
		shptr<SDL_Cursor, Deleter> cursor;
	};

	class Surface
	{
	public:
		Surface(){}
		Surface(SDL_Surface *s):surface(s){}
		      SDL_Surface *get()          {return surface;}
		const SDL_Surface *get()     const{return surface;}
		operator       SDL_Surface*()     {return surface;}
		operator const SDL_Surface*()const{return surface;}
		operator bool(){return surface;}

		int    width() const{return surface->w;}
		int    height()const{return surface->h;}
		Uint32 format()const{return surface->format->format;}
		void*  pixels()const{return surface->pixels;}

		static Surface  CreateRGB(int width, int height) {return SDL_CreateRGBSurface(0, width, height, 24, 0xFF, 0xFF00, 0xFF0000, 0);}
		static Surface  CreateRGBA(int width, int height){return SDL_CreateRGBSurface(0, width, height, 32, 0xFF, 0xFF00, 0xFF0000, 0xFF000000);}
		       Surface  Convert(Uint32 pixel_format)     {return SDL_ConvertSurfaceFormat(surface, pixel_format, 0);}
		Renderer CreateRenderer()					{return SDL_CreateSoftwareRenderer(surface);}
		Texture  CreateTexture(Renderer &renderer)	{return SDL_CreateTextureFromSurface(renderer, surface);}

		int Blit      (Surface &src, const Rect &srcrect, Rect &dstrect)       {return SDL_BlitSurface(src, srcrect, surface, dstrect);}
		int Blit      (Surface &src)                                           {return SDL_BlitSurface(src, nullptr, surface, nullptr);}
		int Blit      (Surface &src,                      int x,     int y)    {return SDL_BlitSurface(src, nullptr, surface, Rect(x, y, src.width(), src.height()));}
		int Blit      (Surface &src, const Rect &srcrect, int x = 0, int y = 0){return SDL_BlitSurface(src, srcrect, surface, Rect(x, y, src.width(), src.height()));}
		int BlitScaled(Surface &src)                                           {return SDL_BlitScaled(src, nullptr, surface, nullptr);}
		int BlitScaled(Surface &src, const Rect &srcrect, Rect &dstrect)       {return SDL_BlitScaled(src, srcrect, surface, dstrect);}

		int FillRect (const Rect &rect,                 const Color &color){return SDL_FillRect(surface, rect, color);}
		int FillRects(const SDL_Rect* rects, int count, const Color &color){return SDL_FillRects(surface, rects, count, color);}

		static Surface LoadBMP(const char *file){return SDL_LoadBMP(file);}
		int            SaveBMP(const char *file){return SDL_SaveBMP(surface, file);}
	private:
		struct Deleter{void operator()(SDL_Surface *r){SDL_FreeSurface(r);}};
		shptr<SDL_Surface, Deleter> surface;
	};

	class Window
	{
	public:
		Window(){}
		Window(const char* title, int x, int y, int w, int h, Uint32 flags):window(SDL_CreateWindow(title, x, y, w, h, flags)){}
		Window(SDL_Window *w):window(w){}
		      SDL_Window *get()          {return window;}
		const SDL_Window *get()     const{return window;}
		operator       SDL_Window*()     {return window;}
		operator const SDL_Window*()const{return window;}
		operator bool(){return window;}

		int width() {int w, h;SDL_GetWindowSize(window, &w, &h);return w;}
		int height(){int w, h;SDL_GetWindowSize(window, &w, &h);return h;}
		int X()     {int x, y;SDL_GetWindowPosition(window, &x, &y);return x;}
		int Y()     {int x, y;SDL_GetWindowPosition(window, &x, &y);return y;}
		Rect getRect()                           {int x, y, w, h;SDL_GetWindowPosition(window, &x, &y);SDL_GetWindowSize(window, &w, &h);return Rect(x, y, w, h);}
		void SetSize    (int w, int h)           {SDL_SetWindowSize(window, w, h);}
		void SetPosition(int x, int y)           {SDL_SetWindowPosition(window, x, y);}
		void SetRect    (const Rect &rect)       {SDL_SetWindowSize(window, rect.width(), rect.height());SDL_SetWindowPosition(window, rect.X(), rect.Y());}
		Rect GetMaximumSize()                    {int w, h;SDL_GetWindowMaximumSize(window, &w, &h);return Rect(0, 0, w, h);}
		Rect GetMinimumSize()                    {int w, h;SDL_GetWindowMinimumSize(window, &w, &h);return Rect(0, 0, w, h);}
		void SetMaximumSize(int max_w, int max_h){SDL_SetWindowMaximumSize(window, max_w, max_h);}
		void SetMinimumSize(int min_w, int min_h){SDL_SetWindowMinimumSize(window, min_w, min_h);}
		const char* GetTitle()                     {return SDL_GetWindowTitle(window);}
		void        SetTitle(const char *title)    {SDL_SetWindowTitle(window, title);}
		Uint32      GetID()                        {return SDL_GetWindowID(window);}
		Uint32      GetFlags()                     {return SDL_GetWindowFlags(window);}
		void        SetBordered(bool bordered)     {SDL_SetWindowBordered(window, bordered ? SDL_TRUE : SDL_FALSE);}
//		float       GetBrightness()                {return SDL_GetWindowBrightness(window);}
//		int         SetBrightness(float brightness){return SDL_SetWindowBrightness(window, brightness);}
		Uint32      GetPixelFormat()               {return SDL_GetWindowPixelFormat(window);}
		void        SetIcon(Surface &surface)      {SDL_SetWindowIcon(window, surface);}

		void Show()    {SDL_ShowWindow(window);}
		void Hide()    {SDL_HideWindow(window);}
		void Raise()   {SDL_RaiseWindow(window);}
		void Maximize(){SDL_MaximizeWindow(window);}
		void Minimize(){SDL_MinimizeWindow(window);}
		void Restore() {SDL_RestoreWindow(window);}
		int SetFullscreen(Uint32 flags = SDL_WINDOW_FULLSCREEN_DESKTOP){return SDL_SetWindowFullscreen(window, flags);}

//		void WarpMouse(int x, int y){SDL_WarpMouseInWindow(window, x, y);}

		int ShowSimpleMessageBox(Uint32 flags, const char* title, const char* message){return SDL_ShowSimpleMessageBox(flags, title, message, window);}

		Surface GetSurface(){return SDL_GetWindowSurface(window);}
		int UpdateSurface(){return SDL_UpdateWindowSurface(window);}
		Renderer GetRenderer(){return SDL_GetRenderer(window);}
		Renderer CreateRenderer(int index = -1, Uint32 flags = SDL_RENDERER_ACCELERATED){return SDL_CreateRenderer(window, index, flags);}
	private:
		struct Deleter{void operator()(SDL_Window *r){SDL_DestroyWindow(r);}};
		shptr<SDL_Window, Deleter> window;
	};

	struct Mouse
	{
		Uint32 button;
		int x;
		int y;
	};

	class Joystick
	{
	public:
		Joystick(){}
		Joystick(int device_index):joystick(SDL_JoystickOpen(device_index)){}
		Joystick(SDL_Joystick *j) :joystick(j){}
		      SDL_Joystick *get()          {return joystick;}
		const SDL_Joystick *get()     const{return joystick;}
		operator       SDL_Joystick*()     {return joystick;}
		operator const SDL_Joystick*()const{return joystick;}
		operator bool(){return joystick;}

		void Update(){SDL_JoystickUpdate();}
		int EventState(int state){return SDL_JoystickEventState(state);}
		bool GetAttached(){return SDL_JoystickGetAttached(joystick);}

		int NumAxes()    {return SDL_JoystickNumAxes(joystick);}
		int NumBalls()   {return SDL_JoystickNumBalls(joystick);}
		int NumButtons() {return SDL_JoystickNumButtons(joystick);}
		int NumHats()    {return SDL_JoystickNumHats(joystick);}
		struct Ball
		{
			int dx;
			int dy;
		};
		Sint16 GetAxis  (int axis)   {return SDL_JoystickGetAxis(joystick, axis);}
		Ball   GetBall  (int ball)   {Ball b;SDL_JoystickGetBall(joystick, ball, &b.dx, &b.dy);return b;}
		Uint8  GetButton(int button) {return SDL_JoystickGetButton(joystick, button);}
		Uint8  GetHat   (int hat = 0){return SDL_JoystickGetHat(joystick, hat);}

		SDL_JoystickGUID GetGUID(){return SDL_JoystickGetGUID(joystick);}
	private:
		struct Deleter{void operator()(SDL_Joystick *j){SDL_JoystickClose(j);}};
		shptr<SDL_Joystick, Deleter> joystick;
	};

	struct Event
	{
		Event(SDL_EventType type)
		{
			event.type = type;
		}
		Event(Sint32 code = 0, void *data1 = nullptr, void *data2 = nullptr)
		{
			event.type = SDL_USEREVENT;
			event.user.code = code;
			event.user.data1 = data1;
			event.user.data2 = data2;
		}
		operator SDL_Event*(){return &event;}
		operator SDL_Event&(){return event;}
		SDL_Event event;
	};

	class RWops
	{
	public:
		RWops(){}
		RWops(SDL_RWops *p):rw(p){}
		RWops(      void *mem,  int size)        :rw(SDL_RWFromMem(mem, size)){}
		RWops(const void *mem,  int size)        :rw(SDL_RWFromConstMem(mem, size)){}
		#ifdef HAVE_STDIO_H
		RWops(      FILE *fp ,  bool autoclose)  :rw(SDL_RWFromFP(fp, autoclose ? SDL_TRUE : SDL_FALSE)){}
		#endif
		RWops(const char *file, const char* mode):rw(SDL_RWFromFile(file, mode)){}
		      SDL_RWops *get()          {return rw;}
		const SDL_RWops *get()     const{return rw;}
		operator       SDL_RWops*()     {return rw;}
		operator const SDL_RWops*()const{return rw;}
		operator bool(){return rw;}

		size_t read (void* ptr, size_t size, size_t maxnum){return SDL_RWread(rw, ptr, size, maxnum);}
		size_t write(void* ptr, size_t size, size_t maxnum){return SDL_RWwrite(rw, ptr, size, maxnum);}
		Sint64 seek (Sint64 offset, int whence)            {return SDL_RWseek(rw, offset, whence);}
		Sint64 tell ()                                     {return SDL_RWtell(rw);}

		Uint16 ReadBE16(){return SDL_ReadBE16(rw);}
		Uint32 ReadBE32(){return SDL_ReadBE32(rw);}
		Uint64 ReadBE64(){return SDL_ReadBE64(rw);}
		Uint16 ReadLE16(){return SDL_ReadLE16(rw);}
		Uint32 ReadLE32(){return SDL_ReadLE32(rw);}
		Uint64 ReadLE64(){return SDL_ReadLE64(rw);}

		size_t WriteBE16(Uint16 value){return SDL_WriteBE16(rw, value);}
		size_t WriteBE32(Uint32 value){return SDL_WriteBE32(rw, value);}
		size_t WriteBE64(Uint64 value){return SDL_WriteBE64(rw, value);}
		size_t WriteLE16(Uint16 value){return SDL_WriteLE16(rw, value);}
		size_t WriteLE32(Uint32 value){return SDL_WriteLE32(rw, value);}
		size_t WriteLE64(Uint64 value){return SDL_WriteLE64(rw, value);}

		int close(){return SDL_RWclose(rw);}
	private:
		struct Deleter{void operator()(SDL_RWops *rw){SDL_FreeRW(rw);}};
		shptr<SDL_RWops, Deleter> rw;
	};

	class SDL
	{
	public:
		SDL(Uint32 flags = SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_EVENTS)
		{
			SDL_Init(flags);
			SDL_RegisterEvents(1);	// it will return a value same as SDL_USEREVENT at first
		}
		~SDL(){SDL_Quit();}
		int InitSubSystem(Uint32 flags) {return SDL_InitSubSystem(flags);}
		Uint32 WasInit(Uint32 flags = 0){return SDL_WasInit(flags);}
		operator bool(){return WasInit();}

		const char* GetError(){return SDL_GetError();}
		SDL_version GetVersion(){SDL_version v;SDL_GetVersion(&v);return v;}
		DisplayMode GetCurrentDisplayMode(int displayIndex = 0)
		{
			DisplayMode dm;
			SDL_GetCurrentDisplayMode(displayIndex, dm);
			return dm;
		}

		int ShowSimpleMessageBox(const char* title, const char* message, Uint32 flags = SDL_MESSAGEBOX_INFORMATION){return SDL_ShowSimpleMessageBox(flags, title, message, nullptr);}
		Window CreateWindow(const char* title, int x, int y, int w, int h, Uint32 flags){return SDL_CreateWindow(title, x, y, w, h, flags);}
		void EnableScreenSaver()     {SDL_EnableScreenSaver();}
		void DisableScreenSaver()    {SDL_DisableScreenSaver();}
		void ScreenSaver(bool enable){enable ? EnableScreenSaver() : DisableScreenSaver();}

												struct Deleter{void operator()(void*p){SDL_free(p);}};
		bool HasClipboardText()					{return SDL_HasClipboardText();}
		shptr<char, Deleter> GetClipboardText()	{return SDL_GetClipboardText();}
		int SetClipboardText(const char* text)	{return SDL_SetClipboardText(text);}

//		Window       GetKeyboardFocus()                      {return SDL_GetKeyboardFocus();}
		// これを行うとこっちのWindowが消えた時点でSDL_Windowが削除される
		// SDLの中にWindowのリストを持っておく、という手はあるが
		// SDL_GetWindowFromIDとかも同じことになるが、どうする
		// SDL_GetMouseFocus
		const Uint8* GetKeyboardState(int *numkeys = nullptr){return SDL_GetKeyboardState(numkeys);}
		SDL_Keymod   SDL_GetModState()                       {return SDL_GetModState();}

		Cursor CreateColorCursor(Surface &surface, int hot_x, int hot_y)                             {return SDL_CreateColorCursor(surface, hot_x, hot_y);}
		Cursor CreateCursor(const Uint8* data, const Uint8* mask, int w, int h, int hot_x, int hot_y){return SDL_CreateCursor(data, mask, w, h, hot_x, hot_y);}
		Cursor CreateSystemCursor(SDL_SystemCursor id)                                               {return SDL_CreateSystemCursor(id);}
		Cursor GetCursor()                                                                           {return SDL_GetCursor();}
		Cursor GetDefaultCursor()                                                                    {return SDL_GetDefaultCursor();}
		void   SetCursor(Cursor &cursor)                                                             {return SDL_SetCursor(cursor);}
		int    ShowCursor(int toggle = 1){return SDL_ShowCursor(toggle);}
		int    HideCursor()              {return SDL_ShowCursor(0);}
		bool   isShowCursor()            {return SDL_ShowCursor(-1) == 1;}
		Mouse GetMouseState()
		{
			Mouse m;
			m.button = SDL_GetMouseState(&m.x, &m.y);
			return m;
		}

		Joystick JoystickOpen(int device_index){return SDL_JoystickOpen(device_index);}

		SDL_TimerID AddTimer(Uint32 interval, SDL_TimerCallback callback, void *param){return SDL_AddTimer(interval, callback, param);}
		bool        RemoveTimer(SDL_TimerID id)                                       {return SDL_RemoveTimer(id);}
		Uint64      GetPerformanceCounter()                                           {return SDL_GetPerformanceCounter();}
		Uint64      GetPerformanceFrequency()                                         {return SDL_GetPerformanceFrequency();}
		Uint32      GetTicks()                                                        {return SDL_GetTicks();}

		static Uint32 timer_callbackfunc(Uint32 interval, void *param)
		{
			Event e((SDL_EventType)*(int*)param);
			SDL_PushEvent(e);
			return interval;
		}
		SDL_TimerID AddTimer(Uint32 interval)
		{
			if (!timer_event_id)
				timer_event_id = SDL_RegisterEvents(1);
			return SDL_AddTimer(interval, timer_callbackfunc, &timer_event_id);
		}
		Uint32 timer_event_id = 0;

		shptr<char, Deleter> GetBasePath()                                {return SDL_GetBasePath();}
		shptr<char, Deleter> GetPrefPath(const char* org, const char* app){return SDL_GetPrefPath(org, app);}
		const char*          GetPlatform()                                {return SDL_GetPlatform();}

		void* LoadObject  (const char* sofile)            {return SDL_LoadObject(sofile);}
		void  UnloadObject(void* handle)                  {return SDL_UnloadObject(handle);}
		void* LoadFunction(void *handle, const char *name){return SDL_LoadFunction(handle, name);}

		SDL_Event event;
		int    WaitEvent(Event &e){return SDL_WaitEvent(e);}
		int    WaitEvent()        {return SDL_WaitEvent(&event);}
		int    PushEvent(Event &e){return SDL_PushEvent(e);}
		Uint32 RegisterEvents(int numevents){return SDL_RegisterEvents(numevents);}

		template<class EH>bool run(EH &e)
		{
			int i = WaitEvent();
			if (!i)
				return false;

			if (event.type == timer_event_id)
			{
				e.onTimer();
				return true;
			}
			switch (event.type)
			{
			case SDL_QUIT:                   return e.onQuit();
			case SDL_APP_TERMINATING:        e.onDestroy();break;
			case SDL_APP_LOWMEMORY:          e.onLowMemory();break;
			case SDL_APP_WILLENTERBACKGROUND:e.onBeforeBackground();break;
			case SDL_APP_DIDENTERBACKGROUND: e.onAfterBackground();break;
			case SDL_APP_WILLENTERFOREGROUND:e.onBeforeForeground();break;
			case SDL_APP_DIDENTERFOREGROUND: e.onAfterForeground();break;
			case SDL_WINDOWEVENT:            e.onWindowEvent(event.window);break;
			case SDL_SYSWMEVENT:             e.onSysWMEvent(event.syswm);break;
			case SDL_KEYDOWN:                e.onKeyDownEvent(event.key);break;
			case SDL_KEYUP:                  e.onKeyUpEvent(event.key);break;
			case SDL_TEXTEDITING:            e.onTextEditing(event.edit);break;
			case SDL_TEXTINPUT:              e.onTextInput(event.text);break;
			case SDL_MOUSEMOTION:            e.onMouseMotionEvent(event.motion);break;
			case SDL_MOUSEBUTTONDOWN:        e.onMouseButtonDownEvent(event.button);break;
			case SDL_MOUSEBUTTONUP:          e.onMouseButtonUpEvent(event.button);break;
			case SDL_MOUSEWHEEL:             e.onMouseWheelEvent(event.wheel);break;
			case SDL_JOYAXISMOTION:          e.onJoyAxisMotionEvent(event.jaxis);break;
			case SDL_JOYBALLMOTION:          e.onJoyBallMotionEvent(event.jball);break;
			case SDL_JOYHATMOTION:           e.onJoyHatMotionEvent(event.jhat);break;
			case SDL_JOYBUTTONDOWN:          e.onJoyButtonDownEvent(event.jbutton);break;
			case SDL_JOYBUTTONUP:            e.onJoyBUttonUpEvent(event.jbutton);break;
			case SDL_JOYDEVICEADDED:         e.onJoyDeviceAdded(event.jdevice);break;
			case SDL_JOYDEVICEREMOVED:       e.onJoyDeviceRemoved(event.jdevice);break;
			case SDL_CONTROLLERAXISMOTION:    e.onControllerAxisMotion(event.caxis);break;
			case SDL_CONTROLLERBUTTONDOWN:    e.onControllerButtonDown(event.cbutton);break;
			case SDL_CONTROLLERBUTTONUP:      e.onControllerButtonUp(event.cbutton);break;
			case SDL_CONTROLLERDEVICEADDED:   e.onControllerDeviceAdded(event.cdevice);break;
			case SDL_CONTROLLERDEVICEREMOVED: e.onControllerDeviceRemoved(event.cdevice);break;
			case SDL_CONTROLLERDEVICEREMAPPED:e.onControllerDeviceMapped(event.cdevice);break;
			case SDL_FINGERDOWN:             e.onFingerDownEvent(event.tfinger);break;
			case SDL_FINGERUP:               e.onFingerUpEvent(event.tfinger);break;
			case SDL_FINGERMOTION:           e.onFingerMotionEvent(event.tfinger);break;
			case SDL_CLIPBOARDUPDATE:        e.onClipboardUpdate();break;
			case SDL_DROPFILE:               e.onDropFileEvent(event.drop);break;
			case SDL_DOLLARGESTURE:
			case SDL_DOLLARRECORD:
			case SDL_MULTIGESTURE:
			case SDL_RENDER_TARGETS_RESET:
				break;
			case SDL_USEREVENT:
				e.onUserEvent(event.user.code, event.user.data1, event.user.data2);
				break;
			}

			return true;
		}
		void Quit()
		{
			Event e(SDL_QUIT);
			PushEvent(e);
		}
	};

	template<class T> struct EventHandler
	{
		bool onQuit(){return false;}
		void onDestroy(){}
		void onLowMemory(){}
		void onBeforeBackground(){}
		void onAfterBackground(){}
		void onBeforeForeground(){}
		void onAfterForeground(){}

		void onWindowShown(){}
		void onWindowHidden(){}
		void onWindowExposed(){}
		void onWindowMoved      (Sint32 x,     Sint32 y){}
		void onWindowResized    (Sint32 width, Sint32 height){}
		void onWindowSizeChanged(Sint32 width, Sint32 height){}
		void onWindowMinimized(){}
		void onWindowMaximized(){}
		void onWindowRestored(){}
		void onWindowEnter(){}
		void onWindowLeave(){}
		void onWindowFocusGained(){}
		void onWindowFocusLost(){}
		void onWindowClose(){}
		void onWindowEvent(SDL_WindowEvent &window)
		{
			switch (window.event)
			{
			case SDL_WINDOWEVENT_SHOWN:       static_cast<T*>(this)->onWindowShown();break;
			case SDL_WINDOWEVENT_HIDDEN:      static_cast<T*>(this)->onWindowHidden();break;
			case SDL_WINDOWEVENT_EXPOSED:     static_cast<T*>(this)->onWindowExposed();break;
			case SDL_WINDOWEVENT_MOVED:       static_cast<T*>(this)->onWindowMoved(window.data1, window.data2);break;
			case SDL_WINDOWEVENT_RESIZED:     static_cast<T*>(this)->onWindowResized(window.data1, window.data2);break;
			case SDL_WINDOWEVENT_SIZE_CHANGED:static_cast<T*>(this)->onWindowSizeChanged(window.data1, window.data2);break;
			case SDL_WINDOWEVENT_MINIMIZED:   static_cast<T*>(this)->onWindowMinimized();break;
			case SDL_WINDOWEVENT_MAXIMIZED:   static_cast<T*>(this)->onWindowMaximized();break;
			case SDL_WINDOWEVENT_RESTORED:    static_cast<T*>(this)->onWindowRestored();break;
			case SDL_WINDOWEVENT_ENTER:       static_cast<T*>(this)->onWindowEnter();break;
			case SDL_WINDOWEVENT_LEAVE:       static_cast<T*>(this)->onWindowLeave();break;
			case SDL_WINDOWEVENT_FOCUS_GAINED:static_cast<T*>(this)->onWindowFocusGained();break;
			case SDL_WINDOWEVENT_FOCUS_LOST:  static_cast<T*>(this)->onWindowFocusLost();break;
			case SDL_WINDOWEVENT_CLOSE:       static_cast<T*>(this)->onWindowClose();break;
			}
		}
		void onSysWM(SDL_SysWMmsg *msg){}
		void onSysWMEvent(SDL_SysWMEvent &syswm){static_cast<T*>(this)->onSysWM(syswm.msg);}

		void onKeyDown     (SDL_Keycode sym, Uint16 mod){}
		void onKeyUp       (SDL_Keycode sym, Uint16 mod){}
		void onKeyDownEvent(SDL_KeyboardEvent &key){static_cast<T*>(this)->onKeyDown(key.keysym.sym, key.keysym.mod);}
		void onKeyUpEvent  (SDL_KeyboardEvent &key){static_cast<T*>(this)->onKeyUp(key.keysym.sym, key.keysym.mod);}

		void onTextEditing(SDL_TextEditingEvent &edit){}
		void onTextInput(SDL_TextInputEvent &text){}

		void onMouseMotion         (              Sint32 x, Sint32 y){}
		void onMouseButtonDown     (Uint8 button, Sint32 x, Sint32 y){}
		void onMouseButtonUp       (Uint8 button, Sint32 x, Sint32 y){}
		void onMouseWheel          (              Sint32 x, Sint32 y){}
		void onMouseMotionEvent    (SDL_MouseMotionEvent &motion){static_cast<T*>(this)->onMouseMotion(motion.x, motion.y);}
		void onMouseButtonDownEvent(SDL_MouseButtonEvent &button){static_cast<T*>(this)->onMouseButtonDown(button.button, button.x, button.y);}
		void onMouseButtonUpEvent  (SDL_MouseButtonEvent &button){static_cast<T*>(this)->onMouseButtonUp(button.button, button.x, button.y);}
		void onMouseWheelEvent     (SDL_MouseWheelEvent &wheel){static_cast<T*>(this)->onMouseWheel(wheel.x, wheel.y);}

		void onJoyAxisMotion     (SDL_JoystickID which, Uint8 axis,   Sint16 value){}
		void onJoyBallMotion     (SDL_JoystickID which, Uint8 ball,   Sint16 xrel, Sint16 yrel){}
		void onJoyHatMotion      (SDL_JoystickID which, Uint8 hat,    Uint8 value){}
		void onJoyButtonDown     (SDL_JoystickID which, Uint8 button, Uint8 state){}
		void onJoyBUttonUp       (SDL_JoystickID which, Uint8 button, Uint8 state){}
		void onJoyAxisMotionEvent(SDL_JoyAxisEvent &jaxis){static_cast<T*>(this)->onJoyAxisMotion(jaxis.which, jaxis.axis, jaxis.value);}
		void onJoyBallMotionEvent(SDL_JoyBallEvent &jball){static_cast<T*>(this)->onJoyBallMotion(jball.which, jball.ball, jball.xrel, jball.yrel);}
		void onJoyHatMotionEvent (SDL_JoyHatEvent &jhat){static_cast<T*>(this)->onJoyHatMotion(jhat.which, jhat.hat, jhat.value);}
		void onJoyButtonDownEvent(SDL_JoyButtonEvent &jbutton){static_cast<T*>(this)->onJoyButtonDown(jbutton.which, jbutton.button, jbutton.state);}
		void onJoyBUttonUpEvent  (SDL_JoyButtonEvent &jbutton){static_cast<T*>(this)->onJoyBUttonUp(jbutton.which, jbutton.button, jbutton.state);}
		void onJoyDeviceAdded    (SDL_JoyDeviceEvent &jdevice){}
		void onJoyDeviceRemoved  (SDL_JoyDeviceEvent &jdevice){}

		void onControllerAxisMotion(SDL_ControllerAxisEvent &caxis){}
		void onControllerButtonDown(SDL_ControllerButtonEvent &cbutton){}
		void onControllerButtonUp(SDL_ControllerButtonEvent &cbutton){}
		void onControllerDeviceAdded(SDL_ControllerDeviceEvent &cdevice){}
		void onControllerDeviceRemoved(SDL_ControllerDeviceEvent &cdevice){}
		void onControllerDeviceMapped(SDL_ControllerDeviceEvent &cdevice){}

		void onFingerDown  (float x, float y){}
		void onFingerUp    (float x, float y){}
		void onFingerMotion(float x, float y){}
		void onFingerDownEvent(SDL_TouchFingerEvent &tfinger){static_cast<T*>(this)->onFingerDown(tfinger.x, tfinger.y);}
		void onFingerUpEvent(SDL_TouchFingerEvent &tfinger){static_cast<T*>(this)->onFingerUp(tfinger.x, tfinger.y);}
		void onFingerMotionEvent(SDL_TouchFingerEvent &tfinger){static_cast<T*>(this)->onFingerMotion(tfinger.x, tfinger.y);}
		
		void onClipboardUpdate(){}
		void onDropFile(char *file){}
		void onDropFileEvent(SDL_DropEvent &drop){static_cast<T*>(this)->onDropFile(drop.file);SDL_free(drop.file);}

		void onTimer(){}
		void onUserEvent(int code, void *data1, void *data2){}
	};
}

#endif
