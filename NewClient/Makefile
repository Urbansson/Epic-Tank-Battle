All: Client

Client:   install_client.o  install_init.o install_audio.o install_clientinternetfuncs.o    install_SDLfuncs.o	install_timers.o install_calculations.o install_rotozoom.o
	gcc install_client.o install_init.o install_audio.o install_clientinternetfuncs.o install_SDLfuncs.o install_timers.o install_calculations.o install_rotozoom.o -o Client -ldl -lm -lSDL -lSDL_mixer -lSDL_image -lpthread -O3
 
install_client.o:   Client.c 
	gcc -c Client.c -o install_client.o

install_init.o:   init.h init.c
	gcc -c init.c -o install_init.o
    
install_audio.o: audio.h	audio.c
	gcc -c audio.c -o install_audio.o
    
install_clientinternetfuncs.o: clientinternetfuncs.h clientinternetfuncs.c protocol.h
	gcc -c	clientinternetfuncs.c	-o install_clientinternetfuncs.o
    
install_SDLfuncs.o: SDLfuncs.h	SDLfuncs.c protocol.h
	gcc -c SDLfuncs.c -o install_SDLfuncs.o

install_timers.o: timers.h	timers.c
	gcc -c timers.c -o install_timers.o

install_calculations.o: calculations.c calculations.h
	gcc -c calculations.c -o install_calculations.o 
    
install_rotozoom.o: SDL_rotozoom.c SDL_rotozoom.h
	gcc -c SDL_rotozoom.c -o install_rotozoom.o 
    
clean:
	rm *.o
	rm *~
    
clean_all:
	rm Client
	rm *.o
	rm *~
