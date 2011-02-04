#include "ipc.h"

using namespace std;

Connection::Connection(key_t& key) {  
  if ((qid_ = msgget(key, 0644 | IPC_CREAT)) == -1) {
    perror("Connection::Connection: Error; Failed getting queue.");
  }
  cerr << "Connection::Connection: Connection established." << endl;
  cerr << "\tqid_: " << hex << showbase << qid_ << endl;
}

bool Connection::Listen(int type, MSG& msg) {
  size_t length = sizeof(MSG) - sizeof(msg.type);

  if (msgrcv(this->qid_, &msg, length, type, 0 & IPC_NOWAIT) == -1) {
    perror("Connection::Listen: Error; Failed reading from queue.");
    return false;
  }
  
  return true;
}

int Connection::Write(MSG& msg) {
  size_t length = sizeof(MSG) - sizeof(msg.type);
  
  if (msgsnd(this->qid_, &msg, length, 0) == -1) {
    perror("Connection::Write: Error; Failed writing to queue.");
  }
  
  return 0;
}