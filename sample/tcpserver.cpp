#include <stdio.h>
#include "SDL2_net.h"
using namespace SDL2;

#pragma comment(linker, "/subsystem:console")
int main(int argc, char **argv)
{
	SDL sdl;
	Net net;
	TCP server(9999);
	if (!server)
	{
		printf("server:%s\n", net.GetError());
		return 1;
	}

	//TCP tcp = server.WaitForAccept(100);

	SocketSet set(2);
	set.Add(server);
	if (set.Check(10000))
	{
		TCP tcp = server.Accept();
		if (!tcp)
		{
			printf("tcp:%s\n", net.GetError());
		}
		else
		{
			tcp.Send("message", 7);
		}
	}

	return 0;
}
