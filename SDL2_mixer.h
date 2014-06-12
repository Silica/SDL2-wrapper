#ifndef SDL2_WRAPPER_MIXER_H
#define SDL2_WRAPPER_MIXER_H
/*Copyright (c) 2014 Silica
This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.*/

#include "SDL2.h"
#include "SDL2/SDL_mixer.h"

#pragma comment(lib, "SDL2_mixer.lib")

namespace SDL2
{
	class Chunk
	{
	public:
		Chunk(){}
		Chunk(Mix_Chunk *c):chunk(c){}
		      Mix_Chunk *get()          {return chunk;}
		const Mix_Chunk *get()     const{return chunk;}
		operator       Mix_Chunk*()     {return chunk;}
		operator const Mix_Chunk*()const{return chunk;}
		operator bool(){return chunk;}

		int Volume(int volume){return Mix_VolumeChunk(chunk, volume);}

		int Play       (int channel = 0, int loops = 0)                   {return Mix_PlayChannel(channel, chunk, loops);}
		int PlayTimed  (int channel = 0, int loops = 0, int ticks = -1)   {return Mix_PlayChannelTimed(channel, chunk, loops, ticks);}
		int FadeIn     (int channel,     int loops,     int ms)           {return Mix_FadeInChannel(channel, chunk, loops, ms);}
		int FadeInTimed(int channel,     int loops,     int ms, int ticks){return Mix_FadeInChannel(channel, chunk, loops, ms);}
	private:
		struct Deleter{void operator()(Mix_Chunk *c){Mix_FreeChunk(c);}};
		shptr<Mix_Chunk, Deleter> chunk;
	};

	struct Channel
	{
		int channel;
		Channel(int c){channel = c;}
		operator int()const{return channel;}

		int Play       (Chunk &chunk, int loops = 0)                       {return Mix_PlayChannel(channel, chunk, loops);}
		int PlayTimed  (Chunk &chunk, int loops = 0, int ticks = -1)       {return Mix_PlayChannelTimed(channel, chunk, loops, ticks);}
		int FadeIn     (Chunk &chunk,     int loops,     int ms)           {return Mix_FadeInChannel(channel, chunk, loops, ms);}
		int FadeInTimed(Chunk &chunk,     int loops,     int ms, int ticks){return Mix_FadeInChannel(channel, chunk, loops, ms);}

		int Halt()           {return Mix_HaltChannel(channel);}
		int Expire(int ticks){return Mix_ExpireChannel(channel, ticks);}
		int FadeOut(int ms)  {return Mix_FadeOutChannel(channel, ms);}

		int Playing(){return Mix_Playing(channel);}
		int Paused() {return Mix_Paused(channel);}
		int Fading() {return Mix_FadingChannel(channel);}

		//Mix_Chunk *GetChunk(){return Mix_GetChunk(channel);}
	};

	class Mix
	{
	public:
		int    frequency;
		Uint16 format;
		int    channels;
		int    chunksize;
		int    numchans;
		Mix(int numchans = 16, int flags = MIX_INIT_OGG, int frequency = 44100, Uint16 format = MIX_DEFAULT_FORMAT, int channels = 2, int chunksize = 1024)
		{
			this->frequency = frequency;
			this->format = format;
			this->channels = channels;
			this->chunksize = chunksize;
			Mix_Init(flags);
			if (!Mix_OpenAudio(frequency, format, channels, chunksize))
				Mix_QuerySpec(&this->frequency, &this->format, &this->channels);
			this->numchans = Mix_AllocateChannels(numchans);
		}
		~Mix()
		{
			Mix_CloseAudio();
			Mix_Quit();
		}

		const char* GetError(){return Mix_GetError();}

		Chunk LoadWAV(const char *file){return Mix_LoadWAV(file);}
		void ChannelFinished(void (*channel_finished)(int channel)){Mix_ChannelFinished(channel_finished);}
	};
}

#endif
