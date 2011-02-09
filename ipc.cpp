#include "ipc.h"

using namespace std;

Connection::Connection(key_t& key) {  
  if ((qid_ = msgget(key, 0644 | IPC_CREAT)) == -1) {
    perror("Connection::Connection: Error; Failed getting queue.");
  }
  cerr << "Connection::Connection: Connection established." << endl;
  cerr << "\tqid_: " << hex << showbase << qid_ << endl;
}

int Connection::Listen(int type, MSG& msg) {
  size_t length = sizeof(MSG) - sizeof(msg.type);
  
  if ((msgrcv(this->qid_, &msg, length, type, 0) < 0)) {
    if (errno == EINTR) {
      return 1;
    }
    perror("Connection::Listen: Error; Failed reading from queue.");
    return -1;
  }
  
  return 0;
}

bool Connection::Write(MSG& msg) {
  size_t length = sizeof(MSG) - sizeof(msg.type);
  
  if (msgsnd(this->qid_, &msg, length, 0) == -1) {
    perror("Connection::Write: Error; Failed writing to queue.");
    return false;
  }
  
  return true;
}
