#ifndef IPC_H
#define IPC_H

#include <sys/msg.h>
#include <iostream>

#define MSGSIZE 2048
#define MSGQUEUE_TMP "/tmp/"
#define MAXMESGDATA (MSGSIZE - sizeof(long) - (2 * sizeof(int)) - sizeof(size_t))

//#define MESGHDRSIZE (sizeof(Mesg) - MAXMESGDATA) /* length of mesg_len and mesg_type */

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
  bool Listen(int, MSG&);
  bool Write(MSG&);

private:
  int qid_;
};

#endif