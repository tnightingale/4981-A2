#include "ipc.h"

using namespace std;

Connection::Connection(key_t& key) {  
  if ((qid_ = msgget(key, 0644 | IPC_CREAT)) == -1) {
    perror("Connection::Connection: Error; Failed getting queue.");
  }
  cerr << "Connection::Connection: Connection established." << endl;
  cerr << "\tqid_: " << hex << showbase << qid_ << endl;
}

int Connection::Listen(int type, MSG& msg, int flags) {
  size_t length = sizeof(MSG) - sizeof(msg.type);
  
  if ((msgrcv(this->qid_, &msg, length, type, flags) < 0)) {
    if (errno == EINTR || errno == ENOMSG) {
      return errno; 
    }
    perror("Connection::Listen: Error; Failed reading from queue.");
    return errno;
  }
  
  return 0;
}

int Connection::Write(MSG& msg, int flags) {
  size_t length = sizeof(MSG) - sizeof(msg.type);
  
  if (msgsnd(this->qid_, &msg, length, flags) == -1) {
    if (errno == EAGAIN) {
      return errno;
    }
    perror("Connection::Write: Error; Failed writing to queue.");
    return -1;
  }
  
  return 0;
}

void Connection::Cleanup(int msg_type) {
  MSG msg;
  while (Listen(msg_type, msg, IPC_NOWAIT) != ENOMSG) ;
}
