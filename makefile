# make for Semaphores

CXX=g++
FLAGS=-ansi -Wall -W -pedantic -ggdb

server: server_main.o Server.o ServerProcess.o ipc.o
	$(CXX) $(FLAGS) $(PLATFORM) server_main.o Server.o ServerProcess.o ipc.o -o server

client: client_main.o Client.o ipc.o
	$(CXX) $(FLAGS) $(PLATFORM) client_main.o Client.o ipc.o -o client

clean:
	rm -f *.o *.bak server client

server_main.o: server_main.cpp
	$(CXX) $(FLAGS) $(PLATFORM) -O -c server_main.cpp

client_main.o: client_main.cpp
	$(CXX) $(FLAGS) $(PLATFORM) -O -c client_main.cpp

Server.o: Server.cpp Server.h
	$(CXX) $(FLAGS) $(PLATFORM) -O -c Server.cpp

ServerProcess.o: ServerProcess.cpp ServerProcess.h
	$(CXX) $(FLAGS) $(PLATFORM) -O -c ServerProcess.cpp

Client.o: Client.cpp Client.h
	$(CXX) $(FLAGS) $(PLATFORM) -O -c Client.cpp

ipc.o: ipc.cpp ipc.h
	$(CXX) $(FLAGS) $(PLATFORM) -O -c ipc.cpp
