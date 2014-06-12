#ifndef SDL2_WRAPPER_TTF_H
#define SDL2_WRAPPER_TTF_H
/*Copyright (c) 2014 Silica
This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.*/

#include "SDL2.h"
#include "SDL2/SDL_ttf.h"

#pragma comment(lib, "SDL2_ttf.lib")

namespace SDL2
{
	class Font
	{
	public:
		Font(){}
		Font(TTF_Font *f):font(f){}
		      TTF_Font *get()          {return font;}
		const TTF_Font *get()     const{return font;}
		operator       TTF_Font*()     {return font;}
		operator const TTF_Font*()const{return font;}
		operator bool(){return font;}
		int  GetStyle()                            {return TTF_GetFontStyle(font);}
		void SetStyle(int style = TTF_STYLE_NORMAL){TTF_SetFontStyle(font, style);}
		int  GetOutline()                          {return TTF_GetFontOutline(font);}
		void SetOutline(int outline = 0)           {TTF_SetFontOutline(font, outline);}
		Surface RenderText_Solid  (const char *text, const Color &fg)                 {return TTF_RenderText_Solid  (font, text, fg);}
		Surface RenderText_Shaded (const char *text, const Color &fg, const Color &bg){return TTF_RenderText_Shaded (font, text, fg, bg);}
		Surface RenderText_Blended(const char *text, const Color &fg)                 {return TTF_RenderText_Blended(font, text, fg);}
	private:
		struct Deleter{void operator()(TTF_Font *f){TTF_CloseFont(f);}};
		shptr<TTF_Font, Deleter> font;
	};

	class TTF
	{
	public:
		TTF(){TTF_Init();}
		~TTF(){TTF_Quit();}
		int WasInit(){return TTF_WasInit();}
		operator bool(){return WasInit();}

		const char* GetError(){return TTF_GetError();}

		Font OpenFont(const char *file, int ptsize){return TTF_OpenFont(file, ptsize);}
	};
}

#endif
