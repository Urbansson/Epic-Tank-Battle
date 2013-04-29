All: Server 

Server: install_server.o install_internetFuncs.o
	gcc install_server.o install_internetFuncs.o -o Server -lpthread -lm  
    
Client: install_client.o
	gcc install_client.o -o Client -lm -ldl -lpthread -lSDL -lglut   

install_server.o:   server.c 
	gcc -c server.c -o install_server.o
    
install_client.o:   client.c 
	gcc -c client.c -o install_client.o

install_internetFuncs.o: internetFuncs.h internetFuncs.c
	gcc -c	internetFuncs.c	-o install_internetfuncs.o
    
clean:
	rm *.o
	rm *~
    
clean_all:
	rm Server Client
	rm *.o
	rm *~