#include "Server.h"
#include "ServerProcess.h"

using namespace std;

Server::Server(key_t& key) : num_clients_(0), key_(key), connection_(key) {
  cerr << "Server started." << endl;
  cerr << "Listening..." << endl << endl;
  
  this->Listen();
}

int Server::Listen() {
  MSG msg;
  int pid;
  
  while (true) {
    connection_.Listen(1, msg);
    cout << "Server::Listen(); Msg received." << endl;
    cout << "\tType:\n\t\t" << msg.type << endl;
    cout << "\tSender's PID:\n\t\t" << msg.sender_pid << endl;
    cout << "\tMessage: (Length: " << msg.data_len << ")\n\t\t" << msg.data << endl;
    
    // Create child process to manage client connection.
    if ((pid = fork()) == -1) {
      perror("Server::Listen; fork():");
    }
    
    // Send process off to process client's request.
    if (pid == 0) {
      if (!this->ProcessMessage(msg)) {
        // ERROR.
      }
      
      // End child process.
      exit(0);
    
    }
  }
  
  return 0;
}

bool Server::ProcessMessage(MSG& msg) {
  // Determine queue share based on priority.
  size_t queue_share;
  
  queue_share = 1;
  
  ServerProcess client_proc(connection_, msg.sender_pid, queue_share);
  client_proc.Respond(msg.data);
  
  return true;
}