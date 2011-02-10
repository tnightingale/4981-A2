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
  Connection(key_t&);
  virtual ~Connection() {}
  
  bool Create();
  int Listen(int msg_type, MSG& msg, int flags = 0);
  int Write(MSG& msg, int flags = 0);
  void Cleanup(int msg_type);

private:
  int qid_;
};

#endif
