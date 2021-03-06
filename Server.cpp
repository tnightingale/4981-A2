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
  bool running = true;
  int result = 0;
  
  while (running) {
    result = connection_.Listen(1, msg);
    // Interrupt: SIGCHLD, continue listening.
    if (result == EINTR) {
      continue;
    }
    
    // Error reading msg queue.
    else if (result < 0) {
      cerr << "Server::Listen(); Error receiving message." << endl;
      exit(2);
    }
    
    // Create child process to manage client connection.
    if ((pid = fork()) == -1) {
      perror("Server::Listen; fork():");
      exit(1);
    }
    
    // Send process off to process client's request.
    if (pid == 0) {
      if (!this->ProcessMessage(msg)) {
        exit(1);
      }
      
      // End child process.
      exit(0);
    } else {
      cout << "Connection: Client PID: " << msg.sender_pid;
      cout << " <--> ";
      cout << "ServerProcess PID: " << pid;
      cout << " | Priority: " << msg.priority << endl;
    }
  }
  
  return 0;
}

bool Server::ProcessMessage(MSG& msg) {
  ifstream file;
  
  // Determine queue share based on priority.
  size_t queue_share = GetShare(msg.priority);
  int proc_prio = GetProcPrio(msg.priority);

  if (setpriority(PRIO_PROCESS, 0, proc_prio) < 0) {
    perror("Server::ProcessMessage; setproirity()");
  }

  ServerProcess client_proc(connection_, msg.sender_pid, queue_share);
  
  if (!client_proc.FileAck(file, msg.data)) {
    // Error: Problem reading file.
    return false;
  }


  if (!client_proc.Respond(file)) {
    // Error: Problem responding to file.
    return false;
  }
  
  return true;
}

size_t Server::GetShare(int priority) {
  switch (priority) {
    case 1:
      return 10;
    case 2:
      return 7;
    case 3: 
      return 5;
    case 4:
      return 3;
    case 5:
      return 2;
    default:
      return 1;
  }
}

int Server::GetProcPrio(int priority) {
  switch (priority) {
    case 1:
      return 0;
    case 2:
      return 10;
    case 3: 
      return 12;
    case 4:
      return 15;
    case 5:
      return 17;
    default:
      return 19;
  }
}
