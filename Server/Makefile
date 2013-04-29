All: Server 

Server: install_server.o install_internetFuncs.o install_calculations.o
	gcc install_server.o install_internetFuncs.o install_calculations.o -o Server -lpthread -lm  
    
install_calculations.o: calculations.c calculations.h
	gcc -c calculations.c -o install_calculations.o   

install_server.o:   server.c 
	gcc -c server.c -o install_server.o
    
install_internetFuncs.o: internetFuncs.h internetFuncs.c
	gcc -c	internetFuncs.c	 -o install_internetfuncs.o

clean:
	rm *.o
	rm *~
    
clean_all:
	rm Server Client
	rm *.o
	rm *~