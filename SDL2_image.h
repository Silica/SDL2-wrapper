#ifndef SDL2_WRAPPER_IMAGE_H
#define SDL2_WRAPPER_IMAGE_H
/*Copyright (c) 2014 Silica
This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.*/

#include "SDL2.h"
#include "SDL2/SDL_image.h"

#pragma comment(lib, "SDL2_image.lib")

namespace SDL2
{
	class IMG
	{
	public:
		IMG(int flags = IMG_INIT_PNG){IMG_Init(flags);}
		~IMG(){IMG_Quit();}
		const char* GetError(){return IMG_GetError();}

		Surface Load(const char *file){return IMG_Load(file);}
		Surface Load_RW(RWops &src)   {return IMG_Load_RW(src, 0);}
	};
}

#endif
