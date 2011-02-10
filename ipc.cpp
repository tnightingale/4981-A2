#include "ipc.h"

using namespace std;

Connection::Connection(key_t& key, bool create) {  
  if (create) {
    qid_ = Connection::CreateQueue(key);
  } else {
    if ((qid_ = Connection::ConnectQueue(key)) == -1) {
      // Throw exception.
    }
  }
}

int Connection::Listen(int type, MSG& msg, int flags) {
  size_t length = sizeof(MSG) - sizeof(msg.type);
  
  if ((msgrcv(this->qid_, &msg, length, type, flags) < 0)) {
    if (errno == EINTR || errno == ENOMSG) {
      return errno; 
    }
    perror("Connection::Listen: Error; Failed reading from queue.");
    return -1;
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
