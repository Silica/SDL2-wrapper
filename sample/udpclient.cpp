#include <stdio.h>
#include <string.h>
#include "SDL2_net.h"
using namespace SDL2;

#pragma comment(linker, "/subsystem:console")
int main(int argc, char **argv)
{
	SDL sdl;
	Net net;
	UDP udp(0);

	Packet packet(1024);
	strcpy((char*)packet.data(), "hello world!");
	packet.len() = 12;
#if 0
	packet.address() = IPAddress("127.0.0.1", 9999);
	udp.Send(-1, packet);
#endif
	udp.Bind(1, IPAddress("127.0.0.1", 9999));
	udp.Send(1, packet);

	return 0;
}
