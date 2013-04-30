All: Client

Client: install_client.o install_clientinternetfuncs.o install_SDLfuncs.o install_timers.o protocol.h install_render.o
	gcc install_client.o install_clientinternetfuncs.o install_SDLfuncs.o install_timers.o install_render.o -o Client -lm -ldl -lpthread -lSDL -lglut -lSDL_image -O3  

    
install_client.o:   client.c protocol.h
	gcc -c client.c -o install_client.o

install_clientinternetfuncs.o: clientinternetFuncs.h clientinternetFuncs.c protocol.h
	gcc -c	clientinternetFuncs.c	-o install_clientinternetfuncs.o

install_SDLfuncs.o: SDLfuncs.h	SDLfuncs.c protocol.h
	gcc -c SDLfuncs.c -o install_SDLfuncs.o

install_timers.o: timers.h	timers.c protocol.h
	gcc -c timers.c -o install_timers.o
    
install_render.o: render.h	render.c
	gcc -c render.c -o install_render.o

clean:
	rm *.o
	rm *~
    
clean_all:
	rm Server Client
	rm *.o
	rm *~
