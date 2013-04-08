All: Server Client

Server: install_server.o
	gcc install_server.o -o Server  
    
Client: install_client.o
	gcc install_client.o -o Client  

install_server.o:   server.c 
	gcc -c server.c -o install_server.o
    
install_client.o:   client.c 
	gcc -c client.c -o install_client.o
    
clean:
	rm *.o
    
clean_all: clean
	rm Server Client