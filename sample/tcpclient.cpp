#include <stdio.h>
#include "SDL2_net.h"
using namespace SDL2;

#pragma comment(linker, "/subsystem:console")
int main(int argc, char **argv)
{
	SDL sdl;
	Net net;

	TCP tcp("127.0.0.1", 9999);
	if (!tcp)
	{
		printf("tcp:%s\n", net.GetError());
	}
	else
	{
		printf("connect ok,%X,%d\n", tcp.GetPeerAddress().host(), tcp.GetPeerAddress().port());

		char buf[1025];
		SocketSet set(2);
		set.Add(tcp);
		if (set.Check(10000))
		{
			int i = tcp.Recv(buf, 1024);
			buf[i] = 0;
			printf("%d,%s\n", i, buf);
		}
	}

	return 0;
}
