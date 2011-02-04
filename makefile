# make for Semaphores

CC=g++
FLAGS=-ansi -Wall -W -pedantic -ggdb

server: server_main.o Server.o ServerProcess.o ipc.o
	$(CC) $(FLAGS) server_main.o Server.o ServerProcess.o ipc.o -o server

client: client_main.o Client.o ipc.o
	$(CC) $(FLAGS) client_main.o Client.o ipc.o -o client

clean:
	rm -f *.o *.bak server client

server_main.o: server_main.cpp
	$(CC) $(FLAGS) -O -c server_main.cpp

client_main.o: client_main.cpp
	$(CC) $(FLAGS) -0 -c client_main.cpp

Server.o: Server.cpp Server.h
	$(CC) $(FLAGS) -O -c Server.cpp

ServerProcess.o: ServerProcess.cpp ServerProcess.h
	$(CC) $(FLAGS) -O -c ServerProcess.cpp

Client.o: Client.cpp Client.h
	$(CC) $(FLAGS) -0 -c Client.cpp

ipc.o: ipc.cpp ipc.h
	$(CC) $(FLAGS) -O -c ipc.cpp
