#ifndef SDL2_WRAPPER_NET_H
#define SDL2_WRAPPER_NET_H
/*Copyright (c) 2014 Silica
This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.*/

#include "SDL2.h"
#include "SDL2/SDL_net.h"

#pragma comment(lib, "SDL2_net.lib")

namespace SDL2
{
	struct IPAddress
	{
		IPAddress(){}
		IPAddress(const char *host, Uint16 port){SDLNet_ResolveHost(&ip, host, port);}
		IPAddress(const IPaddress &ipaddress):ip(ipaddress){}
		operator       IPaddress*()     {return &ip;}
		operator const IPaddress*()const{return &ip;}
		operator       IPaddress&()     {return ip;}
		operator const IPaddress&()const{return ip;}
		Uint32 host(){return ip.host;}
		Uint16 port(){return ip.port;}
		int ResolveHost(const char *host, Uint16 port){return SDLNet_ResolveHost(&ip, host, port);}
		const char *ResolveIP(){return SDLNet_ResolveIP(&ip);}
		IPaddress ip;
	};

	class TCP
	{
	public:
		TCP(){}
		TCP(const char *host, Uint16 port):socket(new TCPsocket(SDLNet_TCP_Open(IPAddress(host, port)))){}
		TCP(Uint16 port)                  :socket(new TCPsocket(SDLNet_TCP_Open(IPAddress(nullptr, port)))){}
		TCP(IPAddress &ip)                :socket(new TCPsocket(SDLNet_TCP_Open(ip))){}
		TCP(TCPsocket s)                  :socket(new TCPsocket(s)){}
		      TCPsocket get()          {return *socket;}
		const TCPsocket get()     const{return *socket;}
		operator       TCPsocket()     {return *socket;}
		operator const TCPsocket()const{return *socket;}
		operator bool(){return *socket;}

		Uint32    host()          {return SDLNet_TCP_GetPeerAddress(socket)->host;}
		Uint16    port()          {return SDLNet_TCP_GetPeerAddress(socket)->port;}
		IPAddress GetPeerAddress(){return *SDLNet_TCP_GetPeerAddress(socket);}

		int Ready(){return SDLNet_SocketReady(*socket);}

		TCP Accept()                            {return SDLNet_TCP_Accept(socket);}
		int Send  (const void *data, int len)   {return SDLNet_TCP_Send(socket, data, len);}
		int Recv  (      void *data, int maxlen){return SDLNet_TCP_Recv(socket, data, maxlen);}

		TCP WaitForAccept(Uint32 timeout = 0)
		{
			TCP tcp = SDLNet_TCP_Accept(socket);
			Uint32 count = 0;
			while (!tcp)
			{
				SDL_Delay(100);
				tcp = SDLNet_TCP_Accept(socket);
				if (++count > timeout)
					break;
			}
			return tcp;
		}
	private:
		struct Deleter{void operator()(TCPsocket *s){if (*s)SDLNet_TCP_Close(*s);delete s;}};
		shptr<TCPsocket, Deleter> socket;
	};

	class Packet
	{
	public:
		Packet(){}
		Packet(int size)    :packet(SDLNet_AllocPacket(size)){}
		Packet(UDPpacket *p):packet(p){}
		      UDPpacket *get()          {return packet;}
		const UDPpacket *get()     const{return packet;}
		operator       UDPpacket*()     {return packet;}
		operator const UDPpacket*()const{return packet;}
		operator bool(){return packet;}

		int       channel() {return packet->channel;}
		Uint8*    data()    {return packet->data;}
		int&      len()     {return packet->len;}
		int&      len(int l){return packet->len = l;}
		int       maxlen()  {return packet->maxlen;}
		int       status()  {return packet->status;}
		IPaddress &address(){return packet->address;}

		int Resize(int size){return SDLNet_ResizePacket(packet, size);}
		//UDPpacket **SDLNet_AllocPacketV(int howmany, int size)
		//void SDLNet_FreePacketV(UDPpacket **packetV)
	private:
		struct Deleter{void operator()(UDPpacket *p){SDLNet_FreePacket(p);}};
		shptr<UDPpacket, Deleter> packet;
	};

