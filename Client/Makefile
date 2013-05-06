All: Client

Client: install_client.o install_clientinternetfuncs.o install_SDLfuncs.o install_timers.o protocol.h install_render.o install_audio.o
	gcc install_client.o install_clientinternetfuncs.o install_SDLfuncs.o install_timers.o install_render.o install_audio.o -o Client -lm -ldl -lpthread -lSDL -lglut -lSDL_image -O3 -lGL -lSDL_mixer 

    
install_client.o:   Client.c protocol.h
	gcc -c Client.c -o install_client.o

install_clientinternetfuncs.o: clientinternetfuncs.h clientinternetfuncs.c protocol.h
	gcc -c	clientinternetfuncs.c	-o install_clientinternetfuncs.o

install_SDLfuncs.o: SDLfuncs.h	SDLfuncs.c protocol.h
	gcc -c SDLfuncs.c -o install_SDLfuncs.o

install_timers.o: timers.h	timers.c protocol.h
	gcc -c timers.c -o install_timers.o
    
install_render.o: render.h	render.c
	gcc -c render.c -o install_render.o 

install_audio.o: audio.h	audio.c
	gcc -c audio.c -o install_audio.o

clean:
	rm *.o
	rm *~
    
clean_all:
	rm Server Client
	rm *.o
	rm *~
