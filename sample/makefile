CC     = clang++
CFLAGS = -std=c++11 -lSDL2 -lSDL2main

all:
	$(CC) $(CFLAGS)              -o lifegame   lifegame.cpp
	$(CC) $(CFLAGS) -lSDL2_ttf   -o helloworld helloworld.cpp
	$(CC) $(CFLAGS) -lSDL2_image -o image      image.cpp
	$(CC) $(CFLAGS) -lSDL2_mixer -o playaudio  playaudio.cpp
	$(CC) $(CFLAGS) -lSDL2_net   -o tcpserver  tcpserver.cpp
	$(CC) $(CFLAGS) -lSDL2_net   -o tcpclient  tcpclient.cpp
	$(CC) $(CFLAGS) -lSDL2_net   -o udpserver  udpserver.cpp
	$(CC) $(CFLAGS) -lSDL2_net   -o udpclient  udpclient.cpp