	class UDP
	{
	public:
		UDP(){}
		UDP(int port)   :socket(new UDPsocket(SDLNet_UDP_Open(port))){} // overload is ambiguous if port is 0 and port type is Uint16
		UDP(UDPsocket s):socket(new UDPsocket(s)){}
		      UDPsocket get()          {return *socket;}
		const UDPsocket get()     const{return *socket;}
		operator       UDPsocket()     {return *socket;}
		operator const UDPsocket()const{return *socket;}
		operator bool(){return *socket;}

		Uint32    host(int channel)          {return SDLNet_UDP_GetPeerAddress(socket, channel)->host;}
		Uint16    port(int channel)          {return SDLNet_UDP_GetPeerAddress(socket, channel)->port;}
		IPAddress GetPeerAddress(int channel){return *SDLNet_UDP_GetPeerAddress(socket, channel);}

		int Ready(){return SDLNet_SocketReady(*socket);}

		int  Bind  (int channel, IPAddress address){return SDLNet_UDP_Bind(socket, channel, address);}
		void Unbind(int channel)                    {SDLNet_UDP_Unbind(socket, channel);}

		int Send(int channel, Packet &packet){return SDLNet_UDP_Send(socket, channel, packet);}
		int Recv(             Packet &packet){return SDLNet_UDP_Recv(socket, packet);}
		//int SDLNet_UDP_SendV(UDPsocket sock, UDPpacket **packetV, int npackets)
		//int SDLNet_UDP_RecvV(UDPsocket sock, UDPpacket **packetV)
	private:
		struct Deleter{void operator()(UDPsocket *s){if (*s)SDLNet_UDP_Close(*s);delete s;}};
		shptr<UDPsocket, Deleter> socket;
	};

	class SocketSet
	{
	public:
		SocketSet(){}
		SocketSet(int maxsockets)    :set(new SDLNet_SocketSet(SDLNet_AllocSocketSet(maxsockets))){}
		SocketSet(SDLNet_SocketSet s):set(new SDLNet_SocketSet(s)){}
		      SDLNet_SocketSet get()          {return *set;}
		const SDLNet_SocketSet get()     const{return *set;}
		operator       SDLNet_SocketSet()     {return *set;}
		operator const SDLNet_SocketSet()const{return *set;}
		operator bool(){return *set;}

		int Add(TCP &sock){return SDLNet_TCP_AddSocket(set, sock);}
		int Add(UDP &sock){return SDLNet_UDP_AddSocket(set, sock);}
		int Del(TCP &sock){return SDLNet_TCP_DelSocket(set, sock);}
		int Del(UDP &sock){return SDLNet_UDP_DelSocket(set, sock);}
		int Check(Uint32 timeout = 0){return SDLNet_CheckSockets(set, timeout);}
	private:
		struct Deleter{void operator()(SDLNet_SocketSet *s){if (*s)SDLNet_FreeSocketSet(*s);delete s;}};
		shptr<SDLNet_SocketSet, Deleter> set;
	};

	class Net
	{
	public:
		Net(){SDLNet_Init();}
		~Net(){SDLNet_Quit();}

		const char* GetError(){return SDLNet_GetError();}

		int         ResolveHost(IPAddress &address, const char *host, Uint16 port){return SDLNet_ResolveHost(address, host, port);}
		const char *ResolveIP  (IPAddress &address){return SDLNet_ResolveIP(address);}

		TCP TCP_Open(IPAddress &ip){return SDLNet_TCP_Open(ip);}
		TCP TCP_Listen(Uint16 port){IPAddress ip(NULL, port);return SDLNet_TCP_Open(ip);}

		SocketSet AllocSocketSet(int maxsockets){return SDLNet_AllocSocketSet(maxsockets);}
	};
}

#endif
