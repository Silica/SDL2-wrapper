#include <stdio.h>
#include "SDL2_net.h"
using namespace SDL2;

#pragma comment(linker, "/subsystem:console")
int main(int argc, char **argv)
{
	SDL sdl;
	Net net;
	UDP udp(9999);

	SocketSet set(2);
	set.Add(udp);
	if (set.Check(10000))
	{
		Packet packet(1024);
		packet.data()[0] = 0;
		udp.Recv(packet);
		packet.data()[packet.len()] = 0;
		printf("%s\n", packet.data());
	}

	return 0;
}
