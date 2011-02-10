#ifndef IPC_H
#define IPC_H

#ifdef __linux__
#include <stdio.h>
#endif

#include <sys/msg.h>
#include <iostream>
#include <errno.h>

#if defined(__APPLE__)
#define MSGSIZE 2048
#else
#define MSGSIZE 4096
#endif

#define MSGQUEUE_TMP "/tmp/"
#define MAXMESGDATA (MSGSIZE - sizeof(long) - (2 * sizeof(int)) - sizeof(size_t))

typedef struct _MSG_ {
  long type;   // message type
  int sender_pid;
  int priority;
  size_t data_len;  // #bytes in mesg_data
  char data[MAXMESGDATA];
} MSG;

class Connection
{
public:
  Connection(key_t& key, bool create = true);
  virtual ~Connection() {}
  
  bool Create();
  int Listen(int msg_type, MSG& msg, int flags = 0);
  int Write(MSG& msg, int flags = 0);
  void Cleanup(int msg_type);
  
  static int CreateQueue(key_t& key) {
    int qid;
    if ((qid = msgget(key, 0644 | IPC_CREAT | IPC_EXCL)) == -1) {
      perror("Connection::CreateQueue: Error; Failed getting queue.");
      throw "Exception: Failed creating message queue.";
    }
  
    return qid;
  }

  static int ConnectQueue(key_t& key) {
    int qid;
    if ((qid = msgget(key, 0644)) == -1) {
      perror("Connection::ConnectQueue: Error; Failed getting queue.");
      throw "Exception: Failed connecting to message queue.";
    }
  
    return qid;
  }
  
  static key_t GetResKey() {
    key_t key;

    if ((key = ftok(MSGQUEUE_TMP, 1)) < 0) {
      perror("Connection::GetResKey(); Error: Could not create key.");
      return 1; 
    }
    
    return key;
  }
  
  static int DestroyQueue(int qid) {
    if (msgctl(qid, IPC_RMID, 0) == -1) {        
      return -1;     
    }

    return (0);
  }


private:
  int qid_;
};

#endif
