All: Server 

Server: install_server.o install_internetfuncs.o install_debugger.o install_collision.o install_clientthreads.o
	gcc install_server.o install_internetfuncs.o install_debugger.o install_collision.o install_clientthreads.o -o Server -lpthread -lm  
    
install_debugger.o: debugger.c debugger.h
	gcc -c debugger.c -o install_debugger.o   

install_server.o:   server.c 
	gcc -c server.c -o install_server.o
    
install_internetfuncs.o: internetFuncs.h internetFuncs.c
	gcc -c	internetFuncs.c	 -o install_internetfuncs.o
    
install_collision.o: collision.h collision.c
	gcc -c	collision.c	 -o install_collision.o
    
install_clientthreads.o: clientThreads.h clientThreads.c
	gcc -c	clientThreads.c	 -o install_clientthreads.o

clean:
	rm *.o
	rm *~
    
clean_all:
	rm Server Client
	rm *.o
	rm *~